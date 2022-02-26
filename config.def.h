/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray             = 1;   /* 0 means no systray */
static const char *fonts[]          = { "JetBrains Mono:size=14", "LXGW WenKai Mono:size=14" };
#define COL_NB_HARD     "#1d2021"
#define COL_NB          "#282828"
#define COL_NF          "#a89984"
#define COL_SB          "#689d6a"
#define COL_SF          "#ebdbb2"
#define COL_NULL        "#000000"
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*                     fg         bg         border   */
	[SchemeNorm]       = { COL_NF, COL_NB,      COL_SB },
	[SchemeSel]        = { COL_NF, COL_NB,      COL_SB},
	[SchemeStatus]     = { COL_NF, COL_NB,      COL_SB }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]    = { COL_NB, COL_SB,      COL_NULL },// Tagbar left selected {text,background,not used but cannot be empty}
    [SchemeTagsNorm]   = { COL_NF, COL_NB_HARD, COL_NULL }, // Tagbar left unselected {text,background,not used but cannot be empty}
    [SchemeInfoSel]    = { COL_SF, COL_NB,      COL_NULL },// infobar middle  selected {text,background,not used but cannot be empty}
    [SchemeInfoNorm]   = { COL_NF, COL_NB,      COL_NULL }, // infobar middle  unselected {text,background,not used but cannot be empty}
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
/*
   用 Windows 键作为 ModKey
   之后可以在重映射 Alt 与 Windows 键位置
   避免 Alt 相关冲突
   映射(Xmodmap)见 https://gist.github.com/zpyg/9b2af4b2cc3416e4efbedb3879d387f9
*/
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
// 快速运行程序
#define RUN(program, ...) { .v = (const char*[]) {program, __VA_ARGS__} }
// 使用 st 运行
#define TERM_RUN(program, ...) { .v = (const char*[]) {"st", "-e", program, __VA_ARGS__} }
// 电源控制，需允许 systemctl 无需密码
#define SYSCTL(action) RUN("sudo", "systemctl", action, NULL)

/* commands */
static const char *termcmd[] = {"st", NULL};
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
    { MODKEY,                       XK_e,      spawn,          TERM_RUN("vifmrun", NULL) },
	{ ControlMask|ShiftMask,        XK_Escape, spawn,          TERM_RUN("bpytop", NULL) },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          RUN("flameshot", "gui", NULL) },
    { MODKEY,                       XK_v,      spawn,          RUN("diodon", NULL) },
	{ MODKEY,                       XK_F1,     spawn,          SYSCTL("poweroff") },
	{ MODKEY,                       XK_F2,     spawn,          SYSCTL("reboot") },
	{ MODKEY,                       XK_F3,     spawn,          SYSCTL("suspend") },
	{ MODKEY,                       XK_F4,     spawn,          RUN("slock", NULL) },

	{ MODKEY,                       XK_p,      spawn,          RUN("rofi", "-show", "run", NULL)},
	{ MODKEY|ShiftMask,             XK_Escape, killclient,     {0} },

	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd} },
	{ MODKEY,                       XK_grave,  togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
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

