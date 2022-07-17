# xmouseless

This program is a replacement for the physical mouse in Linux.

Features:

- move the mouse with different speeds
- click and grab
- scroll
- execute shell commands

## Installation

```
make
sudo make install
```

If you are not using an Arch based distro you might have to install some headers, e.g. on Debian based distros:

```
sudo apt-get install libx11-dev libxtst-dev
```

## Usage

When starting xmouseless, it grabs the keyboard and all defined bindings are available. When pressing an exit key, the
program exits.

The usage is quite intuitive and with some practice, you can move the pointer to a specific location very fast.
Basically, you move the pointer by pressing some keys (the defaults are i, k, j and l for up, down, left and right)
and change the speed by pressing modifier keys. The keys f, d and s (by default) are used to simulate mouse clicks and
grabbing. With some other keys, you can scroll up, down, left and right and execute previously defined shell commands.

You probably want to define a key binding to start xmouseless.

## Configuration

The configuration is done in config.h, which is a C header file, but you don't need any programming knowledge to edit
it. After you edited the file, you have to run make again. 

## New version

Because of some minor issues with xmouseless which cannot be easily fixed the way it operates, a new version, called
[mouseless](https://github.com/jbensmann/mouseless), has been created. It operates on the level of Linux devices and has
the following advantages:
- not dependent on X11, e.g. works with Wayland too
- unless xmouseless, does not have problems with clicks in some menus
- possibility to toggle the mouse mode by holding down a key
- additional feature: remap keys, define arbitrary layers (similar to kmonad)

Nevertheless, you still might want to try xmouseless first to see if it fits your needs, since it is a little
easier to configure, and try mouseless later in case you want more features.

