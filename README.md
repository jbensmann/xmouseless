# xmouseless

This program is a replacement of the physical mouse for X11.

Features:
- move the mouse with different speeds
- click and grab
- execute shell commands

It aims to be simple and efficient.

## Installation

```
make
sudo make install
```

## Configuration

Edit config.h and reinstall.

## Usage

When starting xmouseless, it grabs the keyboard and all defined bindings are
available. When pressing an exit key, the program exits.

You probably want to define a key binding for xmouseless in your desktop
environment or window manager.
