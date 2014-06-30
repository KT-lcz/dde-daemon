//gcc theme_preview.c `pkg-config --libs --cflags gtk+-2.0  libmetacity-private `
//
/**
 * Copyright (c) 2011 ~ 2014 Deepin, Inc.
 *               2013 ~ 2014 jouyouyun
 *
 * Author:      jouyouyun <jouyouwen717@gmail.com>
 * Maintainer:  jouyouyun <jouyouwen717@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 **/


#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <metacity-private/common.h>
#include <metacity-private/util.h>
#include <metacity-private/boxes.h>
#include <metacity-private/gradient.h>
#include <metacity-private/theme-parser.h>
#include <metacity-private/theme.h>
#include <metacity-private/gradient.h>
#include <metacity-private/preview-widget.h>

typedef struct _UserData {
	char *dest;
	char *bg;
} UserData;


#define THUMB_WIDTH 128
#define THUMB_HEIGHT 72
GtkWidget *get_meta_theme (const char* theme_name)
{
	GError *error = NULL;
	MetaTheme *meta = meta_theme_load(theme_name, &error);
	if (!meta) {
		g_warning("Get Current Meta Theme Failed:%s", error->message);
		g_error_free(error);
		return NULL;
	}

	GtkWidget *preview =  meta_preview_new();
	if (!preview) {
		g_warning("Get Meta Preview Failed");
		return NULL;
	}

	meta_preview_set_theme((MetaPreview*)preview, meta);
	meta_preview_set_title((MetaPreview*)preview, "Test Meta Title");

	return preview;
}

void capture(GtkOffscreenWindow* w, GdkEvent* event, gpointer user_data)
{
	UserData *datas = (UserData*)user_data;
	char *dest = datas->dest;
	char *bg = datas->bg;

	cairo_surface_t *target = cairo_image_surface_create_from_png(bg);
	cairo_t *cairo = cairo_create(target);
	GdkPixbuf* pbuf = gtk_offscreen_window_get_pixbuf(w);

	gdk_cairo_set_source_pixbuf (cairo, pbuf, -15, 15);
	cairo_paint(cairo);
	cairo_surface_write_to_png (target, dest);
	printf("Saaveto:%s", dest);

	g_object_unref(G_OBJECT(pbuf));
	cairo_destroy(cairo);
	cairo_surface_destroy(target);

	gtk_main_quit();
}

void padding_thumbnail(GtkFixed* thumb)
{
    //TODO: Should handle gtk2/gtk3 themes. 
    /*GtkWidget *btn = gtk_button_new_with_label("      ");*/

    /*gtk_fixed_put(thumb, btn, 72, 5);*/

    /*btn = gtk_radio_button_new(NULL);*/
    /*gtk_fixed_put(thumb, btn, 5, 5);*/

    /*btn = gtk_check_button_new();*/
    /*gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(btn), TRUE);*/
    /*gtk_fixed_put(thumb, btn, 35, 5);*/
}

int gen_gtk_thumbnail(char *theme, char *dest, char *bg)
{
	if (theme == NULL || dest == NULL || bg == NULL) {
		g_warning("gen_gtk_thumbnail args error");
		return -1;
	}

	GtkWidget* w = gtk_offscreen_window_new();
	gtk_widget_set_size_request(w, 128, 72);
	GtkWidget *t = get_meta_theme(theme);
	if (t == NULL) {
		g_warning("Get Meta Theme Failed");
		return -1;
	}
	gtk_container_add((GtkContainer*)w,t);
	GtkWidget* fixed = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(t), fixed);

	padding_thumbnail(GTK_FIXED(fixed));

	UserData user_data;
	user_data.dest = dest;
	user_data.bg = bg;
	g_signal_connect(G_OBJECT(w), "damage-event", 
			G_CALLBACK(capture), &user_data);
	gtk_widget_show_all(w);
	gtk_main();

	return 0;
}

int try_init()
{
    return gtk_init_check(NULL, NULL);
}
