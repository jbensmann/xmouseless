/*
        gcc xmouseless.c -o xmouseless -lX11 -lpthread -lXtst
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/extensions/XTest.h>


int MOVE_RATE = 30;
int SPEED = 10;

Display *dpy;
int screen;
Window root;
pthread_t movethread;

struct {
    int x;
    int y;
    int movex;
    int movey;
} mouseinfo;

void init_x();
void close_x();
void redraw();
int getrootptr(int *x, int *y);

void moverelative(int x, int y) {
    mouseinfo.x += x;
    mouseinfo.y += y;
    XWarpPointer(dpy, None, root, 0, 0, 0, 0, mouseinfo.x, mouseinfo.y);
    XFlush(dpy);
}

void init_x() {
    dpy=XOpenDisplay((char *)0);
    screen=DefaultScreen(dpy);
    root = RootWindow(dpy, screen);

    XGrabKeyboard(dpy, root, False, GrabModeAsync, GrabModeAsync, CurrentTime);

    /* XGrabKey(dpy, XKeysymToKeycode(dpy, XK_Q), 0, root, False, GrabModeAsync, GrabModeAsync); */
    /* XGrabKey(dpy, XKeysymToKeycode(dpy, XK_Alt_L), AnyModifier, root, False, GrabModeAsync, GrabModeAsync); */

    XAutoRepeatOff(dpy);
}

void close_x() {
    XAutoRepeatOn(dpy);
    XCloseDisplay(dpy);
}

int getrootptr(int *x, int *y) {
    int di;
    unsigned int dui;
    Window dummy;

    return XQueryPointer(dpy, root, &dummy, &dummy, x, y, &di, &di, &dui);
}

void click(int button, int is_press) {
    XTestFakeButtonEvent(dpy, button, is_press, 0);
    XFlush(dpy);
}

void *moveforever(void *val) {
    while (1) {
        if (mouseinfo.movex != 0 || mouseinfo.movey != 0) {
            moverelative(SPEED * mouseinfo.movex, SPEED * mouseinfo.movey);
            /* printf("%i, %i\n", mouseinfo.x, mouseinfo.y); */
        }
        usleep(1000000 / MOVE_RATE);
    }
}

int main () {
    int rc;
    XEvent event;

    init_x();

    getrootptr(&mouseinfo.x, &mouseinfo.y);
    mouseinfo.movex = 0;
    mouseinfo.movey = 0;

    // start the thread for mouse movement
    rc = pthread_create(&movethread, NULL, &moveforever, NULL);
    if( rc != 0 ) {
        printf("Unable to start thread.\n");
        return EXIT_FAILURE;
    }

    while(1) {
        XNextEvent(dpy, &event);

        switch (event.type) {
            case KeyPress:
                getrootptr(&mouseinfo.x, &mouseinfo.y);

                printf("You pressed the %i key at %i, %i\n",
                        event.xkey.keycode, event.xkey.x, event.xkey.y);

                if (event.xkey.keycode == 24 || event.xkey.keycode == 0x09) {
                    close_x();
                    return 0;
                }
                else if (event.xkey.keycode == 45) {
                    mouseinfo.movex = 0;
                    mouseinfo.movey = 0;
                }
                else if (event.xkey.keycode == 44) {
                    mouseinfo.movex -= 1;
                }
                else if (event.xkey.keycode == 46) {
                    mouseinfo.movex += 1;
                }
                else if (event.xkey.keycode == 31) {
                    mouseinfo.movey -= 1;
                }
                else if (event.xkey.keycode == 59) {
                    mouseinfo.movey += 1;
                }
                else if (event.xkey.keycode == 64) {
                    SPEED = 40;
                }
                else if (event.xkey.keycode == 41) {
                    click(1, True);
                }
                else if (event.xkey.keycode == 39) {
                    click(3, True);
                }
                break;
            case KeyRelease:
                printf("You released the %i key at %i, %i\n",
                        event.xkey.keycode, event.xkey.x, event.xkey.y);

                if (event.xkey.keycode == 44) {
                    mouseinfo.movex += 1;
                }
                else if (event.xkey.keycode == 46) {
                    mouseinfo.movex -= 1;
                }
                else if (event.xkey.keycode == 31) {
                    mouseinfo.movey += 1;
                }
                else if (event.xkey.keycode == 59) {
                    mouseinfo.movey -= 1;
                }
                else if (event.xkey.keycode == 64) {
                    SPEED = 10;
                }
                else if (event.xkey.keycode == 41) {
                    click(1, False);
                }
                else if (event.xkey.keycode == 39) {
                    click(3, False);
                }
                break;
        }
    }
}
