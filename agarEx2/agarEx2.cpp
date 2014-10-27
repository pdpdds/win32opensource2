// agarEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <windows.h>
#include <gl/gl.h>
#include "SDL.h"
#include <agar/core.h>
#include <agar/gui.h>
#include <agar/dev.h>
#include <math.h>

#pragma comment(lib, "ag_gui_static.lib")
#pragma comment(lib, "ag_dev_static.lib")
#pragma comment(lib, "ag_math_static.lib")
#pragma comment(lib, "ag_rg_static.lib")
#pragma comment(lib, "ag_vg_static.lib")
#pragma comment(lib, "ag_core_static.lib")
#pragma comment(lib, "pthreadVC2.lib")
#pragma comment(lib, "sdl.lib")
#pragma comment(lib, "sdlmain.lib")
#pragma comment(lib, "freetype253.lib")
#pragma comment(lib, "opengl32.lib")
VOID CreateWindowAA(void);

#include <agar/config/have_sdl.h>
#include <agar/config/have_opengl.h>
//#include "config/have_sdl_image.h"

#include "mytheme.h"

#ifdef HAVE_SDL
#include <SDL.h>
# ifdef HAVE_SDL_IMAGE
#  include <SDL_image.h>
# endif
#endif

#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/opengl.h>

static void
ComboSelected(AG_Event *event)
{
	AG_TlistItem *ti = (AG_TlistItem*)AG_PTR(1);

	AG_TextMsg(AG_MSG_INFO, "Item %s", ti->text);
}

static void
Preferences(AG_Event *event)
{
	DEV_ConfigShow();
}

static void
Quit(AG_Event *event)
{
	AG_QuitGUI();
}

static void
MyKeyboardHandler(AG_Event *event)
{
	AG_Console *cons = (AG_Console*)AG_PTR(1);
	int sym = AG_INT(2);
	int mod = AG_INT(3);
	Uint32 unicode = (Uint32)AG_ULONG(4);

	AG_ConsoleMsg(cons,
		"%s: sym=%d, modifier=0x%x, unicode=0x%lx",
		event->name, sym, (unsigned)mod, unicode);
}

int
main(int argc, char *argv[])
{
#ifdef HAVE_SDL
	AG_Window *win;
	SDL_Surface *screen, *bmp, *tex1, *tex2, *avatar;
	AG_Surface *agbmp, *agavatar;
	int c, useGL = 0;
	Uint32 sdlFlags = 0;
	char *optArg;
	Uint32 rmask, gmask, bmask, amask;

	if (AG_InitCore("agar-sdl-demo", 0) == -1) {
		fprintf(stderr, "AG_InitCore: %s\n", AG_GetError());
		goto fail;
	}
	while ((c = AG_Getopt(argc, argv, "?g", &optArg, NULL)) != -1) {
		switch (c) {
		case 'g':
			useGL = 1;
			break;
		default:
			printf("Usage: %s [-g]\n", agProgName);
			break;
		}
	}

	/* Set up SDL */
	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
		return (1);
	}
	if (useGL) {
		sdlFlags = SDL_RESIZABLE | SDL_OPENGL;
	}
	else {
		sdlFlags = SDL_RESIZABLE | SDL_SWSURFACE;
	}
	if ((screen = SDL_SetVideoMode(1024, 768, 32, sdlFlags)) == NULL) {
		fprintf(stderr, "SDL_SetVideoMode: %s\n", SDL_GetError());
		goto fail;
	}

	if (useGL) {
		/* Set up OpenGL viewport and projection. */
		AG_GL_InitContext(AG_RECT(0, 0, 1024, 768));
	}

	/* Initialize Agar-GUI to reuse display */
	if (AG_InitVideoSDL(screen, 0) == -1) {
		fprintf(stderr, "%s\n", AG_GetError());
		AG_Destroy();
		goto fail;
	}
	AG_BindGlobalKey(AG_KEY_ESCAPE, AG_KEYMOD_ANY, AG_QuitGUI);

	/* Display some test widgets. */
	win = AG_WindowNew(AG_WINDOW_PLAIN);
	AG_LabelNew(win, 0, "Attached to existing %s display",
		useGL ? "SDL/OpenGL" : "SDL");

	/* Test conversion from SDL_Surface to AG_Surface. */
	if ((bmp = SDL_LoadBMP("agar.bmp")) != NULL) {
		if ((agbmp = AG_SurfaceFromSDL(bmp)) != NULL) {
			AG_PixmapFromSurface(win, 0, agbmp);
		}
		else {
			AG_LabelNewS(win, 0, AG_GetError());
		}
	}

	double vDbl = 1.0, minDbl = -1e6, maxDbl = 1e6, visDbl = 0.0;
	AG_Console *cons;
	cons = AG_ConsoleNew(win, AG_CONSOLE_EXPAND);
	AG_SetEvent(win, "key-down", MyKeyboardHandler, "%p", cons);
	AG_SetEvent(win, "key-up", MyKeyboardHandler, "%p", cons);

	AGWIDGET(win)->flags |= AG_WIDGET_UNFOCUSED_KEYUP;
	AGWIDGET(win)->flags |= AG_WIDGET_UNFOCUSED_KEYDOWN;


	AG_Scrollbar *sb;
	sb = AG_ScrollbarNewDouble(win, AG_SCROLLBAR_HORIZ, AG_SCROLLBAR_HFILL,
		&vDbl, &minDbl, &maxDbl, &visDbl);

	CreateWindowAA();

#ifdef HAVE_SDL_IMAGE
	if ((tex1 = IMG_Load("test1.png")) == NULL ||
		(tex2 = IMG_Load("test2.png")) == NULL) {
		fprintf(stderr, "IMG_Load() failed\n");
		goto fail;
	}
# if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
# else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
# endif
	avatar = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 64, 128,
		tex1->format->BitsPerPixel, rmask, gmask, bmask, amask);
	SDL_SetAlpha(tex1, 0, 0);
	SDL_BlitSurface(tex1, NULL, avatar, NULL);
	SDL_SetAlpha(tex2, SDL_SRCALPHA, 0);
	SDL_BlitSurface(tex2, NULL, avatar, NULL);
	if ((agavatar = AG_SurfaceFromSDL(avatar)) != NULL) {
		AG_PixmapFromSurface(win, 0, agavatar);
	}
	else {
		AG_LabelNewS(win, 0, AG_GetError());
	}
#endif /* HAVE_SDL_IMAGE */

	AG_WindowShow(win);

	AG_EventLoop();
	AG_Destroy();

	SDL_Quit();
	return (0);
fail:
	SDL_Quit();
	return (1);
#else
	fprintf(stderr, "Agar was compiled without SDL support\n");
	return (1);
#endif /* HAVE_SDL */
}


VOID CreateWindowAA(void)
{
	AG_Window *win;
	AG_Box *hBox, *vBox;
	AG_Pane *pane;
	AG_Combo *com;
	AG_UCombo *ucom;
	AG_Box *div1, *div2;
	AG_Textbox *tbox;
	int i;

	/*
	* Create a new window and attach widgets to it. The Window object
	* acts as a container widget that packs its children vertically.
	*/
	win = AG_WindowNew(0);
	AG_WindowSetCaption(win, "Some Agar-GUI widgets");
	AG_ObjectSetName(win, "MainWindow");

	/*
	* Pane provides two Box containers which can be resized using
	* a control placed in the middle.
	*/
	pane = AG_PaneNewHoriz(win, AG_PANE_EXPAND);
	AG_PaneSetDivisionMin(pane, 0, 50, 100);
	AG_PaneMoveDividerPct(pane, 40);
	div1 = pane->div[0];
	div2 = pane->div[1];
	{
		AG_Label *lbl;

		/* The Pixmap widget displays a raster surface. */
		AG_PixmapFromBMP(div1, 0, "agar.bmp");

		/*
		* The Label widget provides a simple static or polled label
		* (polled labels use special format strings; see AG_Label(3)
		* for details).
		*/
		AG_LabelNew(div1, 0, "This is a static label");

		lbl = AG_LabelNewPolled(div1, AG_LABEL_FRAME,
			"This is a polled label.\n"
			"Window is at %i,%i (%ux%u)",
			&AGWIDGET(win)->x,
			&AGWIDGET(win)->y,
			&AGWIDGET(win)->w,
			&AGWIDGET(win)->h);
		AG_LabelSizeHint(lbl, 1,
			"This is a polled label\n"
			"Window is at 0000,0000 (0000x0000)");
		AG_LabelJustify(lbl, AG_TEXT_CENTER);
	}

	/*
	* Box is a general-purpose widget container. We use AG_BoxNewHoriz()
	* for horizontal widget packing.
	*/
	hBox = AG_BoxNewHoriz(div1, AG_BOX_HOMOGENOUS | AG_BOX_HFILL);
	{
		/*
		* The Button widget is a simple push-button. It is typically
		* used to trigger events, but it can also bind its state to
		* an boolean (integer) value or a bitmask.
		*/
		for (i = 0; i < 5; i++)
			AG_ButtonNew(hBox, 0, "%c", 0x41 + i);
	}

	hBox = AG_BoxNewHoriz(div1, AG_BOX_HFILL);
	{
		/* The Radio checkbox is a group of radio buttons. */
		{
			const char *radioItems[] = {
				"Radio1",
				"Radio2",
				NULL
			};
			AG_Radio *rad;

			rad = AG_RadioNew(hBox, AG_RADIO_EXPAND, radioItems);
		}

		vBox = AG_BoxNewVert(hBox, 0);
		{
			/*
			* The Checkbox widget can bind to boolean values
			* and bitmasks.
			*/
			AG_CheckboxNew(vBox, 0, "Checkbox 1");
			AG_CheckboxNew(vBox, 0, "Checkbox 2");
		}
	}

	/* Separator simply renders horizontal or vertical line. */
	AG_SeparatorNew(div1, AG_SEPARATOR_HORIZ);

	/*
	* The Combo widget is a textbox widget with a expander button next
	* to it. The button triggers a popup window which displays a list
	* (using the AG_Tlist(3) widget).
	*/
	com = AG_ComboNew(div1, AG_COMBO_HFILL, "Combo: ");
	AG_ComboSizeHint(com, "Item #00 ", 10);
	AG_SetEvent(com, "combo-selected", ComboSelected, NULL);

	/* UCombo is a variant of Combo which looks like a single button. */
	ucom = AG_UComboNew(div1, AG_UCOMBO_HFILL);
	AG_UComboSizeHint(ucom, "Item #1234", 5);

	/* Populate the Tlist displayed by the combo widgets we just created. */
	for (i = 0; i < 50; i++) {
		AG_TlistAdd(com->list, NULL, "Item #%d", i);
		AG_TlistAdd(ucom->list, NULL, "Item #%d", i);
	}

	/*
	* Numerical binds to an integral or floating-point number.
	* It can also provides built-in unit conversion (see AG_Units(3)).
	*/
	{
		AG_Numerical *num;
		static float myFloat = 1.0;
		static int myMin = 0, myMax = 10, myInt = 1;

		num = AG_NumericalNewS(div1, AG_NUMERICAL_HFILL, "cm", "Real: ");
		AG_BindFloat(num, "value", &myFloat);

		num = AG_NumericalNewS(div1, AG_NUMERICAL_HFILL, NULL, "Int: ");
		AG_BindInt(num, "value", &myInt);
	}

	/*
	* Textbox is a single or multiline text edition widget. It can bind
	* to a fixed-size buffer and supports UTF-8.
	*/
	tbox = AG_TextboxNew(div1, AG_TEXTBOX_HFILL | AG_TEXTBOX_STATIC,
		"Enter text: ");

	/*
	* Scrollbar provides three bindings, "value", "min" and "max",
	* which we can bind to integers or floating-point variables.
	* Progressbar and Slider have similar interfaces.
	*/
	{
		static int myVal = 50, myMin = -100, myMax = 100, myVisible = 0;
		AG_Scrollbar *sb;
		AG_Slider *sl;
		AG_ProgressBar *pb;

		sb = AG_ScrollbarNewInt(div1, AG_SCROLLBAR_HORIZ,
			AG_SCROLLBAR_HFILL,
			&myVal, &myMin, &myMax, &myVisible);
		AG_ScrollbarSetIntIncrement(sb, 10);

		sl = AG_SliderNewInt(div1, AG_SLIDER_HORIZ,
			AG_SLIDER_HFILL,
			&myVal, &myMin, &myMax);
		AG_SliderSetIntIncrement(sl, 10);

		pb = AG_ProgressBarNewInt(div1, AG_PROGRESS_BAR_HORIZ,
			AG_PROGRESS_BAR_SHOW_PCT,
			&myVal, &myMin, &myMax);
	}

	/*
	* Notebook provides multiple containers which can be selected by
	* the user.
	*/
	{
		AG_Notebook *nb;
		AG_NotebookTab *ntab;
		AG_Table *table;
		AG_Menu *menu;
		AG_MenuItem *m;

		/* Create a test menu */
		menu = AG_MenuNew(div2, AG_MENU_HFILL);
		m = AG_MenuNode(menu->root, "File", NULL);
		AG_MenuAction(m, "Preferences...", agIconGear.s, Preferences, NULL);
		AG_MenuSeparator(m);
		AG_MenuAction(m, "Quit", agIconClose.s, Quit, NULL);
		m = AG_MenuNode(menu->root, "Test", NULL);
		AG_MenuNode(m, "Submenu A", NULL);
		AG_MenuSeparator(m);
		m = AG_MenuNode(m, "Submenu B", NULL);
		AG_MenuNode(m, "한글", NULL);
		AG_MenuNode(m, "Submenu D", NULL);
		AG_MenuNode(m, "Submenu E", NULL);

		nb = AG_NotebookNew(div2, AG_NOTEBOOK_EXPAND);

		ntab = AG_NotebookAddTab(nb, "Some table", AG_BOX_VERT);
		{
			float f;

			/*
			* AG_Table displays a set of cells organized in
			* rows and columns. It is optimized for cases where
			* the table is static or needs to be repopulated
			* periodically.
			*/
			table = AG_TableNew(ntab, AG_TABLE_EXPAND);
			AG_TableAddCol(table, "x", "33%", NULL);
			AG_TableAddCol(table, "sin(x)", "33%", NULL);
			AG_TableAddCol(table, "cos(x)", "33%", NULL);
			for (f = 0.0f; f < 60.0f; f += 0.3f) {
				/*
				* Insert a Table row for sin(f) and cos(f).
				* The directives of the format string are
				* documented in AG_Table(3).
				*/
				AG_TableAddRow(table, "%.02f:%.02f:%.02f",
					f, sin(f), cos(f));
			}
		}

		ntab = AG_NotebookAddTab(nb, "Some text", AG_BOX_VERT);
		{
			char *someText;
			size_t size, bufSize;
			FILE *f;

			/*
			* Textboxes with the MULTILINE flag provide basic
			* text edition functionality. The CATCH_TAB flag
			* causes the widget to receive TAB key events
			* (normally used to focus other widgets).
			*/
			tbox = AG_TextboxNew(ntab,
				AG_TEXTBOX_MULTILINE | AG_TEXTBOX_CATCH_TAB |
				AG_TEXTBOX_EXPAND | AG_TEXTBOX_STATIC, NULL);
			AG_WidgetSetFocusable(tbox, 1);

			/*
			* Load the contents of this file into a buffer. Make
			* the buffer a bit larger so the user can try
			* entering text.
			*/
			if ((f = fopen("themes.c", "r")) != NULL) {
				size_t rv;

				fseek(f, 0, SEEK_END);
				size = ftell(f);
				fseek(f, 0, SEEK_SET);
				bufSize = size + 1024;
				someText = (char*)AG_Malloc(bufSize);
				rv = fread(someText, size, 1, f);
				fclose(f);
				someText[size] = '\0';
			}
			else {
				someText = AG_Strdup("Failed to load "
					"themes.c");
			}

			/*
			* Bind the buffer's contents to the Textbox. The
			* size argument to AG_TextboxBindUTF8() must include
			* space for the terminating NUL.
			*/
			AG_TextboxBindUTF8(tbox, someText, bufSize);
			AG_TextboxSetCursorPos(tbox, 0);
		}

		ntab = AG_NotebookAddTab(nb, "Empty tab", AG_BOX_VERT);
	}

	/* Override default window sizing. */
	AG_WindowSetGeometryAlignedPct(win, AG_WINDOW_MC, 80, 70);

	AG_WindowShow(win);
}