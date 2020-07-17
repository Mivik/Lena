
#include <gtkmm.h>

#include "main_window.h"
#include "tray_icon.h"

int main(int argc, char *argv[]) {
	auto app = Gtk::Application::create(argc, argv, "com.mivik.lena");
	app->set_flags(Gio::APPLICATION_IS_SERVICE);
	MainWindow window;
	TrayIcon icon(app, window);
	icon.display();
	app->signal_activate().connect([app, &window] { app->add_window(window); });
	app->hold();
	app->run();
	return 0;
}