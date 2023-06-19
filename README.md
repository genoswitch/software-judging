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

## Linux Usage

<!--  Instructions derived from https://support.microbit.org/support/solutions/articles/19000105428-webusb-troubleshooting -->

On Linux, this hardware will not work out of the box. To enable the hardware to work, please follow these steps:

1. Close your web browser

2. Check that the `plugdev` group exists and create the group if it does not. (Fedora/Arch)

| Web browsers expect to run as a user in the `plugdev` to be able to access physical devices.

```shell
getent group plugdev >/dev/null || sudo groupadd -r plugdev
```

3. Copy `50-genoswitch.rules` to `/etc/udev/rules.d/`

```shell
sudo cp 50-genoswitch.rules /etc/udev/rules.d/
```

4. Add you user to the plugdev group.

| Replace with your username!

```shell
sudo usermod -a -G plugdev <username>
```

5. Reload udev rules

```shell
sudo udevadm control --reload-rules
```

6. Log out and log back in for the new rules and grouping to take effect.
