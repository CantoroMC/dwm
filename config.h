// Appearance
static const unsigned int borderpx       = 1;            /* border pixel of windows */
static const unsigned int snap           = 24;           /* snap pixel */
static const Gap          default_gap    = {.isgap = 1, .realgap = 0, .gappx = 0};
static const int          showbar        = 1;            /* 0 means no bar */
static const int          topbar         = 1;            /* 0 means bottom bar */
static const char         buttonbar[]    = "";
static const unsigned int systraypinning = 1;            /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;            /* systray spacing */
// 1: if pinning fails, display systray on the first monitor,
//    False: display systray on the last monitor
static const int systraypinningfailfirst = 1;
static const int showsystray             = 1;            /* 0 means no systray */
// Display modes of the tab bar:
//   never shown, always shown, shown only in monocle mode in presence of several windows.
// Modes after showtab_nmodes are disabled
enum showtab_modes { showtab_never, showtab_auto, showtab_nmodes, showtab_always };
static const int showtab                 = showtab_auto; /* Default tab bar show mode */
static const Bool toptab                 = True;        /* False means bottom tab bar */
static const char *fonts[]               = {
	"Operator Mono Lig Book:size=9.0",
	"FiraCode Nerd Font Book:size=9.0",
};
static const char norm_bg[]              = "#f2e5bc";
static const char norm_border[]          = "#f2e5bc";
static const char norm_fg[]              = "#427b58";
static const char self_fg[]              = "#79740e";
static const char self_bg[]              = "#f2e5bc";
static const char self_border[]          = "#79740e";
static const char *colors[][3]           = {
	[SchemeNorm]  = { norm_fg, norm_bg,  norm_border },
	[SchemeSel]   = { self_fg, self_bg,  self_border },
	[SchemeTitle] = { self_fg, "#f9f5d7", norm_border },
};



// Tagging and Rules (Sinhala Archaic Digits)
static const char *tags[] = { "𑇡", "𑇢", "𑇣", "𑇤", "𑇥", "𑇦", "𑇧", "𑇨", "𑇩" };
// default layout per tags
// The first element is for all-tag view, following i-th element corresponds to tags[i].
// Layout is referred using the layouts array index.
static int def_layouts[1 + LENGTH(tags)]  = { 0, 0, 0, 5, 0, 0, 0, 0, 7, 3 };
static const Rule rules[] = {
	/* class                          instance     title             tags mask  iscentered isfloating  monitor  scratch key */
	{ "Arandr",                       NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Avahi-discover",               NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Baobab",                       NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Blueberry.py",                 NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Bssh",                         NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Bvnc",                         NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "CMakeSetup",                   NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Conky",                        NULL,       NULL,              ~0,       0,          1,          -1,      0 },
	{ "Exo-helper-2",                 NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "feh",                          NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Gimp",                         NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Gnome-disks",                  NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Gpick",                        NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Hardinfo",                     NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "imagewriter",                  NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Lxappearance",                 NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "MPlayer",                      NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Nitrogen",                     NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "ParaView",                     NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Parcellite",                   NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Pavucontrol",                  NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "qv4l2",                        NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "qvidcap",                      NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Snapper-gui",                  NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "System-config-printer.py",     NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Sxiv",                         NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Xarchiver",                    NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Xboard",                       NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Xfce4-about",                  NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Xmessage",                     NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Yad",                          NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ "Yad-icon-browser",             NULL,       NULL,               0,       1,          1,          -1,      0 },
	{ NULL,                           NULL,       "Event Tester",     0,       1,          1,          -1,      0 },
	{ NULL,                           NULL,       "lstopo",           0,       1,          1,          -1,      0 },
	{ NULL,                           NULL,       "weatherreport",    0,       1,          1,          -1,      0 },
	{ NULL,                           "pop-up",   NULL,               0,       1,          1,          -1,      0 },
	{ "Display",                      NULL,       "ImageMagick: ",    0,       1,          1,          -1,      0 },
	{ "MATLAB R2021a - academic use", NULL,       "Help",             0,       0,          1,          -1,      0 },
	{ "MATLAB R2021a - academic use", NULL,       "Preferences",      0,       1,          1,          -1,      0 },
	{ "Transmission-gtk",             NULL,       NULL,               1 << 8,  1,          1,          -1,      0 },
	{ "mpv",                          NULL,       NULL,               1 << 4,  0,          0,          -1,      0 },
	{ NULL,                           NULL,       "MATLAB",           1 << 1,  0,          0,          -1,      0 },
	{ NULL,                           NULL,       "yakuake",          0,       1,          1,          -1,      'y' },
	{ NULL,                           NULL,       "cmus",             0,       1,          1,          -1,      'm' },
	{ NULL,                           NULL,       "ncmpcpp",          0,       1,          1,          -1,      'n' },
	{ NULL,                           NULL,       "orgenda",          0,       1,          1,          -1,      'o' },
	/* class                          instance     title             tags mask  iscentered isfloating  monitor  scratch key */
};



// Layouts
static const int focusonwheel    = 0;
static const float mfact         = 0.5;  /* factor of master area size [0.05..0.95] */
static const float smfact        = 0.00; /* factor of tiled clients [0.00..0.95] */
static const int nmaster         = 1;    /* number of clients in master area */
static const unsigned int minwsz = 20;   /* Minimal height of a client for smfact */
static const int resizehints     = 0;    /* 1 means respect size hints in tiled resizals */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },
	{ "[D]",      deck },
	{ "TTT",      bstack },
	{ ">>=",      NULL },
	{ "|+|",      tatami },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "[M]",      monocle },
	{ NULL,       NULL },
};



// Key Definitions
static       char dmenumon[2]     = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *termcmd[]      = { "kitty", NULL };
static const char *dmenucmd[]     = { "dmenu_run", "-m", dmenumon, NULL };
static const char *layoutmenu_cmd = "xmenu_dwmlayout";


#define MODKEY Mod4Mask
#define TAGKEYS(CHAIN,KEY,TAG) \
	{ MODKEY,                       CHAIN,    KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           CHAIN,    KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             CHAIN,    KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, CHAIN,    KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define TERMINAL "kitty"

/*First arg only serves to match against key in rules*/
static const char *yakuakecmd[] = {"y", TERMINAL, "--title", "yakuake", NULL};
static const char *cmuscmd[]    = {"m", TERMINAL, "--title", "cmus", "-e", "cmus", NULL};
static const char *ncmpcppcmd[] = {"n", TERMINAL, "--title", "ncmpcpp", "-e", "ncmpcpp", NULL};
static const char *orgendacmd[] = {"o", "emacs",  "--name=orgenda", "~/Documents/organization/Notes.org", NULL};

static Key keys[] = {
	/* modifier                     chain key   key        function        argument */
	/*                              Left Side                           */
	{ MODKEY,                       -1,         XK_q,            resetlayout,    {0} },
	{ MODKEY|ShiftMask,             -1,         XK_q,            killclient,     {0} },
	// Monitors: shift and move to previous/next
	{ MODKEY,                       -1,         XK_w,            focusmon,       {.i = -1 } },
	{ MODKEY,                       -1,         XK_e,            focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             -1,         XK_w,            tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             -1,         XK_e,            tagmon,         {.i = +1 } },
	// Go to previous tag/layout
	{ MODKEY,                       -1,         XK_r,            view,           {0} },
	{ MODKEY|ShiftMask,             -1,         XK_r,            setlayout,      {0} },
	{ MODKEY|ControlMask,           -1,         XK_r,            spawn,          SHCMD("tabbed -c zathura -e") },
	{ MODKEY,                       -1,         XK_t,            togglefloating, {0} },
	{ MODKEY|ShiftMask,             -1,         XK_t,            tabmode,        {-1} },

	{ MODKEY,                       -1,         XK_a,            spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             -1,         XK_a,            spawn,          SHCMD("st") },
	{ MODKEY|ControlMask,           -1,         XK_a,            spawn,          SHCMD("tabbed -c -r 2 st -w ''") },
	// Gaps or Spacing
	{ MODKEY,                       -1,         XK_s,            setgaps,        {.i = +3 } },
	{ MODKEY|ShiftMask,             -1,         XK_s,            setgaps,        {.i = -3 } },
	{ MODKEY|ControlMask,           -1,         XK_s,            setgaps,        {.i = GAP_RESET } },
	{ MODKEY|ShiftMask|ControlMask, -1,         XK_s,            setgaps,        {.i = GAP_TOGGLE} },

	{ MODKEY,                       -1,         XK_d,            spawn,          SHCMD("emacs") },
	{ MODKEY|ShiftMask,             XK_d,       XK_y,            togglescratch,  {.v = yakuakecmd } },
	{ MODKEY|ShiftMask,             XK_d,       XK_m,            togglescratch,  {.v = cmuscmd } },
	{ MODKEY|ShiftMask,             XK_d,       XK_n,            togglescratch,  {.v = ncmpcppcmd } },
	{ MODKEY|ShiftMask,             XK_d,       XK_o,            togglescratch,  {.v = orgendacmd } },

	{ MODKEY,                       -1,         XK_f,            spawn,          SHCMD("vivaldi-stable") },
	{ MODKEY|ShiftMask,             -1,         XK_f,            spawn,          SHCMD("surf-open") },
	{ MODKEY,                       -1,         XK_b,            togglebar,      {0} },
	/*                              Right Side                                           */
	// Focus previous/next client
	{ MODKEY,                       -1,         XK_k,            focusstack,     {.i = -1 } },
	{ MODKEY,                       -1,         XK_j,            focusstack,     {.i = +1 } },
	// Move the focused client forward/backward in the stack
	{ MODKEY|ShiftMask,             -1,         XK_k,            movestack,      {.i = -1 } },
	{ MODKEY|ShiftMask,             -1,         XK_j,            movestack,      {.i = +1 } },
	// Resize slave clients
	{ MODKEY|ShiftMask,             -1,         XK_l,            setsmfact,      {.f = -0.05} },
	{ MODKEY|ShiftMask,             -1,         XK_h,            setsmfact,      {.f = +0.05} },
	// Resize Master fraction
	{ MODKEY,                       -1,         XK_h,            setmfact,       {.f = -0.05} },
	{ MODKEY,                       -1,         XK_l,            setmfact,       {.f = +0.05} },
	// focus the master client
	{ MODKEY,                       -1,         XK_m,            focusmaster,    {0} },
	// toggle between master and stack
	{ MODKEY|ShiftMask,             -1,         XK_m,            zoom,           {0} },
	// Menu launchers
	{ MODKEY,                       -1,         XK_u,            spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             -1,         XK_u,            spawn,          SHCMD("rofi -modi drun,run,combi -show combi") },
	{ MODKEY|ControlMask,           -1,         XK_u,            spawn,          SHCMD("xmenu-apps") },
	{ MODKEY|ShiftMask|ControlMask, -1,         XK_u,            spawn,          SHCMD("xmenu-utilities") },
	// Scratchpads
	{ MODKEY|ShiftMask|ControlMask, -1,         XK_y,            togglescratch,  {.v = yakuakecmd } },
	{ MODKEY|ShiftMask|ControlMask, -1,         XK_m,            togglescratch,  {.v = cmuscmd } },
	{ MODKEY|ShiftMask|ControlMask, -1,         XK_n,            togglescratch,  {.v = ncmpcppcmd } },
	{ MODKEY|ShiftMask|ControlMask, -1,         XK_o,            togglescratch,  {.v = orgendacmd } },
	{ MODKEY,                       XK_p,       XK_h,            spawn,          SHCMD("mpc prev") },
	{ MODKEY,                       XK_p,       XK_l,            spawn,          SHCMD("mpc next") },
	{ MODKEY,                       XK_p,       XK_j,            spawn,          SHCMD("mpc play") },
	{ MODKEY,                       XK_p,       XK_k,            spawn,          SHCMD("mpc pause") },
	{ MODKEY,                       XK_p,       XK_space,        spawn,          SHCMD("mpc toggle") },
	{ MODKEY,                       XK_p,       XK_r,            spawn,          SHCMD("mpc repeat") },
	{ MODKEY,                       XK_p,       XK_c,            spawn,          SHCMD("mpc consume") },
	{ MODKEY,                       XK_p,       XK_z,            spawn,          SHCMD("mpc random") },
	{ MODKEY,                       XK_p,       XK_y,            spawn,          SHCMD("mpc single") },
	{ MODKEY,                       XK_p,       XK_m,            spawn,          SHCMD(TERMINAL " -e pulsemixer") },
	{ MODKEY,                       XK_p,       XK_q,            spawn,          SHCMD("mpv_bulk_quit") },
	{ MODKEY,                       XK_p,       XK_t,            spawn,          SHCMD("mpv_bulk_toggle") },
	/*                              Surrounding Keys                                     */
	// Master/Stack Vertical Layout
	{ MODKEY,                       -1,         XK_Tab,          cyclelayout,    {.i = +1 } },
	{ MODKEY|ShiftMask,             -1,         XK_Tab,          cyclelayout,    {.i = -1 } },
	// Floating Layout
	{ MODKEY,                       -1,         XK_space,        setlayout,      {.v = &layouts[3]} },
	// Monocle Layout
	{ MODKEY|ShiftMask,             -1,         XK_space,        setlayout,      {.v = &layouts[7]} },
	// Horizontal Layout
	{ MODKEY|ControlMask,           -1,         XK_space,        setlayout,      {.v = &layouts[2]} },
	// Centered Master Layout
	{ MODKEY|ShiftMask|ControlMask, -1,         XK_space,        setlayout,      {.v = &layouts[5]} },
	// Decrease/Increase Number of masters
	{ MODKEY,                       -1,         XK_period,       incnmaster,     {.i = -1 } },
	{ MODKEY,                       -1,         XK_comma,        incnmaster,     {.i = +1 } },

	{ MODKEY,                       -1,         XK_bracketleft,  shiftview,      { .i = -1 } },
	{ MODKEY,                       -1,         XK_bracketright, shiftview,      { .i = +1 } },
	{ MODKEY|ShiftMask,             -1,         XK_bracketleft,  shifttag,       { .i = -1 } },
	{ MODKEY|ShiftMask,             -1,         XK_bracketright, shifttag,       { .i = +1 } },

	{ MODKEY,                       -1,         XK_Return,       spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             -1,         XK_Return,       spawn,          SHCMD("st") },
	{ MODKEY|ControlMask,           -1,         XK_Return,       spawn,          SHCMD("tabbed -c -r 2 st -w ''") },
	{ MODKEY|ShiftMask,             -1,         XK_BackSpace,    spawn,          SHCMD("slock") },
	{ MODKEY,                       -1,         XK_Delete,       spawn,          SHCMD("xmenu-shutdown") },
	{ MODKEY|ShiftMask,             -1,         XK_Delete,       quit,           {0} },

	{ MODKEY,                       -1,         XK_Print,        spawn,          SHCMD("scrot ~/Pictures/Screenshots/%Y-%m-%d-%T-screenshoot.png; notify-send -i photo 'Taken fullscreen screenshot'") },
	{ MODKEY|ShiftMask,             -1,         XK_Print,        quit,           SHCMD("import ~/Pictures/Screenshots/$(date +%Y-%m-%d-%T)screenshoot.png; notify-send -i photo 'Saved to ~/Pictures/Screenshots'") },
	/*                              Arrows                                             */
	{ MODKEY,                       -1,         XK_Down,         moveresize,     {.v = "0x 25y 0w 0h"  } },
	{ MODKEY,                       -1,         XK_Up,           moveresize,     {.v = "0x -25y 0w 0h" } },
	{ MODKEY,                       -1,         XK_Right,        moveresize,     {.v = "25x 0y 0w 0h"  } },
	{ MODKEY,                       -1,         XK_Left,         moveresize,     {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             -1,         XK_Down,         moveresize,     {.v = "0x 0y 0w 25h"  } },
	{ MODKEY|ShiftMask,             -1,         XK_Up,           moveresize,     {.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,             -1,         XK_Right,        moveresize,     {.v = "0x 0y 25w 0h"  } },
	{ MODKEY|ShiftMask,             -1,         XK_Left,         moveresize,     {.v = "0x 0y -25w 0h" } },
	{ MODKEY|ControlMask,           -1,         XK_Down,         spawn,          SHCMD("mpc pause")},
	{ MODKEY|ControlMask,           -1,         XK_Up,           spawn,          SHCMD("mpc play")},
	{ MODKEY|ControlMask,           -1,         XK_Right,        spawn,          SHCMD("mpc next")},
	{ MODKEY|ControlMask,           -1,         XK_Left,         spawn,          SHCMD("mpc prev")},
	/*                              Numbers                                              */
	{ MODKEY,                       -1,         XK_0,            view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             -1,         XK_0,            tag,            {.ui = ~0 } },
	TAGKEYS(                        -1,         XK_1, 0)
	TAGKEYS(                        -1,         XK_2, 1)
	TAGKEYS(                        -1,         XK_3, 2)
	TAGKEYS(                        -1,         XK_4, 3)
	TAGKEYS(                        -1,         XK_5, 4)
	TAGKEYS(                        -1,         XK_6, 5)
	TAGKEYS(                        -1,         XK_7, 6)
	TAGKEYS(                        -1,         XK_8, 7)
	TAGKEYS(                        -1,         XK_9, 8)
	/*                              Fn and Extra Keys                                    */
	{ 0, -1, XF86XK_AudioMute,         spawn,  SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle") },
	{ 0, -1, XF86XK_AudioLowerVolume,  spawn,  SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%") },
	{ 0, -1, XF86XK_AudioRaiseVolume,  spawn,  SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%") },
	{ 0, -1, XF86XK_MonBrightnessDown, spawn,  SHCMD("xbacklight -dec 5") },
	{ 0, -1, XF86XK_MonBrightnessUp,   spawn,  SHCMD("xbacklight -inc 5") },
	{ 0, -1, XF86XK_Display,           spawn,  SHCMD("monitor_handler") },
	{ 0, -1, XF86XK_Search,            spawn,  SHCMD(TERMINAL " -e nnn") },
	{ 0, -1, XF86XK_Explorer,          spawn,  SHCMD("vivaldi-stable") },
	{ 0, -1, XF86XK_Calculator,        spawn,  SHCMD(TERMINAL " -e ghci") },
};



// Mouse Button Definitions
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkButton,            0,              Button1,        spawn,          SHCMD("xmenu-apps") },
	{ ClkButton,            0,              Button3,        spawn,          SHCMD("xmenu-shutdown") },
	{ ClkButton,            0,              Button2,        spawn,          SHCMD("weather") },
	{ ClkButton,            0,              Button4,        spawn,          SHCMD("xbacklight -inc 5") },
	{ ClkButton,            0,              Button5,        spawn,          SHCMD("xbacklight -dec 5") },

	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,            0,              Button4,        shiftview,      {.i = +1} },
	{ ClkTagBar,            0,              Button5,        shiftview,      {.i = -1} },

	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button2,        setlayout,      {.v = &layouts[6]} },
	{ ClkLtSymbol,          0,              Button3,        layoutmenu,     {0} },
	{ ClkLtSymbol,          0,              Button4,        cyclelayout,    {.i = +1 } },
	{ ClkLtSymbol,          0,              Button5,        cyclelayout,    {.i = -1 } },

	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkWinTitle,          0,              Button4,        movestack,      {.i = +1 } },
	{ ClkWinTitle,          0,              Button5,        movestack,      {.i = -1 } },

	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkStatusText,        0,              Button3,        spawn,          SHCMD(TERMINAL " -e htop") },
	{ ClkStatusText,        0,              Button4,        spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%") },
	{ ClkStatusText,        0,              Button5,        spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%") },

	{ ClkRootWin,           0,              Button2,        togglebar,      {0} },

	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },

	{ ClkTabBar,            0,              Button1,        focuswin,       {0} },
	{ ClkTabBar,            0,              Button2,        togglefloating, {0} },
};

// vim:ft=c:nospell:tw=0
