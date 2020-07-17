
#ifndef LENA_MAIN_WINDOW_H
#define LENA_MAIN_WINDOW_H

#include <gtkmm.h>

#include "lena_client.h"
#include "log.h"

class MainWindow : public Gtk::Window {
public:
	MainWindow();

private:
	Gtk::Box settings_box, name_box, work_dir_box, work_dir_button_box;
	Gtk::Label name_label, work_dir_label, work_dir_button_label, info_label;
	Gtk::Entry name_entry;
	Gtk::Button work_dir_button, save_button;
	Gtk::Image work_dir_button_icon;
	Gtk::Notebook notebook;
	Glib::RefPtr<Gio::File> current_work_dir;
	LenaClient client;

	void on_work_dir_button_clicked();
};

#endif //LENA_MAIN_WINDOW_H
