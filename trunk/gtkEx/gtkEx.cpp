// gtkEx.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <gtk/gtk.h>

#pragma comment(lib, "gtk-win32-3.0.lib")

#pragma comment(lib, "gdk-win32-3.0.lib")
#pragma comment(lib, "pangoft2-1.0.lib")
#pragma comment(lib, "freetype253_d.lib")
#pragma comment(lib, "fontconfig.lib")
#pragma comment(lib, "pangowin32-1.0.lib")
#pragma comment(lib, "pango-1.0.lib")

#pragma comment(lib, "glib-2.0.lib")
#pragma comment(lib, "gobject-2.0.lib")
#pragma comment(lib, "gio-2.0.lib")
#pragma comment(lib, "gdk_pixbuf-2.0.lib")
#pragma comment(lib, "atk-1.0.lib")
   
int main(int   argc, char *argv[])
{
	GtkWidget *window;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_show(window);

	gtk_main();

	return(0);
}

