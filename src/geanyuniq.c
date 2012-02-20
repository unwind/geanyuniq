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

#define	MNEMONIC_NAME	"geanyuniq"

GeanyPlugin         *geany_plugin;
GeanyData           *geany_data;
GeanyFunctions      *geany_functions;

PLUGIN_VERSION_CHECK(147)

PLUGIN_SET_INFO("Geanyuniq",
		"A plugin to automatically delete duplicate lines in the entire document, named after the \"uniq\" shell command.",
		"1.1",
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

/* Remove duplicate lines from the entire document. */
static guint geany_uniq_document(ScintillaObject *sci, GString *prev)
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
	g_free(here);

	return count;
}

/* -------------------------------------------------------------------------------------------------------------- */

static guint geany_uniq_selection(ScintillaObject *sci, GString *prev)
{
	const gint	sel_start = sci_get_selection_start(sci);
	const gint	sel_end = sci_get_selection_end(sci);
	gint		sel_line_start, sel_line_end;

	if(sci_get_col_from_position(sci, sel_start) != 0 || sci_get_col_from_position(sci, sel_end) != 0)
		return 0;

	sel_line_start = sci_get_line_from_position(sci, sel_start);
	sel_line_end = sci_get_line_from_position(sci, sel_end);

	return 0;
}

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
		guint	count;

		sci_start_undo_action(sci);
		if(!sci_has_selection(sci))
			count = geany_uniq_document(sci, prev);
		else
			count = geany_uniq_selection(sci, prev);
		sci_end_undo_action(sci);

		if(count > 0)
			msgwin_status_add("Geanyuniq deleted %u duplicate lines.", count);

		g_string_free(prev, TRUE);
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
	/* This plugin implements a single command, coupled with a menu item. */
	geany_uniq.menu_item = ui_image_menu_item_new(NULL, _("Delete Duplicate Lines"));
	gtk_widget_show_all(geany_uniq.menu_item);
	gtk_container_add(GTK_CONTAINER(geany->main_widgets->tools_menu), geany_uniq.menu_item);

	geany_uniq.key_group = plugin_set_key_group(geany_plugin, MNEMONIC_NAME, NUM_KEYS, cb_key_group_callback);
	keybindings_set_item(geany_uniq.key_group, KEY_GEANY_UNIQ, NULL, 0, 0, "geany-uniq-uniq", _("Delete Duplicate Lines"), geany_uniq.menu_item);
}

void plugin_cleanup(void)
{
	gtk_widget_destroy(geany_uniq.menu_item);
}
