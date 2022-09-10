#!/usr/bin/env python3
import argparse
import datetime
import os
import shutil
import tempfile
from pathlib import Path

parser = argparse.ArgumentParser()
parser.add_argument("--debug", action="store_true", default=False)
parser.add_argument("--rebuild", action="store_true", default=False)
options = parser.parse_args()

current_dir = Path(".").resolve()
build_dir = Path(tempfile.gettempdir()) / "r-backup-build"
destination_dir = build_dir / "mixins" / "dest" / "r-backup"

try:
    # Prepare
    os.environ["CONFIG_ROOT_PATH"] = str(current_dir)
    os.makedirs(build_dir, exist_ok=True)
    os.chdir(build_dir)

    # Build
    if not (build_dir / "src" / "r-backup").exists() or options.rebuild:
        assert not os.system(f"cmake {current_dir}")
        assert not os.system("cmake --build . -j")
        assert not os.system(build_dir / "tests" / "r-backup_test")

    # # Backup
    r_backup_bin = build_dir / "src" / "r-backup"
    config_path = current_dir / "mixins" / "config.yaml"
    shutil.copytree(current_dir / "mixins", build_dir / "mixins", dirs_exist_ok=True)
    os.makedirs(destination_dir, exist_ok=True)
    assert not os.system(f"{r_backup_bin} backup --verbose --config-file {config_path}")

    # Assert result
    backup_dir = destination_dir / datetime.datetime.now().strftime("%Y-%m-%d")
    shutil.rmtree(backup_dir, ignore_errors=True)
    
    expected_tree = [
        (False, backup_dir / "truc"),
        (False, backup_dir / "truc" / "shouldnt-be-copied.txt"),
        (False, backup_dir / "truc" / ".ignore"),
        (False, backup_dir / "truc" / ".ignore" / "not-this-file"),
        (False, backup_dir / "one" / "exclude_me"),
        (False, backup_dir / "one" / "exclude_me" / "onetwothree.cpp"),
        (False, backup_dir / "one" / "exclude_me" / "onetwothree.hpp"),
        (False, backup_dir / "one" / "two" / "three" / "exclude_me"),
        (False, backup_dir / "one" / "two" / "three" / "exclude_me" / "a file.txt"),
        (True, backup_dir / "one" / "copy-this-file.txt"),
        (True, backup_dir / "one" / "two" / "this-file-should-exists.md"),
        (True, backup_dir / "one" / "two" / "three" / "go.txt"),
        (True, backup_dir / "copy-this-file.txt"),
        (True, backup_dir / "file-sample-1.yml"),
    ]

    for exists, expected in expected_tree:
        status, neg = ("OK", "") if exists else ("KO", "should not")
        if options.debug:
            print(f"{status}: {expected} {neg} exists")
        else:
            assert exists == expected.exists()

finally:
    # Restore
    os.chdir(current_dir)
    if not options.debug:
        shutil.rmtree(build_dir, ignore_errors=True)
