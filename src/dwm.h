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
#ifndef DWM_H
#define DWM_H

#include <X11/XF86keysym.h>
#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdint.h>

#include "drw.h"

// Macros {{{
#define BUTTONMASK (ButtonPressMask | ButtonReleaseMask)
#define CLEANMASK(mask) \
	(mask & ~(numlockmask | LockMask) & \
	 (ShiftMask | ControlMask | Mod1Mask | Mod2Mask | Mod3Mask | Mod4Mask | \
		Mod5Mask))
#define HEIGHT(X) ((X)->h + 2 * (X)->bw)
#define INTERSECT(x, y, w, h, m) \
	(MAX(0, MIN((x) + (w), (m)->wx + (m)->ww) - MAX((x), (m)->wx)) * \
	 MAX(0, MIN((y) + (h), (m)->wy + (m)->wh) - MAX((y), (m)->wy)))
#define ICONSIZE    16
#define ICONSPACING 5
#define ISVISIBLE(C) ((C->tags & C->mon->tagset[C->mon->seltags]))
#define LENGTH(X) (sizeof X / sizeof X[0])
#define MAXTABS 50
#define MODKEY Mod4Mask
#define MOUSEMASK (BUTTONMASK | PointerMotionMask)
#define SHCMD(cmd) { .v = (const char*[]) { "/bin/sh", "-c", cmd, NULL } }
#define SYSTEM_TRAY_REQUEST_DOCK 0
#define TAGMASK ((1 << LENGTH(tags)) - 1)
#define TAGKEYS(KEY, TAG)                                          \
	{ MODKEY, KEY, view, {.ui = 1 << TAG} },                        \
		{ MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG} }, \
		{ MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG} },          \
		{ MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG} },
#define TERMINAL     "kitty"
#define TEXTW(X)     (drw_fontset_getwidth(drw, (X)) + lrpad)
#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define WIDTH(X)     ((X)->w + 2 * (X)->bw)
#define XEMBED_EMBEDDED_NOTIFY      0
#define XEMBED_EMBEDDED_VERSION     (VERSION_MAJOR << 16) | VERSION_MINOR
#define XEMBED_FOCUS_IN             4
#define XEMBED_MAPPED              (1 << 0)
#define XEMBED_MODALITY_ON         10
#define XEMBED_WINDOW_ACTIVATE      1
#define XEMBED_WINDOW_ACTIVATE      1
#define XEMBED_WINDOW_DEACTIVATE    2
// }}}

// Enums, structs and unions {{{
// Enums {{{
enum { CurNormal, CurResize, CurMove, CurLast }; /* cursor */
enum { SchemeNorm, SchemeSel };                  /* color schemes */
enum {
	NetSupported,
	NetWMName,
	NetWMIcon,
	NetWMState,
	NetWMCheck,
	NetSystemTray,
	NetSystemTrayOP,
	NetSystemTrayOrientation,
	NetSystemTrayOrientationHorz,
	NetWMFullscreen,
	NetActiveWindow,
	NetWMWindowType,
	NetWMWindowTypeDialog,
	NetClientList,
	NetLast
}; /* EWMH atoms */
enum {
	Manager,
	Xembed,
	XembedInfo,
	XLast
}; /* Xembed atoms */
enum {
	WMProtocols,
	WMDelete,
	WMState,
	WMTakeFocus,
	WMLast
}; /* default atoms */
enum {
	ClkTagBar,
	ClkTabBar,
	ClkLtSymbol,
	ClkStatusText,
	ClkButton,
	ClkWinTitle,
	ClkClientWin,
	ClkRootWin,
	ClkLast
}; /* clicks */
enum showtab_modes {
	showtab_never,
	showtab_auto,
	showtab_nmodes,
	showtab_always
}; // Tab bar display modes
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
	int          sfx, sfy, sfw, sfh;
	int          oldx, oldy, oldw, oldh;
	int          basew, baseh, incw, inch, maxw, maxh, minw, minh, hintsvalid;
	int          bw, oldbw;
	unsigned int tags;
	int          isfixed, isfloating, isurgent, neverfocus, oldstate, isfullscreen;
	int          floatborderpx;
	int          hasfloatbw;
	unsigned int icw, ich; Picture icon;
	char         scratchkey;
	Client*      next;
	Client*      snext;
	Monitor*     mon;
	Window       win;
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
	int           ty;             /* tab bar geometry */
	int           mx, my, mw, mh; /* screen size */
	int           wx, wy, ww, wh; /* window area  */
	unsigned int  seltags;
	unsigned int  sellt;
	unsigned int  tagset[2];
	int           showbar;
	int           showtab;
	int           topbar;
	int           toptab;
	Client*       clients;
	Client*       sel;
	Client*       stack;
	Client*       tagmarked[32];
	Monitor*      next;
	Window        barwin;
	Window        tabwin;
	int           ntabs;
	int           tab_widths[MAXTABS];
	const Layout* lt[2];
};

typedef struct {
	const char* class;
	const char*  instance;
	const char*  title;
	unsigned int tags;
	int          isfloating;
	int          monitor;
	int          floatx, floaty, floatw, floath;
	int          floatborderpx;
	const char   scratchkey;
} Rule;

typedef struct Systray Systray;
struct Systray {
	Window win;
	Client *icons;
};
// }}}
// }}}

// Functions {{{
static void         applyrules(Client* c);
static int          applysizehints(Client* c, int* x, int* y, int* w, int* h, int interact);
static void         arrange(Monitor* m);
static void         arrangemon(Monitor* m);
static void         attach(Client* c);
static void         attachabove(Client *c);
static void         attachstack(Client* c);
static void         bstack(Monitor *m);
static void         buttonpress(XEvent* e);
static void         centeredmaster(Monitor *m);
static void         centeredfloatingmaster(Monitor *m);
static void         checkotherwm(void);
static void         cleanup(void);
static void         cleanupmon(Monitor* mon);
static void         clientmessage(XEvent* e);
static int          cmpint(const void *p1, const void *p2);
static void         configure(Client* c);
static void         configurenotify(XEvent* e);
static void         configurerequest(XEvent* e);
static Monitor*     createmon(void);
static void         cyclelayout(const Arg *arg);
static void         deck(Monitor *m);
static void         destroynotify(XEvent* e);
static void         detach(Client* c);
static void         detachstack(Client* c);
static Monitor*     dirtomon(int dir);
static void         drawbar(Monitor* m);
static void         drawbars(void);
static void         drawtab(Monitor *m);
static void         drawtabs(void);
static int          drawstatusbar(Monitor *m, int bh, char* text);
static void         expose(XEvent* e);
static void         focus(Client* c);
static void         focusin(XEvent* e);
static void         focusmaster(const Arg *arg);
static void         focusmon(const Arg* arg);
static void         focusstack(const Arg* arg);
static void         focuswin(const Arg* arg);
static void         freeicon(Client *c);
static Atom         getatomprop(Client* c, Atom prop);
static Picture      geticonprop(Window w, unsigned int *icw, unsigned int *ich);
static int          getrootptr(int* x, int* y);
static long         getstate(Window w);
static unsigned int getsystraywidth();
static int          gettextprop(Window w, Atom atom, char* text, unsigned int size);
static void         grabbuttons(Client* c, int focused);
static void         grabkeys(void);
static void         incnmaster(const Arg* arg);
static void         keypress(XEvent* e);
static void         killclient(const Arg* arg);
static void         layoutmenu(const Arg *arg);
static void         manage(Window w, XWindowAttributes* wa);
static void         mappingnotify(XEvent* e);
static void         maprequest(XEvent* e);
static void         monocle(Monitor* m);
static void         movemouse(const Arg* arg);
static void         movestack(const Arg* arg);
static Client*      nexttiled(Client* c);
static void         pop(Client* c);
static uint32_t     prealpha(uint32_t p);
static void         propertynotify(XEvent* e);
static void         quit(const Arg* arg);
static Monitor*     recttomon(int x, int y, int w, int h);
static void         removesystrayicon(Client *i);
static void         resize(Client* c, int x, int y, int w, int h, int interact);
static void         resizebarwin(Monitor *m);
static void         resizeclient(Client* c, int x, int y, int w, int h);
static void         resizemouse(const Arg* arg);
static void         resizerequest(XEvent *e);
static void         restack(Monitor* m);
static void         run(void);
static void         scan(void);
static int          sendevent(Window w, Atom proto, int m, long d0, long d1, long d2, long d3, long d4);
static void         sendmon(Client* c, Monitor* m);
static void         setclientstate(Client* c, long state);
static void         setfocus(Client* c);
static void         setfullscreen(Client* c, int fullscreen);
static void         setlayout(const Arg* arg);
static void         setmfact(const Arg* arg);
static void         setup(void);
static void         seturgent(Client* c, int urg);
static void         showhide(Client* c);
static void         sigchld(int unused);
static void         spawn(const Arg* arg);
static void         spawnscratch(const Arg *arg);
static Monitor*     systraytomon(Monitor *m);
static void         tabmode(const Arg *arg);
static void         tag(const Arg* arg);
static void         tagmon(const Arg* arg);
static void         tatami(Monitor *m);
static void         tile(Monitor* m);
static void         togglebar(const Arg* arg);
static void         togglefloating(const Arg* arg);
static void         togglescratch(const Arg *arg);
static void         toggletag(const Arg* arg);
static void         toggleview(const Arg* arg);
static void         unfocus(Client* c, int setfocus);
static void         unmanage(Client* c, int destroyed);
static void         unmapnotify(XEvent* e);
static void         updatebarpos(Monitor* m);
static void         updatebars(void);
static void         updateclientlist(void);
static int          updategeom(void);
static void         updateicon(Client *c);
static void         updatenumlockmask(void);
static void         updatesizehints(Client* c);
static void         updatestatus(void);
static void         updatesystray(void);
static void         updatesystrayicongeom(Client *i, int w, int h);
static void         updatesystrayiconstate(Client *i, XPropertyEvent *ev);
static void         updatetitle(Client* c);
static void         updatewindowtype(Client* c);
static void         updatewmhints(Client* c);
static void         view(const Arg* arg);
static Client*      wintoclient(Window w);
static Monitor*     wintomon(Window w);
static Client*      wintosystrayicon(Window w);
static int          xerror(Display* dpy, XErrorEvent* ee);
static int          xerrordummy(Display* dpy, XErrorEvent* ee);
static int          xerrorstart(Display* dpy, XErrorEvent* ee);
static void         zoom(const Arg* arg);
// }}}

// Variables {{{
static const char broken[] = "broken";
static char       stext[1024];
static int        screen;
static int        sw, sh; /* X display screen geometry width, height */
static int        bh;     /* bar height */
static int        th = 0; /* tab bar geometry */
static int        lrpad;  /* sum of left and right padding for text */
static int (*xerrorxlib)(Display*, XErrorEvent*);
static unsigned int numlockmask            = 0;
static void (*handler[LASTEvent])(XEvent*) = {
	[ButtonPress]      = buttonpress,
	[ClientMessage]    = clientmessage,
	[ConfigureRequest] = configurerequest,
	[ConfigureNotify]  = configurenotify,
	[DestroyNotify]    = destroynotify,
	[Expose]           = expose,
	[FocusIn]          = focusin,
	[KeyPress]         = keypress,
	[MappingNotify]    = mappingnotify,
	[MapRequest]       = maprequest,
	[PropertyNotify]   = propertynotify,
	[ResizeRequest] = resizerequest,
	[UnmapNotify]      = unmapnotify
};
static Atom     wmatom[WMLast], netatom[NetLast], xatom[XLast];
static int      running = 1;
static Cur*     cursor[CurLast];
static Clr**    scheme;
static Display* dpy;
static Drw*     drw;
static Monitor *mons, *selmon;
static Window   root, wmcheckwin;
static Systray* systray = NULL;
// }}}

// Configuration {{{
// Appearance {{{
static const unsigned int borderpx = 2;
static const unsigned int snap = 22;
static const int showbar = 1;
static const int topbar = 1;
// 0: sloppy systray follows selected monitor, >0: pin systray to monitor X
static const unsigned int systraypinning = 0;
static const unsigned int systrayonleft = 0;
static const unsigned int systrayspacing = 1;
// 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor
static const int systraypinningfailfirst = 1;
static const int showsystray = 1;
// Fonts
static const char* fonts[]  = {
	"Operator Mono Lig Book:size=9.0",
	"FiraCode Nerd Font Book:size=8.0",
};
// Colors
static const char  std_fg[]  = "#fdf6e2";
static const char  std_bg[]  = "#073642";
static const char  std_bd[]  = "#002b36";
static const char  self_fg[] = "#eee8d5";
static const char  self_bg[] = "#657b83";
static const char  self_bd[] = "#268bd2";
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
	// class | instance | title | tags mask | isfloat | monitor | x | y | w | h | floatborderpx | scratchkey
	{ NULL,                       NULL,     "Event Tester",  0,      1, -1,  -1,   -1,  -1,  -1, -1, 0 },
	{ NULL,                       NULL,     "lstopo",        0,      1, -1,  -1,   -1,  -1,  -1, -1, 0 },
	{ NULL,                       NULL,     "weatherreport", 0,      1, -1, 460,  165, 925, 700, -1, 0 },
	{ NULL,                       "pop-up", NULL,            0,      1, -1,  -1,   -1,  -1,  -1, -1, 0 },
	{ "Arandr",                   NULL,     NULL,            0,      1, -1, 700,  340, 500, 400, -1, 0 },
	{ "Avahi-discover",           NULL,     NULL,            0,      1, -1,  -1,   -1,  -1,  -1, -1, 0 },
	{ "Blueberry.py",             NULL,     NULL,            0,      1, -1, 661,  308, 613, 445, -1, 0 },
	{ "Bssh",                     NULL,     NULL,            0,      1, -1,  -1,   -1,  -1,  -1, -1, 0 },
	{ "Bvnc",                     NULL,     NULL,            0,      1, -1,  -1,   -1,  -1,  -1, -1, 0 },
	{ "CMakeSetup",               NULL,     NULL,            0,      1, -1,  -1,   -1,  -1,  -1, -1, 0 },
	{ "Display",                  NULL,     "ImageMagick: ", 0,      1, -1, 610,  320, 640, 480,  0, 0 },
	{ "feh",                      NULL,     NULL,            0,      1, -1,  -1,   -1,  -1,  -1, -1, 0 },
	{ "Hardinfo",                 NULL,     NULL,            0,      1, -1,  -1,   -1,  -1,  -1, -1, 0 },
	{ "Lxappearance",             NULL,     NULL,            0,      1, -1,  -1,   -1,  -1,  -1, -1, 0 },
	{ "matplotlib",               NULL,     NULL,            0,      1, -1, 610,  320, 640, 480,  0, 0 },
	{ "Nibbler",                  NULL,     NULL,            0,      1, -1, 1850, 220, 745, 640, -1, 0 },
	{ "Parcellite",               NULL,     NULL,            0,      1, -1,  -1,   -1,  -1,  -1, -1, 0 },
	{ "Pavucontrol",              NULL,     NULL,            0,      1, -1, 700,  340, 500, 400, -1, 0 },
	{ "qv4l2",                    NULL,     NULL,            0,      1, -1,  -1,   -1,  -1,  -1, -1, 0 },
	{ "qvidcap",                  NULL,     NULL,            0,      1, -1,  -1,   -1,  -1,  -1, -1, 0 },
	{ "System-config-printer.py", NULL,     NULL,            0,      1, -1,  -1,   -1,  -1,  -1, -1, 0 },
	{ "Sxiv",                     NULL,     NULL,            0,      1, -1, 570,  265, 800, 600, -1, 0 },
	{ "Transmission-gtk",         NULL,     NULL,            1 << 8, 1, -1, 660,  210, 600, 500,  0, 0 },
	{ "Xboard",                   NULL,     NULL,            0,      1, -1,  -1,   -1,  -1,  -1, -1, 0 },
	{ "Xmessage",                 NULL,     NULL,            0,      1, -1,  -1,   -1,  -1,  -1, -1, 0 },
	{ "Yad",                      NULL,     NULL,            0,      1, -1,  -1,   -1,  -1,  -1, -1, 0 },
	{ "Yad-icon-browser",         NULL,     NULL,            0,      1, -1,  -1,   -1,  -1,  -1, -1, 0 },
	// Named scratchpads
	{ NULL,                       NULL,     "yakuake",       0,      1, -1, 460, 240, 980, 600, -1, 'y' },
	{ NULL,                       NULL,     "kimux",         0,      1, -1, 460, 240, 980, 600, -1, 'x' },
	{ NULL,                       NULL,     "cmus",          0,      1, -1, 460, 240, 980, 600, -1, 'm' },
	{ NULL,                       NULL,     "ncmpcpp",       0,      1, -1, 460, 240, 980, 600, -1, 'n' },
};
// }}}
// Layout(s) {{{
static const float  mfact           = 0.50; /* factor of master area size [0.05..0.95] */
static const int    nmaster         = 1;    /* number of clients in master area */
static const int    resizehints     = 0;    /* 1 means respect size hints in tiled resizals */
static const int    lockfullscreen  = 0;    /* 1 will force focus on the fullscreen window */
static const int    focusonwheel    = 0;
static const char   *layoutmenu_cmd = "xmenu_dwmlayout";
static const char   buttonbar[]     = "";
static const int    showtab         = showtab_auto;
static const int    toptab          = True;

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
// Named scratchpad
static const char *yakuakecmd[] = { "y", TERMINAL, "--title", "yakuake", NULL };
static const char *kimuxcmd[]   = { "x", TERMINAL, "--title", "kimux",   "-e", "tmux", NULL };
static const char *cmuscmd[]    = { "m", TERMINAL, "--title", "cmus",    "-e", "cmus", NULL };
static const char *ncmpcppcmd[] = { "n", TERMINAL, "--title", "ncmpcpp", "-e", "ncmpcpp", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	// Left Side {{{
	{ MODKEY|ShiftMask,             XK_q, killclient,     { 0 } },
	{ MODKEY,                       XK_w, focusmon,       { .i = -1 } },
	{ MODKEY,                       XK_e, focusmon,       { .i = +1 } },
	{ MODKEY|ShiftMask,             XK_w, tagmon,         { .i = -1 } },
	{ MODKEY|ShiftMask,             XK_e, tagmon,         { .i = +1 } },
	{ MODKEY,                       XK_r, view,           { 0 } },
	{ MODKEY|ShiftMask,             XK_r, setlayout,      { 0 } },
	{ MODKEY,                       XK_t, togglefloating, { 0 } },
	{ MODKEY,                       XK_a, spawn,          SHCMD(TERMINAL) },
	{ MODKEY|ShiftMask,             XK_a, spawn,          SHCMD("st") },
	{ MODKEY|ControlMask,           XK_a, spawn,          SHCMD("tabbed -c -r 2 st -w ''") },
	{ MODKEY,                       XK_d, spawn,          SHCMD("tabbed -c zathura -e") },
	{ MODKEY|ShiftMask,             XK_d, spawn,          SHCMD("evince") },
	{ MODKEY,                       XK_f, spawn,          SHCMD("google-chrome-stable") },
	{ MODKEY|ShiftMask,             XK_f, spawn,          SHCMD("vieb") },
	{ MODKEY|ShiftMask|ControlMask, XK_x, togglescratch,  { .v = kimuxcmd } },
	{ MODKEY,                       XK_b, togglebar,      { 0 } },
	{ MODKEY|ShiftMask,             XK_b, tabmode,        { -1 } },
	// }}}
	// Right Side {{{
	{ MODKEY,                       XK_y, togglescratch,  { .v = yakuakecmd } },
	{ MODKEY,                       XK_u, spawn,          SHCMD("dmenu_run") },
	{ MODKEY|ShiftMask,             XK_u, spawn,          SHCMD("rofi -modi drun,run,combi -show combi") },
	{ MODKEY,                       XK_o, spawn,          SHCMD("code") },
	{ MODKEY,                       XK_k, focusstack,     { .i = -1 } },
	{ MODKEY,                       XK_j, focusstack,     { .i = +1 } },
	{ MODKEY|ShiftMask,             XK_k, movestack,      { .i = -1 } },
	{ MODKEY|ShiftMask,             XK_j, movestack,      { .i = +1 } },
	{ MODKEY,                       XK_h, setmfact,       { .f = -0.05 } },
	{ MODKEY,                       XK_l, setmfact,       { .f = +0.05 } },
	{ MODKEY|ShiftMask|ControlMask, XK_n, togglescratch,  { .v = ncmpcppcmd } },
	{ MODKEY,                       XK_m, focusmaster,    { 0 } },
	{ MODKEY|ShiftMask,             XK_m, zoom,           { 0 } },
	{ MODKEY|ShiftMask|ControlMask, XK_m, togglescratch,  { .v = cmuscmd } },
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
	{ MODKEY|ShiftMask,   XK_space,     setlayout,   { .v = &layouts[4] } },
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
//   ClkWinTitle, ClkClientWin, ClkRootWin,
//   ClkButton, ClkTabBar
static const Button buttons[] = {
	/* click     event mask button   function        argument */
	{ ClkButton,     0,      Button1, spawn,          SHCMD("xdg-xmenu") },
	{ ClkButton,     0,      Button3, spawn,          SHCMD("xmenu-shutdown") },
	{ ClkButton,     0,      Button2, spawn,          SHCMD("weather") },
	{ ClkButton,     0,      Button4, spawn,          SHCMD("xbacklight -inc 5") },
	{ ClkButton,     0,      Button5, spawn,          SHCMD("xbacklight -dec 5") },
	{ ClkLtSymbol,   0,      Button1, setlayout,      { 0 } },
	{ ClkLtSymbol,   0,      Button2, setlayout,      { .v = &layouts[1] } },
	{ ClkLtSymbol,   0,      Button3, layoutmenu,     { 0 } },
	{ ClkLtSymbol,   0,      Button4, cyclelayout,    { .i = +1 } },
	{ ClkLtSymbol,   0,      Button5, cyclelayout,    { .i = -1 } },
	{ ClkWinTitle,   0,      Button2, zoom,           { 0 } },
	{ ClkWinTitle,   0,      Button4, movestack,      { .i = +1 } },
	{ ClkWinTitle,   0,      Button5, movestack,      { .i = -1 } },
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
	{ ClkTabBar,     0,      Button1, focuswin,       {0} },
	{ ClkTabBar,     0,      Button2, togglefloating, {0} },
};
// }}}
// }}}

#endif /* ifndef DWM_H */

// vim:fdm=marker
