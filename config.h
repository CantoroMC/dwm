// Appearance
static const unsigned int borderpx       = 1;  /* border pixel of windows */
static const unsigned int snap           = 24; /* snap pixel */
static const int showbar                 = 1;  /* 0 means no bar */
static const int topbar                  = 1;  /* 0 means bottom bar */
static const char *fonts[]               = {
	"Operator Mono Book:size=12.0"
};
static const char norm_bg[]              = "#151a1e";
static const char norm_border[]          = "#151a1e";
static const char norm_fg[]              = "#b8cc52";
static const char self_fg[]              = "#eaeaea";
static const char self_bg[]              = "#3f4e5a";
static const char self_border[]          = "#b8cc52";
static const char *colors[][3]           = {
	[SchemeNorm] = { norm_fg, norm_bg, norm_border },
	[SchemeSel]  = { self_fg, self_bg, self_border },
};



// Tagging and Rules
static const char *tags[] = { "α", "β", "γ", "δ", "ε", "ζ", "η", "θ", "ι" };
static const Rule rules[] = {
	/* class                    instance     title             tags mask  isfloating  monitor */
	{ "Arandr",                   NULL,       NULL,               0,       1,          -1 },
	{ "Avahi-discover",           NULL,       NULL,               0,       1,          -1 },
	{ "Baobab",                   NULL,       NULL,               0,       1,          -1 },
	{ "Blueberry.py",             NULL,       NULL,               0,       1,          -1 },
	{ "Bssh",                     NULL,       NULL,               0,       1,          -1 },
	{ "Bvnc",                     NULL,       NULL,               0,       1,          -1 },
	{ "CMakeSetup",               NULL,       NULL,               0,       1,          -1 },
	{ "Exo-helper-2",             NULL,       NULL,               0,       1,          -1 },
	{ "feh",                      NULL,       NULL,               0,       1,          -1 },
	{ "Gimp",                     NULL,       NULL,               0,       1,          -1 },
	{ "Gnome-disks",              NULL,       NULL,               0,       1,          -1 },
	{ "Gpick",                    NULL,       NULL,               0,       1,          -1 },
	{ "Hardinfo",                 NULL,       NULL,               0,       1,          -1 },
	{ "imagewriter",              NULL,       NULL,               0,       1,          -1 },
	{ "Lxappearance",             NULL,       NULL,               0,       1,          -1 },
	{ "MPlayer",                  NULL,       NULL,               0,       1,          -1 },
	{ "Nitrogen",                 NULL,       NULL,               0,       1,          -1 },
	{ "ParaView",                 NULL,       NULL,               0,       1,          -1 },
	{ "Parcellite",               NULL,       NULL,               0,       1,          -1 },
	{ "Pavucontrol",              NULL,       NULL,               0,       1,          -1 },
	{ "qv4l2",                    NULL,       NULL,               0,       1,          -1 },
	{ "qvidcap",                  NULL,       NULL,               0,       1,          -1 },
	{ "System-config-printer.py", NULL,       NULL,               0,       1,          -1 },
	{ "Sxiv",                     NULL,       NULL,               0,       1,          -1 },
	{ "Xarchiver",                NULL,       NULL,               0,       1,          -1 },
	{ "Xboard",                   NULL,       NULL,               0,       1,          -1 },
	{ "Xfce4-about",              NULL,       NULL,               0,       1,          -1 },
	{ "Xmessage",                 NULL,       NULL,               0,       1,          -1 },
	{ "Yad",                      NULL,       NULL,               0,       1,          -1 },
	{ "Yad-icon-browser",         NULL,       NULL,               0,       1,          -1 },
	{ NULL,                       NULL,       "Event Tester",     0,       1,          -1 },
	{ NULL,                       NULL,       "lstopo",           0,       1,          -1 },
	{ NULL,                       NULL,       "weatherreport",    0,       1,          -1 },
	{ NULL,                       "pop-up",   NULL,               0,       1,          -1 },
	{ "Display",                  NULL,       "ImageMagick: ",    0,       1,          -1 },
	{ "Transmission-gtk",         NULL,       NULL,               1 << 8,  1,          -1 },
	{ "mpv",                      NULL,       NULL,               1 << 4,  0,          -1 },
};



// Layouts
static const float mfact      = 0.5;  /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const int resizehints  = 0;    /* 1 means respect size hints in tiled resizals */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },
	{ ">>=",      NULL },
	{ "[M]",      monocle },
};



// Key Definitions
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *roficmd[]  = { "rofi", "-modi", "drun,run", "-show", "drun", NULL};
static const char *browsercmd[]  = { "vivaldi-stable", NULL };

#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

static Key keys[] = {
	/* modifier                     key        function        argument */

	/*                              Left Side                           */

	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	// Monitors: shift and move to previous/next
	{ MODKEY,                       XK_w,      focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_e,      focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_w,      tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_e,      tagmon,         {.i = +1 } },
	// Go to previous tag/layout
	{ MODKEY,                       XK_r,      view,           {0} },
	{ MODKEY|ShiftMask,             XK_r,      setlayout,      {0} },
	{ MODKEY,                       XK_t,      togglefloating, {0} },
	{ MODKEY,                       XK_f,      spawn,          {.v = browsercmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },

	/*                              Right Side                          */

	// Focus previous/next client
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	// Resize Master fraction
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	// toggle between master and stack
	{ MODKEY|ShiftMask,             XK_m,      zoom,           {0} },
	// Menu launchers
	{ MODKEY,                       XK_u,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_u,      spawn,          {.v = roficmd } },

	/*                              Surrounding Keys                    */

	// Master/Stack Vertical Layout
	{ MODKEY,                       XK_Tab,    setlayout,      {.v = &layouts[0]} },
	// Floating Layout
	{ MODKEY,                       XK_space,  setlayout,      {.v = &layouts[1]} },
	// Monocle Layout
	{ MODKEY|ShiftMask,             XK_space,  setlayout,      {.v = &layouts[2]} },
	// Decrease/Increase Number of masters
	{ MODKEY,                       XK_period, incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_comma,  incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_Delete, quit,           {0} },

	/*                              Numbers                             */

	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

	/*                              Fn and Extra Keys                   */

};



// Mouse Button Definitions
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

// vim:nospell
