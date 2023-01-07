/* DWM
 *
 * dynamic window manager is designed like any other X client as well. It is
 * driven through handling X events. In contrast to other X clients, a window
 * manager selects for SubstructureRedirectMask on the root window, to receive
 * events about window (dis-)appearance. Only one X connection at a time is
 * allowed to select for this event mask.
 *
 * The event handlers of dwm are organized in an array which is accessed
 * whenever a new event has been fetched. This allows event dispatching
 * in O(1) time.
 *
 * Each child of the root window is called a client, except windows which have
 * set the override_redirect flag. Clients are organized in a linked client
 * list on each monitor, the focus history is remembered through a stack list
 * on each monitor. Each client contains a bit array to indicate the tags of a
 * client.
 *
 * To understand everything else, start reading main().
 */

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/XF86keysym.h>
#include <X11/Xft/Xft.h>

#include "drw.h"

// Macros {{{
#define BUTTONMASK (ButtonPressMask | ButtonReleaseMask)
#define CLEANMASK(mask)                                                   \
	(mask & ~(numlockmask | LockMask) &                                     \
	 (ShiftMask | ControlMask | Mod1Mask | Mod2Mask | Mod3Mask | Mod4Mask | \
		Mod5Mask))
#define HEIGHT(X)    ((X)->h + 2 * (X)->bw)
#define INTERSECT(x, y, w, h, m)                                   \
	(MAX(0, MIN((x) + (w), (m)->wx + (m)->ww) - MAX((x), (m)->wx)) * \
	 MAX(0, MIN((y) + (h), (m)->wy + (m)->wh) - MAX((y), (m)->wy)))
#define ISVISIBLE(C) ((C->tags & C->mon->tagset[C->mon->seltags]))
#define LENGTH(X)    (sizeof X / sizeof X[0])
#define MODKEY       Mod4Mask
#define MOUSEMASK    (BUTTONMASK | PointerMotionMask)
#define SHCMD(cmd)   { .v = (const char*[]) { "/bin/sh", "-c", cmd, NULL } }
#define TAGMASK      ((1 << LENGTH(tags)) - 1)
#define TAGKEYS(KEY, TAG)                                          \
	{ MODKEY, KEY, view, {.ui = 1 << TAG} },                        \
		{ MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG} }, \
		{ MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG} },          \
		{ MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG} },
#define TERMINAL     "kitty"
#define TEXTW(X)     (drw_fontset_getwidth(drw, (X)) + lrpad)
#define WIDTH(X)     ((X)->w + 2 * (X)->bw)
// }}}

// Enums, structs and unions {{{
// Enums {{{
enum { CurNormal, CurResize, CurMove, CurLast }; /* cursor */
enum { SchemeNorm, SchemeSel };                  /* color schemes */
enum {
	NetSupported,
	NetWMName,
	NetWMState,
	NetWMCheck,
	NetWMFullscreen,
	NetActiveWindow,
	NetWMWindowType,
	NetWMWindowTypeDialog,
	NetClientList,
	NetLast
}; /* EWMH atoms */
enum {
	WMProtocols,
	WMDelete,
	WMState,
	WMTakeFocus,
	WMLast
}; /* default atoms */
enum {
	ClkTagBar,
	ClkLtSymbol,
	ClkStatusText,
	ClkWinTitle,
	ClkClientWin,
	ClkRootWin,
	ClkLast
}; /* clicks */
// }}}
// Unions {{{
typedef union {
	int          i;
	unsigned int ui;
	float        f;
	const void*  v;
} Arg;
// }}}
// Structs {{{
typedef struct {
	unsigned int click;
	unsigned int mask;
	unsigned int button;
	void (*func)(const Arg* arg);
	const Arg arg;
} Button;

typedef struct Monitor Monitor;
typedef struct Client  Client;
struct Client {
	char         name[256];
	float        mina, maxa;
	int          x, y, w, h;
	int          oldx, oldy, oldw, oldh;
	int          basew, baseh, incw, inch, maxw, maxh, minw, minh, hintsvalid;
	int          bw, oldbw;
	unsigned int tags;
	int      isfixed, isfloating, isurgent, neverfocus, oldstate, isfullscreen;
	Client*  next;
	Client*  snext;
	Monitor* mon;
	Window   win;
};

typedef struct {
	unsigned int mod;
	KeySym       keysym;
	void (*func)(const Arg*);
	const Arg arg;
} Key;

typedef struct {
	const char* symbol;
	void (*arrange)(Monitor*);
} Layout;

struct Monitor {
	char          ltsymbol[16];
	float         mfact;
	int           nmaster;
	int           num;
	int           by;             /* bar geometry */
	int           mx, my, mw, mh; /* screen size */
	int           wx, wy, ww, wh; /* window area  */
	unsigned int  seltags;
	unsigned int  sellt;
	unsigned int  tagset[2];
	int           showbar;
	int           topbar;
	Client*       clients;
	Client*       sel;
	Client*       stack;
	Monitor*      next;
	Window        barwin;
	const Layout* lt[2];
};

typedef struct {
	const char* class;
	const char*  instance;
	const char*  title;
	unsigned int tags;
	int          isfloating;
	int          monitor;
} Rule;
// }}}
// }}}

// Functions {{{
static void applyrules(Client* c);
static int
applysizehints(Client* c, int* x, int* y, int* w, int* h, int interact);
static void     arrange(Monitor* m);
static void     arrangemon(Monitor* m);
static void     attach(Client* c);
static void     attachstack(Client* c);
static void     bstack(Monitor *m);
static void     buttonpress(XEvent* e);
static void     centeredmaster(Monitor *m);
static void     centeredfloatingmaster(Monitor *m);
static void     checkotherwm(void);
static void     cleanup(void);
static void     cleanupmon(Monitor* mon);
static void     clientmessage(XEvent* e);
static void     configure(Client* c);
static void     configurenotify(XEvent* e);
static void     configurerequest(XEvent* e);
static Monitor* createmon(void);
static void     cyclelayout(const Arg *arg);
static void     deck(Monitor *m);
static void     destroynotify(XEvent* e);
static void     detach(Client* c);
static void     detachstack(Client* c);
static Monitor* dirtomon(int dir);
static void     drawbar(Monitor* m);
static void     drawbars(void);
static void     enternotify(XEvent* e);
static void     expose(XEvent* e);
static void     focus(Client* c);
static void     focusin(XEvent* e);
static void     focusmon(const Arg* arg);
static void     focusstack(const Arg* arg);
static Atom     getatomprop(Client* c, Atom prop);
static int      getrootptr(int* x, int* y);
static long     getstate(Window w);
static int      gettextprop(Window w, Atom atom, char* text, unsigned int size);
static void     grabbuttons(Client* c, int focused);
static void     grabkeys(void);
static void     incnmaster(const Arg* arg);
static void     keypress(XEvent* e);
static void     killclient(const Arg* arg);
static void     manage(Window w, XWindowAttributes* wa);
static void     mappingnotify(XEvent* e);
static void     maprequest(XEvent* e);
static void     monocle(Monitor* m);
static void     motionnotify(XEvent* e);
static void     movemouse(const Arg* arg);
static Client*  nexttiled(Client* c);
static void     pop(Client* c);
static void     propertynotify(XEvent* e);
static void     quit(const Arg* arg);
static Monitor* recttomon(int x, int y, int w, int h);
static void     resize(Client* c, int x, int y, int w, int h, int interact);
static void     resizeclient(Client* c, int x, int y, int w, int h);
static void     resizemouse(const Arg* arg);
static void     restack(Monitor* m);
static void     run(void);
static void     scan(void);
static int      sendevent(Client* c, Atom proto);
static void     sendmon(Client* c, Monitor* m);
static void     setclientstate(Client* c, long state);
static void     setfocus(Client* c);
static void     setfullscreen(Client* c, int fullscreen);
static void     setlayout(const Arg* arg);
static void     setmfact(const Arg* arg);
static void     setup(void);
static void     seturgent(Client* c, int urg);
static void     showhide(Client* c);
static void     sigchld(int unused);
static void     spawn(const Arg* arg);
static void     tag(const Arg* arg);
static void     tagmon(const Arg* arg);
static void     tatami(Monitor *m);
static void     tile(Monitor* m);
static void     togglebar(const Arg* arg);
static void     togglefloating(const Arg* arg);
static void     toggletag(const Arg* arg);
static void     toggleview(const Arg* arg);
static void     unfocus(Client* c, int setfocus);
static void     unmanage(Client* c, int destroyed);
static void     unmapnotify(XEvent* e);
static void     updatebarpos(Monitor* m);
static void     updatebars(void);
static void     updateclientlist(void);
static int      updategeom(void);
static void     updatenumlockmask(void);
static void     updatesizehints(Client* c);
static void     updatestatus(void);
static void     updatetitle(Client* c);
static void     updatewindowtype(Client* c);
static void     updatewmhints(Client* c);
static void     view(const Arg* arg);
static Client*  wintoclient(Window w);
static Monitor* wintomon(Window w);
static int      xerror(Display* dpy, XErrorEvent* ee);
static int      xerrordummy(Display* dpy, XErrorEvent* ee);
static int      xerrorstart(Display* dpy, XErrorEvent* ee);
static void     zoom(const Arg* arg);
// }}}

// Variables {{{
static const char broken[] = "broken";
static char       stext[256];
static int        screen;
static int        sw, sh; /* X display screen geometry width, height */
static int        bh;     /* bar height */
static int        lrpad;  /* sum of left and right padding for text */
static int (*xerrorxlib)(Display*, XErrorEvent*);
static unsigned int numlockmask            = 0;
static void (*handler[LASTEvent])(XEvent*) = {
	[ButtonPress]      = buttonpress,
	[ClientMessage]    = clientmessage,
	[ConfigureRequest] = configurerequest,
	[ConfigureNotify]  = configurenotify,
	[DestroyNotify]    = destroynotify,
	[EnterNotify]      = enternotify,
	[Expose]           = expose,
	[FocusIn]          = focusin,
	[KeyPress]         = keypress,
	[MappingNotify]    = mappingnotify,
	[MapRequest]       = maprequest,
	[MotionNotify]     = motionnotify,
	[PropertyNotify]   = propertynotify,
	[UnmapNotify]      = unmapnotify
};
static Atom     wmatom[WMLast], netatom[NetLast];
static int      running = 1;
static Cur*     cursor[CurLast];
static Clr**    scheme;
static Display* dpy;
static Drw*     drw;
static Monitor *mons, *selmon;
static Window   root, wmcheckwin;
// }}}

// Configuration {{{
// Appearance {{{
static const unsigned int borderpx = 1;  /* border pixel of windows */
static const unsigned int snap     = 22; /* snap pixel */
static const int          showbar  = 1;  /* 0 means no bar */
static const int          topbar   = 1;  /* 0 means bottom bar */
static const char*        fonts[]  = {
	"Operator Mono Lig Book:size=9.0",
	"FiraCode Nerd Font Book:size=8.0",
};
static const char  std_fg[]  = "#dcd7ba";
static const char  std_bg[]  = "#1f1f28";
static const char  std_bd[]  = "#1f1f28";
static const char  self_fg[] = "#c8c093";
static const char  self_bg[] = "#2d4f67";
static const char  self_bd[] = "#72a7bc";
static const char* colors[][3] = {
	//               fg         bg     border
	[SchemeNorm] = { std_fg,  std_bg,  std_bd },
	[SchemeSel]  = { self_fg, self_bg, self_bd },
};
// }}}
// Tags and Rules {{{
static const char *tags[] = { "α", "β", "ξ", "δ", "ε", "φ", "γ", "θ", "ι" };
/* compile-time check if all tags fit into an unsigned int bit array. */
struct NumTags { char limitexceeded[LENGTH(tags) > 31 ? -1 : 1]; };

static const Rule rules[] = {
	/* xprop(1):
	 * WM_CLASS(STRING) = instance, class
	 * WM_NAME(STRING) = title
	 */
	// class                    instance        title       tags mask  isfloating   monitor
	{ NULL,                       NULL,     "Event Tester",     0,       1,         -1 },
	{ NULL,                       NULL,     "lstopo",           0,       1,         -1 },
	{ NULL,                       NULL,     "weatherreport",    0,       1,         -1 },
	{ NULL,                       "pop-up", NULL,               0,       1,         -1 },
	{ "Arandr",                   NULL,     NULL,               0,       1,         -1 },
	{ "Avahi-discover",           NULL,     NULL,               0,       1,         -1 },
	{ "Blueberry.py",             NULL,     NULL,               0,       1,         -1 },
	{ "Bssh",                     NULL,     NULL,               0,       1,         -1 },
	{ "Bvnc",                     NULL,     NULL,               0,       1,         -1 },
	{ "CMakeSetup",               NULL,     NULL,               0,       1,         -1 },
	{ "Display",                  NULL,     "ImageMagick: ",    0,       1,         -1 },
	{ "feh",                      NULL,     NULL,               0,       1,         -1 },
	{ "Hardinfo",                 NULL,     NULL,               0,       1,         -1 },
	{ "Lxappearance",             NULL,     NULL,               0,       1,         -1 },
	{ "matplotlib",               NULL,     NULL,               0,       1,         -1 },
	{ "Nibbler",                  NULL,     NULL,               0,       1,         -1 },
	{ "Parcellite",               NULL,     NULL,               0,       1,         -1 },
	{ "Pavucontrol",              NULL,     NULL,               0,       1,         -1 },
	{ "qv4l2",                    NULL,     NULL,               0,       1,         -1 },
	{ "qvidcap",                  NULL,     NULL,               0,       1,         -1 },
	{ "System-config-printer.py", NULL,     NULL,               0,       1,         -1 },
	{ "Sxiv",                     NULL,     NULL,               0,       1,         -1 },
	{ "Transmission-gtk",         NULL,     NULL,               1 << 8,  1,         -1 },
	{ "Xboard",                   NULL,     NULL,               0,       1,         -1 },
	{ "Xmessage",                 NULL,     NULL,               0,       1,         -1 },
	{ "Yad",                      NULL,     NULL,               0,       1,         -1 },
	{ "Yad-icon-browser",         NULL,     NULL,               0,       1,         -1 },
};
// }}}
// Layout(s) {{{
static const float  mfact          = 0.50; /* factor of master area size [0.05..0.95] */
static const int    nmaster        = 1;    /* number of clients in master area */
static const int    resizehints    = 0;    /* 1 means respect size hints in tiled resizals */
static const int    lockfullscreen = 0;    /* 1 will force focus on the fullscreen window */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=", tile },
	{ "TTT", bstack },
	{ "[D]", deck },
	{ "|+|", tatami },
	{ "[M]", monocle },
	{ "|M|", centeredmaster },
	{ ">M>", centeredfloatingmaster },
	{ ">>=", NULL },
	{ NULL,  NULL },
};
// }}}
// Kbd {{{
static const Key keys[] = {
	/* modifier                     key        function        argument */
	// Left Side {{{
	{ MODKEY|ShiftMask,   XK_q, killclient,     { 0 } },
	{ MODKEY,             XK_w, focusmon,       { .i = -1 } },
	{ MODKEY,             XK_e, focusmon,       { .i = +1 } },
	{ MODKEY|ShiftMask,   XK_w, tagmon,         { .i = -1 } },
	{ MODKEY|ShiftMask,   XK_e, tagmon,         { .i = +1 } },
	{ MODKEY,             XK_r, view,           { 0 } },
	{ MODKEY|ShiftMask,   XK_r, setlayout,      { 0 } },
	{ MODKEY,             XK_t, togglefloating, { 0 } },
	{ MODKEY,             XK_a, spawn,          SHCMD(TERMINAL) },
	{ MODKEY|ShiftMask,   XK_a, spawn,          SHCMD("st") },
	{ MODKEY|ControlMask, XK_a, spawn,          SHCMD("tabbed -c -r 2 st -w ''") },
	{ MODKEY,             XK_d, spawn,          SHCMD("tabbed -c zathura -e") },
	{ MODKEY|ShiftMask,   XK_d, spawn,          SHCMD("evince") },
	{ MODKEY,             XK_f, spawn,          SHCMD("google-chrome-stable") },
	{ MODKEY|ShiftMask,   XK_f, spawn,          SHCMD("vieb") },
	{ MODKEY,             XK_b, togglebar,      { 0 } },
	// }}}
	// Right Side {{{
	{ MODKEY,           XK_u, spawn,      SHCMD("dmenu_run") },
	{ MODKEY|ShiftMask, XK_u, spawn,      SHCMD("rofi -modi drun,run,combi -show combi") },
	{ MODKEY,           XK_o, spawn,      SHCMD("code") },
	{ MODKEY,           XK_k, focusstack, { .i = -1 } },
	{ MODKEY,           XK_j, focusstack, { .i = +1 } },
	{ MODKEY,           XK_h, setmfact,   { .f = -0.05 } },
	{ MODKEY,           XK_l, setmfact,   { .f = +0.05 } },
	{ MODKEY|ShiftMask, XK_m, zoom,       { 0 } },
	// }}}
	// Surrounding Keys {{{
	{ MODKEY,             XK_Tab,       cyclelayout, { .i = +1 } },
	{ MODKEY|ShiftMask,   XK_Tab,       cyclelayout, { .i = -1 } },
	{ MODKEY,             XK_Delete,    spawn,       SHCMD("xmenu-shutdown") },
	{ MODKEY|ShiftMask,   XK_Delete,    quit,        { 0 } },
	{ MODKEY|ShiftMask,   XK_BackSpace, spawn,       SHCMD("loginctl lock-session") },
	{ MODKEY,             XK_Return,    spawn,       SHCMD(TERMINAL) },
	{ MODKEY|ShiftMask,   XK_Return,    spawn,       SHCMD("st") },
	{ MODKEY|ControlMask, XK_Return,    spawn,       SHCMD("tabbed -c -r 2 st -w ''") },
	{ MODKEY,             XK_period,    incnmaster,  { .i = -1 } },
	{ MODKEY,             XK_comma,     incnmaster,  { .i = +1 } },
	{ MODKEY,             XK_space,     setlayout,   { .v = &layouts[0] } },
	{ MODKEY|ShiftMask,   XK_space,     setlayout,   { .v = &layouts[1] } },
	{ MODKEY|ControlMask, XK_space,     setlayout,   { .v = &layouts[2] } },
	{ MODKEY,             XK_Print,     spawn,       SHCMD("scrotwp -fd") },
	{ MODKEY|ShiftMask,   XK_Print,     spawn,       SHCMD("scrotwp -sd") },
	{ MODKEY|ControlMask, XK_Print,     spawn,       SHCMD("scrotwp -wd") },
	// }}}
	// Arrows {{{
	// }}}
	// Numbers {{{
	TAGKEYS(XK_1, 0)
	TAGKEYS(XK_2, 1)
	TAGKEYS(XK_3, 2)
	TAGKEYS(XK_4, 3)
	TAGKEYS(XK_5, 4)
	TAGKEYS(XK_6, 5)
	TAGKEYS(XK_7, 6)
	TAGKEYS(XK_8, 7)
	TAGKEYS(XK_9, 8)
	{ MODKEY,           XK_0, view, {.ui = ~0 } },
	{ MODKEY|ShiftMask, XK_0, tag,  {.ui = ~0 } },
	// }}}
	// Fn and Extra Keys {{{
	{ 0, XF86XK_AudioMute,         spawn,  SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle") },
	{ 0, XF86XK_AudioLowerVolume,  spawn,  SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%") },
	{ 0, XF86XK_AudioRaiseVolume,  spawn,  SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%") },
	{ 0, XF86XK_MonBrightnessDown, spawn,  SHCMD("xbacklight -dec 5") },
	{ 0, XF86XK_MonBrightnessUp,   spawn,  SHCMD("xbacklight -inc 5") },
	{ 0, XF86XK_Display,           spawn,  SHCMD("xrander") },
	{ 0, XF86XK_Search,            spawn,  SHCMD(TERMINAL " -e nnn") },
	{ 0, XF86XK_Explorer,          spawn,  SHCMD("google-chrome-stable") },
	{ 0, XF86XK_Calculator,        spawn,  SHCMD(TERMINAL " -e ghci") },
	// }}}
};
// }}}
// Mouse {{{
// Click can be:
//   ClkTagBar, ClkLtSymbol, ClkStatusText,
//   ClkWinTitle, ClkClientWin, ClkRootWin
static const Button buttons[] = {
	/* click     event mask button   function        argument */
	{ ClkLtSymbol,   0,      Button1, setlayout,      { 0 } },
	{ ClkLtSymbol,   0,      Button2, setlayout,      { .v = &layouts[1] } },
	{ ClkLtSymbol,   0,      Button4, cyclelayout,    { .i = +1 } },
	{ ClkLtSymbol,   0,      Button5, cyclelayout,    { .i = -1 } },
	{ ClkWinTitle,   0,      Button2, zoom,           { 0 } },
	{ ClkStatusText, 0,      Button1, spawn,          SHCMD(TERMINAL) },
	{ ClkStatusText, 0,      Button2, spawn,          SHCMD(TERMINAL " -e pulsemixer") },
	{ ClkStatusText, 0,      Button3, spawn,          SHCMD(TERMINAL " -e htop") },
	{ ClkStatusText, 0,      Button4, spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%") },
	{ ClkStatusText, 0,      Button5, spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%") },
	{ ClkClientWin,  MODKEY, Button1, movemouse,      { 0 } },
	{ ClkClientWin,  MODKEY, Button2, togglefloating, { 0 } },
	{ ClkClientWin,  MODKEY, Button3, resizemouse,    { 0 } },
	{ ClkTagBar,     0,      Button1, view,           { 0 } },
	{ ClkTagBar,     0,      Button3, toggleview,     { 0 } },
	{ ClkTagBar,     MODKEY, Button1, tag,            { 0 } },
	{ ClkTagBar,     MODKEY, Button3, toggletag,      { 0 } },
};
// }}}
// }}}

// vim:fdm=marker
