# Genoswitch MP Firmware

## Installation

### Fetch submodules

```shell
git submodule init
git submodule update --depth 1
```

### Toolchain

To build this project, a number of extra tools and libraries are required.

Here are the installation instructions for Linux.

#### Debian-based distros

```shell
sudo apt update
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential libstdc++-arm-none-eabi-newlib python3 git nodejs
```

#### Arch-based distros

~2GB install.

```shell
sudo pacman -S cmake arm-none-eabi-gcc arm-none-eabi-newlib arm-none-eabi-gdb arm-none-eabi-binutils python git nodejs npm
```

## Licensing

The codebase is licensed under MIT.

However, certain files are licensed using other open-source licenses. These files are mainly ones that have heavily based on files from other projects.

A SPDX license header is included in every file in the repository to easily tell which license is in use.
