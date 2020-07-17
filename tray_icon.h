
#ifndef LENA_TRAY_ICON_H
#define LENA_TRAY_ICON_H

#include <gtkmm.h>

class TrayIcon : Gtk::Menu {
public:
	explicit TrayIcon(const Glib::RefPtr<Gtk::Application>& app, MainWindow &window);

	void display();
private:
	Gtk::MenuItem item_open_main_window, item_quit;
};

#endif //LENA_TRAY_ICON_H
