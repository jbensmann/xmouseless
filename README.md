# xmouseless

This program is a replacement for the physical mouse in X11.

It aims to be simple and efficient.

Features:
- move the mouse with different speeds
- click and grab
- execute shell commands

## Installation

```
make
sudo make install
```

If you are not using an Arch based distro you might have to install some headers, e.g. on
Debian based distros:
```
sudo apt-get install libx11-dev libxtst-dev
```

## Configuration

Edit config.h and reinstall. The configuration file should be self explaining.

## Usage

When starting xmouseless, it grabs the keyboard and all defined bindings are
available. When pressing an exit key, the program exits.

You probably want to define a key binding for xmouseless in your desktop
environment or window manager.
