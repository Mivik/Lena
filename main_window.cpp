
#include "main_window.h"
#include "common.h"

void MainWindow::on_work_dir_button_clicked() {
	Gtk::FileChooserDialog dialog("选择工作目录", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
	dialog.set_deletable(false);
	dialog.add_button("取消", Gtk::RESPONSE_CANCEL);
	dialog.add_button("选择", Gtk::RESPONSE_OK);
	if (dialog.run() == Gtk::RESPONSE_OK) {
		current_work_dir = dialog.get_file();
		work_dir_button_label.set_text(current_work_dir->get_path());
	}
}

MainWindow::MainWindow()
		: name_label("名字"),
		  work_dir_label("工作目录"),
		  save_button("保存") {
	current_work_dir = Gio::File::create_for_path(Glib::get_home_dir());
	client.set_work_dir(current_work_dir);

	set_title("Lena");
	set_position(Gtk::WindowPosition::WIN_POS_CENTER);
	set_icon_name(Gtk::Stock::NETWORK.id);
	settings_box.set_orientation(Gtk::ORIENTATION_VERTICAL);
	settings_box.set_border_width(10);

	name_box.pack_start(name_label, Gtk::PACK_SHRINK);
	name_entry.set_margin_left(5);
	name_entry.set_text(client.get_user_name());
	name_box.pack_start(name_entry);
	settings_box.pack_start(name_box, Gtk::PACK_SHRINK);
	work_dir_box.set_margin_top(10);
	work_dir_box.pack_start(work_dir_label, Gtk::PACK_SHRINK);
	work_dir_button.set_margin_left(5);
	work_dir_button_icon.set_from_icon_name(Gtk::Stock::FILE.id, Gtk::ICON_SIZE_BUTTON);
	work_dir_button_box.add(work_dir_button_icon);
	work_dir_button_label.set_text(client.get_work_dir()->get_path());
	work_dir_button_label.set_margin_left(5);
	work_dir_button_label.set_ellipsize(Pango::ELLIPSIZE_START);
	work_dir_button_box.add(work_dir_button_label);
	work_dir_button.add(work_dir_button_box);
	work_dir_button.signal_clicked().connect([this] { on_work_dir_button_clicked(); });
	work_dir_box.pack_start(work_dir_button);
	settings_box.pack_start(work_dir_box, Gtk::PACK_SHRINK);
	save_button.set_margin_top(10);
	save_button.signal_clicked().connect([this] {
		auto name = name_entry.get_text();
		if (name.bytes() > sizeof(Cena::name_t)) {
			Gtk::MessageDialog("名字长度不得大于 " + std::to_string(sizeof(Cena::name_t)) + " 字节", false,
							   Gtk::MESSAGE_ERROR).run();
			return;
		}
		client.set_user_name(name);
		client.set_work_dir(current_work_dir);
	});
	settings_box.pack_start(save_button, Gtk::PACK_EXPAND_WIDGET);
	notebook.append_page(settings_box, "设置");

	info_label = Gtk::Label("Lena\n" LENA_VERSION "\nMade by Mivik");
	info_label.set_justify(Gtk::JUSTIFY_CENTER);
	notebook.append_page(info_label, "关于");

	add(notebook);
	show_all_children();

	signal_hide().connect([this] {
		name_entry.set_text(client.get_user_name());
		current_work_dir = client.get_work_dir();
		work_dir_button_label.set_text(current_work_dir->get_path());
	});
}