# r-backup

This is a work in progress. Here for backup reason.

However, it works an it is partially functional.

## Warning

This project is written with C++ 20

## Dependencies

* yaml-cpp
* gtest
* fakeit

## Build

Create a build directory

```shell
  $ mkdir build && cd build
``` 

Then build the projet makefiles

```shell
  $ cmake ..
```

And build the project

```shell
  $ cmake --build . -j 
``` 

## Launch

Create a configuration file into `mkdir ${HOME}/.config/r-backup/ && touch ${HOME}/.config/r-backup/config.yml`. There's
a lot of commentaries into the example configuration file to understand different behavioursi.

Then

```shell
  $ ./r-backup backup
``` 

To backup.

## Help

Use:

```shell
  $ ./r-backup --help
``` 

## Notes (in french) for myself.

Par défaut, on utilise root sauf s'il y a include si root absent alors on utilise ${home}

## Thingz todo

[ ] Force backup option (`--force`)
[ ] Override option (`--override`)
[-] Implement strategy
[ ] Create missing directories unless --no-create-destination option
