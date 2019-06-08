#include <X11/Xlib.h>
#include <X11/Xutil.h>

/* the rate at which the mouse moves */
#define MOVE_RATE 30

/* the speed with no modifier */
#define DEFAULT_SPEED 10

#define EXIT_KEY XK_q

typedef struct {
    KeySym keysym;
    int x;
    int y;
} MoveBinding;

typedef struct {
    KeySym keysym;
    int button;
} ClickBinding;

typedef struct {
    KeySym keysym;
    int speed;
} SpeedBindings;

static MoveBinding move_bindings[] = {
    /* key         x      y */
    { XK_j,        -1,     0 },
    { XK_l,         1,     0 },
    { XK_i,         0,    -1 },
    { XK_comma,     0,     1 },
    { XK_u,        -1,    -1 },
    { XK_o,         1,    -1 },
    { XK_m,        -1,     1 },
    { XK_period,    1,     1 },
};

static ClickBinding click_bindings[] = {
    /* key         button */  
    { XK_f,        1 },
    { XK_d,        2 },
    { XK_s,        3 },
};

static SpeedBindings speed_bindings[] = {
    /* key             speed */  
    { XK_Super_L,      200 },
    { XK_Alt_L,        50  },
    { XK_a,            2   },
};
