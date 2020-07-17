
#include <libappindicator/app-indicator.h>

#include "main_window.h"
#include "tray_icon.h"

TrayIcon::TrayIcon(const Glib::RefPtr<Gtk::Application> &app, MainWindow &window) :
		item_open_main_window("显示"),
		item_quit("退出") {
	item_open_main_window.signal_activate().connect([&window] {
		if (window.is_visible()) window.hide();
		else window.show();
	});
	window.signal_show().connect([this] {
		item_open_main_window.set_label("隐藏");
	});
	window.signal_hide().connect([this] {
		item_open_main_window.set_label("显示");
	});
	item_quit.signal_activate().connect([app] {
		app->quit();
	});
	append(item_open_main_window);
	append(item_quit);
	show_all_children();
}

void TrayIcon::display() {
	AppIndicator *indicator = app_indicator_new("com.mivik.lena.appindicator", Gtk::Stock::NETWORK.id,
												APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
	app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
	app_indicator_set_menu(indicator, gobj());
}