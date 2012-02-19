/*
 * Geanyuniq - A geany plugin to delete duplicate lines.
 *
 * Copyright (C) 2012 by Emil Brink <emil@obsession.se>.
 *
 * This file is part of geany-uniq.
 *
 * Geanyuniq is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Geanyuniq is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Geanyuniq.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string.h>

#include <gdk/gdkkeysyms.h>

#include "geanyplugin.h"

#define	MNEMONIC_NAME			"geanyuniq"

#define	MENU_EDIT_LABEL		"_Edit"
#define	MENU_DUPLICATE_LABEL	"_Duplicate Line or Selection"

GeanyPlugin         *geany_plugin;
GeanyData           *geany_data;
GeanyFunctions      *geany_functions;

PLUGIN_VERSION_CHECK(147)

PLUGIN_SET_INFO("Geanyuniq",
		"A plugin to automatically delete duplicate lines in the entire document, named after the \"uniq\" shell command.",
		"1.0",
		"Emil Brink <emil@obsession.se>")

enum {
	KEY_GEANY_UNIQ,
	NUM_KEYS
};

static struct {
	GtkWidget	*menu_item;
	GeanyKeyGroup	*key_group;
} geany_uniq;

/* -------------------------------------------------------------------------------------------------------------- */

/* Remove duplicate lines, leaving the first line. */
static void run_geany_uniq(void)
{
	GeanyDocument	*doc;
	ScintillaObject	*sci;
	GString		*prev;

	if((doc = document_get_current()) == NULL)
		return;
	if((sci = doc->editor->sci) == NULL)
		return;
	if((prev = g_string_sized_new(512)) != NULL)
	{
		gint	index = 0, pos;
		gchar	*here;
		guint	count = 0;

		while((here = sci_get_line(sci, index)) != NULL && *here != '\0')
		{
			const gboolean	skip = (index == 0) || strcmp(here, prev->str) != 0;

			if(skip)
			{
				g_string_assign(prev, here);
				index++;
			}
			else
			{
				/* Line is identical to the previous, delete it. */
				pos = sci_get_position_from_line(sci, index);
				sci_set_selection_start(sci, pos);
				sci_set_selection_end(sci, pos + sci_get_line_length(sci, index));
				sci_replace_sel(sci, "");
				count++;
			}
			g_free(here);
		}
		g_string_free(prev, TRUE);
		if(count > 0)
			msgwin_status_add("Geanyuniq deleted %u duplicate lines.", count);
	}
}

/* -------------------------------------------------------------------------------------------------------------- */

/* The UTF-8 equivalent of { while(*s == codepoint) s++; }. */
static const gchar * utf8_skip(const gchar *s, const gunichar codepoint)
{
	while(g_utf8_get_char(s) == codepoint)
	{
		s = g_utf8_next_char(s);
	}
	return s;
}

/* Compares two label texts for equality, ignoring any embedded underscores. Texts are assumed to be in valid UTF-8. */
static gboolean utf8_labels_equal(const gchar *label1, const gchar *label2)
{
	while(*label1 && *label2)
	{
		label1 = utf8_skip(label1, '_');
		label2 = utf8_skip(label2, '_');
		if(g_utf8_get_char(label1) != g_utf8_get_char(label2))
			return FALSE;
		label1 = g_utf8_next_char(label1);
		label2 = g_utf8_next_char(label2);
	}
	return *label1 == *label2;
}

/* Helper function to walk a GtkMenu and look for an item with the given label. */
GtkWidget * find_menu_by_label(GtkWidget *menu, const gchar *find_label, gint *pos)
{
	GtkWidget	*item = NULL;
	GList		*children, *iter;
	gint		index = 0;

	for(iter = children = gtk_container_get_children(GTK_CONTAINER(menu)); iter != NULL && item == NULL; iter = g_list_next(iter), index++)
	{
		GtkWidget	*child = iter->data;
		const gchar	*label = gtk_menu_item_get_label(GTK_MENU_ITEM(child));

		if(utf8_labels_equal(label, find_label))
		{
			item = child;
			if(pos != NULL)
				*pos = index;
		}
	}
	g_list_free(children);

	return item;
}

/* This gets called when Geany's "Edit" menu is mapped. Try to find submenu, and then attach our item there.
 * Slightly more involved than just the plain old "append to Tools menu" approach, but also way better results.
 *
 * Might break horribly when running a localized Geany.
*/
static void cb_edit_menu_mapped(GtkWidget *widget, gpointer user)
{
	GtkWidget	*sub = gtk_menu_item_get_submenu(GTK_MENU_ITEM(widget));

	if(sub != NULL)
	{
		GtkWidget	*dup;
		gint		pos;

		/* In the submenu we just got, look for a known entry. Semi-hackish. */
		if((dup = find_menu_by_label(sub, _(MENU_DUPLICATE_LABEL), &pos)) != NULL)
		{
			gtk_menu_shell_insert(GTK_MENU_SHELL(sub), user, pos + 1);
		}
		else
		{
			/* As fall-back, append to the submenu directly. */
			gtk_container_add(GTK_CONTAINER(sub), user);
		}
	}
}

/* -------------------------------------------------------------------------------------------------------------- */

static gboolean cb_key_group_callback(guint key_id)
{
	switch(key_id)
	{
	case KEY_GEANY_UNIQ:
		run_geany_uniq();
		return TRUE;
	}
	return FALSE;
}

/* -------------------------------------------------------------------------------------------------------------- */

void plugin_init(GeanyData *geany_data)
{
	GtkWidget	*edit;

	/* This plugin implements a single command, coupled with a menu item. */
	geany_uniq.menu_item = ui_image_menu_item_new(NULL, _("Delete Duplicate Lines"));
	gtk_widget_show_all(geany_uniq.menu_item);

	/* Try and sneak the menu item into where it works best, in the Edit->Commands menu. */
	if((edit = find_menu_by_label(geany->main_widgets->editor_menu, _(MENU_EDIT_LABEL), NULL)) != NULL)
		g_signal_connect(G_OBJECT(edit), "map", G_CALLBACK(cb_edit_menu_mapped), geany_uniq.menu_item);
	else
	{
		/* Fall-back: append to the good old Tools menu. */
		gtk_container_add(GTK_CONTAINER(geany->main_widgets->tools_menu), geany_uniq.menu_item);
	}

	geany_uniq.key_group = plugin_set_key_group(geany_plugin, MNEMONIC_NAME, NUM_KEYS, cb_key_group_callback);
	keybindings_set_item(geany_uniq.key_group, KEY_GEANY_UNIQ, NULL, GDK_KEY_d, GDK_CONTROL_MASK | GDK_SHIFT_MASK, "geany-uniq-uniq", _("Remove Duplicate Lines"), geany_uniq.menu_item);
}

void plugin_cleanup(void)
{
	gtk_widget_destroy(geany_uniq.menu_item);
}
