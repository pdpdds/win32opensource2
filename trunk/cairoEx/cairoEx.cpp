// cairoEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <cairo.h>
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
#pragma comment(lib, "cairo.lib")
#pragma comment(lib, "atk-1.0.lib")

static void do_drawing(cairo_t *);

int points[11][2] = {
	{ 0, 85 },
	{ 75, 75 },
	{ 100, 10 },
	{ 125, 75 },
	{ 200, 85 },
	{ 150, 125 },
	{ 160, 190 },
	{ 100, 150 },
	{ 40, 190 },
	{ 50, 125 },
	{ 0, 85 }
};

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr,
	gpointer user_data)
{
	do_drawing(cr);

	return FALSE;
}

static void do_drawing(cairo_t *cr)
{
	cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
	cairo_set_line_width(cr, 1);

	gint i;
	for (i = 0; i < 10; i++) {
		cairo_line_to(cr, points[i][0], points[i][1]);
	}

	cairo_close_path(cr);
	cairo_stroke_preserve(cr);
	cairo_fill(cr);

	cairo_move_to(cr, 240, 40);
	cairo_line_to(cr, 240, 160);
	cairo_line_to(cr, 350, 160);
	cairo_close_path(cr);

	cairo_stroke_preserve(cr);
	cairo_fill(cr);

	cairo_move_to(cr, 380, 40);
	cairo_line_to(cr, 380, 160);
	cairo_line_to(cr, 450, 160);
	cairo_curve_to(cr, 440, 155, 380, 145, 380, 40);

	cairo_stroke_preserve(cr);
	cairo_fill(cr);
}

int main(int argc, char *argv[])
{
	GtkWidget *window;
	GtkWidget *darea;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	darea = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(window), darea);

	g_signal_connect(G_OBJECT(darea), "draw",
		G_CALLBACK(on_draw_event), NULL);
	g_signal_connect(window, "destroy",
		G_CALLBACK(gtk_main_quit), NULL);

	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 460, 240);
	gtk_window_set_title(GTK_WINDOW(window), "Other shapes");

	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}

