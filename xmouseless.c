/*
 * xmouseless
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XTest.h>

#define LENGTH(X)   (sizeof X / sizeof X[0])


typedef struct {
    KeySym keysym;
    float x;
    float y;
} MoveBinding;

typedef struct {
    KeySym keysym;
    unsigned int button;
} ClickBinding;

typedef struct {
    KeySym keysym;
    unsigned int speed;
} SpeedBindings;

typedef struct {
    KeySym keysym;
    char *command;
} ShellBinding;

/* load configuration */
#include "config.h"


Display *dpy;
Window root;
pthread_t movethread;

static unsigned int speed = default_speed;

struct {
    float x;
    float y;
    float speed_x;
    float speed_y;
} mouseinfo;


void get_pointer();
void moverelative(float x, float y);
void click(unsigned int button, Bool is_press);
void handle_key(XKeyEvent event);
void init_x();
void close_x();


void get_pointer() {
    int x, y;
    int di;
    unsigned int dui;
    Window dummy;
    XQueryPointer(dpy, root, &dummy, &dummy, &x, &y, &di, &di, &dui);
    mouseinfo.x = x;
    mouseinfo.y = y;
}

void moverelative(float x, float y) {
    mouseinfo.x += x;
    mouseinfo.y += y;
    XWarpPointer(dpy, None, root, 0, 0, 0, 0,
            (int) mouseinfo.x, (int) mouseinfo.y);
    XFlush(dpy);
}

void click(unsigned int button, Bool is_press) {
    XTestFakeButtonEvent(dpy, button, is_press, 0);
    XFlush(dpy);
}

void init_x() {
    int i;
    int screen;

    /* initialize support for concurrent threads */
    XInitThreads();

    dpy = XOpenDisplay((char *) 0);
    screen = DefaultScreen(dpy);
    root = RootWindow(dpy, screen);

    /* turn auto key repeat off */
    XAutoRepeatOff(dpy);

    /* grab keys until success */
    for (i = 0; i < 100; i++) {
        if (XGrabKeyboard(dpy, root, False, GrabModeAsync,
                GrabModeAsync, CurrentTime) == GrabSuccess)
            return;
        usleep(10000);
    }

    printf("grab keyboard failed\n");
    close_x(EXIT_FAILURE);
}

void close_x(int exit_status) {
    /* turn auto repeat on again */
    XAutoRepeatOn(dpy);
    XUngrabKey(dpy, AnyKey, AnyModifier, root);
    XCloseDisplay(dpy);
    exit(exit_status);
}

void *moveforever(void *val) {
    /* this function is executed in a seperate thread */
    while (1) {
        if (mouseinfo.speed_x != 0 || mouseinfo.speed_y != 0) {
            moverelative((float) mouseinfo.speed_x * speed / move_rate,
                         (float) mouseinfo.speed_y * speed / move_rate);
        }
        usleep(1000000 / move_rate);
    }
}

void handle_key(XKeyEvent event) {
    unsigned int i;
    KeySym keysym;
    Bool is_press;

    keysym = XkbKeycodeToKeysym(dpy, event.keycode, 0, 0);
    is_press = (event.type == KeyPress);

    /* move bindings */
    for (i = 0; i < LENGTH(move_bindings); i++) {
        if (move_bindings[i].keysym == keysym) {
            int sign = is_press ? 1 : -1;
            mouseinfo.speed_x += sign * move_bindings[i].x;
            mouseinfo.speed_y += sign * move_bindings[i].y;
        }
    }

    /* click bindings */
    for (i = 0; i < LENGTH(click_bindings); i++) {
        if (click_bindings[i].keysym == keysym) {
            click(click_bindings[i].button, is_press);
            printf("click: %i %i\n", click_bindings[i].button, is_press);
        }
    }

    /* speed bindings */
    for (i = 0; i < LENGTH(speed_bindings); i++) {
        if (speed_bindings[i].keysym == keysym) {
            speed = is_press ? speed_bindings[i].speed : default_speed;
            printf("speed: %i\n", speed);
        }
    }

    /* shell and exit bindings only on key release */
    if (!is_press) {
        /* shell bindings */
        for (i = 0; i < LENGTH(shell_bindings); i++) {
            if (shell_bindings[i].keysym == keysym) {
                printf("executing: %s\n", shell_bindings[i].command);
                if (fork() == 0) {
                    system(shell_bindings[i].command);
                    exit(EXIT_SUCCESS);
                }
            }
        }

        /* exit */ 
        for (i = 0; i < LENGTH(exit_keys); i++) {
            if (exit_keys[i] == keysym) {
                close_x(EXIT_SUCCESS);
            }
        }
    }
}

int main () {
    int rc;
    XEvent event;

    init_x();

    get_pointer();
    mouseinfo.speed_x = 0;
    mouseinfo.speed_y = 0;

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
            case KeyRelease:
                get_pointer();
                handle_key(event.xkey);
                break;
        }
    }
}
