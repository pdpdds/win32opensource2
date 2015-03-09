// gtkEx2.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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


static void print_hello(GtkWidget *widget, gpointer   data)
{
	g_print("Hello World\n");
}

int
main(int   argc,
char *argv[])
{
	GtkBuilder *builder;
	GObject *window;
	GObject *button;

	gtk_init(&argc, &argv);

	/* Construct a GtkBuilder instance and load our UI description */
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "builder.ui", NULL);

	/* Connect signal handlers to the constructed widgets. */
	window = gtk_builder_get_object(builder, "window");
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	button = gtk_builder_get_object(builder, "button1");
	g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

	button = gtk_builder_get_object(builder, "button2");
	g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

	button = gtk_builder_get_object(builder, "quit");
	g_signal_connect(button, "clicked", G_CALLBACK(gtk_main_quit), NULL);

	gtk_main();

	return 0;
}