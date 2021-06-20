# r-backup

This is a work in progress. Here for backup reason.

However, it works an it is partially functional. 

## Build

Install and configure [conan](https://conan.io/)

Create a build directory

  $ mkdir build && cd build

Then launch conan package manager. The argument `--build=missing` will build libraries if the binary is missing

  $ conan install .. --build=missing

Build the project

  $ cmake --build . -j 

## Launch

Create a configuration file into `mkdir ${HOME}/.config/r-backup/ && touch ${HOME}/.config/r-backup/config.yml`. There's a lot of commentary to understand different behaviours.

Then 

  $ ./r-backup backup
  
To backup.

## Help

Use:
  $ ./r-backup --help

## Notes (in french) for myself.

Par défaut, on utilise root sauf s'il y a include
si root absent alors on utilise ${home}

## Thingz todo

[ ] Force backup option (`--force`)
[ ] Override option (`--override`)
[-] Implement strategy
