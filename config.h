
/* the rate at which the mouse moves */
static const unsigned int move_rate = 30;

/* the speed with no modifier */
static const unsigned int default_speed = 20;

static SpeedBindings speed_bindings[] = {
    /* key             speed */  
    { XK_Super_L,      100 },
    { XK_Alt_L,        50  },
    { XK_a,            2   },
};

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

/* 1: left
 * 2: middle
 * 3: right
 * 4: scroll up
 * 5: scroll down */
static ClickBinding click_bindings[] = {
    /* key         button */  
    { XK_space,    1 },
    { XK_f,        1 },
    { XK_d,        2 },
    { XK_s,        3 },
    { XK_plus,     4 },
    { XK_minus,    5 },
};

static ShellBinding shell_bindings[] = {
    /* key         command */  
    { XK_b,        "wmctrl -a firefox" },
    { XK_t,        "echo $0 >> ~/test" },
};

/* exits on key release which allows click and exit with one key */
static unsigned int exit_keys[] = {
    XK_Escape, XK_q, XK_space
};
