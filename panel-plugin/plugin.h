/*
 * Copyright (C) 2013 Graeme Gott <graeme@gottcode.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WHISKERMENU_PLUGIN_H
#define WHISKERMENU_PLUGIN_H

#include <string>

#include <gtk/gtk.h>
#include <libxfce4panel/libxfce4panel.h>

namespace WhiskerMenu
{

class Window;

class Plugin
{
public:
	explicit Plugin(XfcePanelPlugin* plugin);
	~Plugin();

	GtkWidget* get_button() const
	{
		return m_button;
	}

	enum ButtonStyle
	{
		ShowIcon = 0x1,
		ShowText = 0x2,
		ShowIconAndText = ShowIcon | ShowText
	};

	ButtonStyle get_button_style() const;
	std::string get_button_title() const;
	static std::string get_button_title_default();
	std::string get_button_icon_name() const;

	void reload();
	void set_button_style(ButtonStyle style);
	void set_button_title(const std::string& title);
	void set_button_icon_name(const std::string& icon);
	void set_configure_enabled(bool enabled);

private:
	bool button_clicked(GdkEventButton* event);
	void menu_hidden();
	void configure();
	bool remote_event(gchar* name, GValue* value);
	void save();
	bool size_changed(int size);
	void orientation_changed(bool vertical);
	void popup_menu(bool at_cursor);

private:
	XfcePanelPlugin* m_plugin;
	Window* m_window;

	GtkWidget* m_button;
	GtkBox* m_button_box;
	GtkLabel* m_button_label;
	XfcePanelImage* m_button_icon;


private:
	static gboolean button_clicked_slot(GtkWidget*, GdkEventButton* event, Plugin* obj)
	{
		return obj->button_clicked(event);
	}

	static void menu_hidden_slot(GtkWidget*, Plugin* obj)
	{
		obj->menu_hidden();
	}

	static void configure_slot(XfcePanelPlugin*, Plugin* obj)
	{
		obj->configure();
	}

	static gboolean remote_event_slot(XfcePanelPlugin*, gchar* name, GValue* value, Plugin* obj)
	{
		return obj->remote_event(name, value);
	}

	static void save_slot(Plugin* obj)
	{
		obj->save();
	}

	static gboolean size_changed_slot(XfcePanelPlugin*, gint size, Plugin* obj)
	{
		return obj->size_changed(size);
	}

#if (LIBXFCE4PANEL_CHECK_VERSION(4,9,0))
	static void mode_changed_slot(XfcePanelPlugin*, XfcePanelPluginMode mode, Plugin* obj)
	{
		obj->orientation_changed(mode == XFCE_PANEL_PLUGIN_MODE_VERTICAL);
	}
#else
	static void orientation_changed_slot(XfcePanelPlugin*, GtkOrientation orientation, Plugin* obj)
	{
		obj->orientation_changed(orientation == GTK_ORIENTATION_VERTICAL);
	}
#endif
};

}

#endif // WHISKERMENU_PLUGIN_H
