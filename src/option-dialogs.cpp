#include <cctype>
#include <fstream>
#include <sstream>

#pragma warning(push, 0)
#include <FL/Enumerations.H>
#include <FL/Fl_Double_Window.H>
#pragma warning(pop)

#include "config.h"
#include "themes.h"
#include "widgets.h"
#include "utils.h"
#include "option-dialogs.h"

Option_Dialog::Option_Dialog(int w, const char *t) : _width(w), _title(t), _canceled(false),
	_dialog(NULL), _content(NULL), _ok_button(NULL), _cancel_button(NULL) {}

Option_Dialog::~Option_Dialog() {
	delete _dialog;
	delete _content;
	delete _ok_button;
	delete _cancel_button;
}

void Option_Dialog::initialize() {
	if (_dialog) { return; }
	Fl_Group *prev_current = Fl_Group::current();
	Fl_Group::current(NULL);
	// Populate dialog
	_dialog = new Fl_Double_Window(0, 0, 0, 0, _title);
	_content = new Fl_Group(0, 0, 0, 0);
	_content->begin();
	initialize_content();
	_content->end();
	_dialog->begin();
	_ok_button = new Default_Button(0, 0, 0, 0, "OK");
	_cancel_button = new OS_Button(0, 0, 0, 0, "Cancel");
	_dialog->end();
	// Initialize dialog
	_dialog->resizable(NULL);
	_dialog->callback((Fl_Callback *)cancel_cb, this);
	_dialog->set_modal();
	// Initialize dialog's children
	_ok_button->tooltip("OK (Enter)");
	_ok_button->callback((Fl_Callback *)close_cb, this);
	_cancel_button->shortcut(FL_Escape);
	_cancel_button->tooltip("Cancel (Esc)");
	_cancel_button->callback((Fl_Callback *)cancel_cb, this);
	Fl_Group::current(prev_current);
}

void Option_Dialog::refresh() {
	_canceled = false;
	_dialog->copy_label(_title);
	// Refresh widget positions and sizes
	int dy = 10;
	dy += refresh_content(_width - 20, dy) + 16;
#ifdef _WIN32
	_ok_button->resize(_width - 184, dy, 80, 22);
	_cancel_button->resize(_width - 90, dy, 80, 22);
#else
	_cancel_button->resize(_width - 184, dy, 80, 22);
	_ok_button->resize(_width - 90, dy, 80, 22);
#endif
	dy += _cancel_button->h() + 10;
	_dialog->size_range(_width, dy, _width, dy);
	_dialog->size(_width, dy);
	_dialog->redraw();
}

void Option_Dialog::show(const Fl_Widget *p) {
	initialize();
	refresh();
	int x = p->x() + (p->w() - _dialog->w()) / 2;
	int y = p->y() + (p->h() - _dialog->h()) / 2;
	_dialog->position(x, y);
	_ok_button->take_focus();
	_dialog->show();
	while (_dialog->shown()) { Fl::wait(); }
}

void Option_Dialog::close_cb(Fl_Widget *, Option_Dialog *od) {
	od->_dialog->hide();
}

void Option_Dialog::cancel_cb(Fl_Widget *, Option_Dialog *od) {
	od->_canceled = true;
	od->_dialog->hide();
}

Map_Options_Dialog::Map_Options_Dialog(const char *t) : Option_Dialog(290, t), _max_tileset_name_length(0),
	_map_width(NULL), _map_height(NULL), _tileset(NULL), _lighting(NULL), _skip_60_7f(NULL) {}

Map_Options_Dialog::~Map_Options_Dialog() {
	delete _map_width;
	delete _map_height;
	delete _tileset;
	delete _lighting;
	delete _skip_60_7f;
}

inline static bool isupperordigit(int c) { return isupper(c) || isdigit(c); }
inline static int toconstant(int c) { return isalnum(c) ? toupper(c) : '_'; }

static void guess_map_constant(const char *name, char *constant) {
	char prev = (char)toconstant(*name);
	*constant++ = (char)toupper(*name++);
	size_t n = strlen(name) - 4; // ignore ".blk" extension
	for (int i = 0; i < n; i++) {
		char c = *name;
		if ((islower(prev) && isupperordigit(c)) || // ...zA... -> ...Z_A...
			(i < n - 1 && isupperordigit(prev) && isupper(c) && islower(*(name+1)))) { // ...ZAb... -> ...Z_AB...
			*constant++ = '_';
		}
		prev = c;
		*constant++ = (char)toconstant(*name++);
	}
	*constant++ = ',';
	*constant = '\0';
}

bool Map_Options_Dialog::guess_map_size(const char *filename, const char *directory) {
	char map_constants[FL_PATH_MAX] = {};
	map_constants_path(map_constants, directory);

	std::ifstream ifs(map_constants);
	if (!ifs.good()) { return false; }

	const char *name = fl_filename_name(filename);
	char constant[FL_PATH_MAX] = {};
	guess_map_constant(name, constant);

	while (ifs.good()) {
		std::string line;
		std::getline(ifs, line);
		trim(line);
		if (!starts_with(line, "mapgroup ")) { continue; }
		line.erase(0, 9); // remove leading "mapgroup "
		if (!starts_with(line, constant)) { continue; }
		line.erase(0, strlen(constant));
		std::istringstream lss(line);
		int w, h;
		char comma;
		lss >> h >> comma >> w;
		if (1 <= w && w <= 255 && 1 <= h && h <= 255) {
			_map_width->value(w);
			_map_height->value(h);
			return true;
		}
		return false;
	}

	return false;
}

bool Map_Options_Dialog::limit_blk_options(const char *filename, const char *directory) {
	initialize();

	// Initialize map size
	if (!guess_map_size(filename, directory)) {
		_map_width->value(1);
		_map_height->value(1);
	}

	// Initialize tileset choices

	char tileset_directory[FL_PATH_MAX] = {};
	strcpy(tileset_directory, directory);
	strcat(tileset_directory, gfx_tileset_dir());

	dirent **list;
	int n = fl_filename_list(tileset_directory, &list);
	if (n < 0) { return false; }

	_max_tileset_name_length = 0;
	_tileset->clear();

	for (int i = 0; i < n; i++) {
		const char *name = list[i]->d_name;
		if (ends_with(name, "_colored.png")) { continue; } // ignore utils/metatiles.py renders
		else if (ends_with(name, ".2bpp.lz")) {
			std::string v(name);
			v.erase(v.size() - 8, 8);
			_tileset->add(v.c_str());
			int m = text_width(v.c_str(), 6);
			_max_tileset_name_length = MAX(m, _max_tileset_name_length);
		}
		else if (ends_with(name, ".png")) {
			std::string v(name);
			v.erase(v.size() - 4, 4);
			if (_tileset->find_index(v.c_str()) == -1) {
				_tileset->add(v.c_str());
				int m = text_width(v.c_str(), 6);
				_max_tileset_name_length = MAX(m, _max_tileset_name_length);
			}
		}
	}
	_tileset->value(0);

	return true;
}

void Map_Options_Dialog::initialize_content() {
	// Populate content group
	_map_width = new OS_Spinner(0, 0, 0, 0, "Width:");
	_map_height = new OS_Spinner(0, 0, 0, 0, "Height:");
	_tileset = new Dropdown(0, 0, 0, 0, "Tileset:");
	_lighting = new Dropdown(0, 0, 0, 0, "Lighting:");
	_skip_60_7f = new OS_Check_Button(0, 0, 0, 0, "Skip tile IDs $60-$7F (pokecrystal)");
	// Initialize content group's children
	_map_width->align(FL_ALIGN_LEFT);
	_map_width->range(1, 255);
	_map_height->align(FL_ALIGN_LEFT);
	_map_height->range(1, 255);
	_tileset->align(FL_ALIGN_LEFT);
	_lighting->align(FL_ALIGN_LEFT);
	_lighting->add("Day"); // DAY
	_lighting->add("Nite"); // NITE
	_lighting->add("Indoor"); // INDOOR
	int lighting;
	global_config.get("map-lighting", lighting, (int)Tileset::Lighting::DAY);
	_lighting->value((Tileset::Lighting)lighting);
	int skip;
	global_config.get("map-skip", skip, 0);
	_skip_60_7f->value(skip);
}

int Map_Options_Dialog::refresh_content(int ww, int dy) {
	int wgt_w = 0, wgt_h = 22, win_m = 10, wgt_m = 4;
	int ch = (wgt_h + wgt_m) * 3 + wgt_h;
	_content->resize(win_m, dy, ww, ch);

	int wgt_off = win_m + MAX(MAX(text_width(_map_width->label(), 2), text_width(_tileset->label(), 2)), text_width(_lighting->label(), 2));

	wgt_w = text_width("999", 2) + wgt_h;
	_map_width->resize(wgt_off, dy, wgt_w, wgt_h);
	_map_height->resize(_map_width->x() + _map_width->w() + 10 + text_width("Height:"), dy, wgt_w, wgt_h);
	dy += _map_height->h() + wgt_m;

	wgt_w = _max_tileset_name_length + wgt_h;
	_tileset->resize(wgt_off, dy, wgt_w, wgt_h);
	dy += _tileset->h() + wgt_m;

	wgt_w = text_width(_lighting->label()) + MAX(MAX(text_width("Day", 2), text_width("Nite", 2)), text_width("Indoor", 2));
	_lighting->resize(wgt_off, dy, wgt_w, wgt_h);
	dy += _tileset->h() + wgt_m;

	wgt_w = text_width("Skip tile IDs $60-$7F (pokecrystal)", 2) + wgt_h;
	_skip_60_7f->resize(wgt_off, dy, wgt_w, wgt_h);

	return ch;
}

Resize_Dialog::Resize_Dialog(const char *t) : Option_Dialog(220, t), _map_width(NULL), _map_height(NULL),
	_anchor_top_left(NULL), _anchor_top_center(NULL), _anchor_top_right(NULL),
	_anchor_middle_left(NULL), _anchor_middle_center(NULL), _anchor_middle_right(NULL),
	_anchor_bottom_left(NULL), _anchor_bottom_center(NULL), _anchor_bottom_right(NULL) {}

Resize_Dialog::~Resize_Dialog() {
	delete _map_width;
	delete _map_height;
	delete _anchor_top_left;
	delete _anchor_top_center;
	delete _anchor_top_right;
	delete _anchor_middle_left;
	delete _anchor_middle_center;
	delete _anchor_middle_right;
	delete _anchor_bottom_left;
	delete _anchor_bottom_center;
	delete _anchor_bottom_right;
}

Resize_Dialog::Hor_Align Resize_Dialog::horizontal_anchor() const {
	if (_anchor_top_left->value() || _anchor_middle_left->value() || _anchor_bottom_left->value()) { return LEFT; }
	if (_anchor_top_right->value() || _anchor_middle_right->value() || _anchor_bottom_right->value()) { return RIGHT; }
	return CENTER;
}

Resize_Dialog::Vert_Align Resize_Dialog::vertical_anchor() const {
	if (_anchor_top_left->value() || _anchor_top_center->value() || _anchor_top_right->value()) { return TOP; }
	if (_anchor_bottom_left->value() || _anchor_bottom_center->value() || _anchor_bottom_right->value()) { return BOTTOM; }
	return MIDDLE;
}

int Resize_Dialog::anchor() const {
	if (_anchor_top_left->value())      { return 0; }
	if (_anchor_top_center->value())    { return 1; }
	if (_anchor_top_right->value())     { return 2; }
	if (_anchor_middle_left->value())   { return 3; }
	if (_anchor_middle_center->value()) { return 4; }
	if (_anchor_middle_right->value())  { return 5; }
	if (_anchor_bottom_left->value())   { return 6; }
	if (_anchor_bottom_center->value()) { return 7; }
	if (_anchor_bottom_right->value())  { return 8; }
	return 4;
}

void Resize_Dialog::anchor(int a) {
	switch (a) {
	case 0: _anchor_top_left->setonly(); return;
	case 1: _anchor_top_center->setonly(); return;
	case 2: _anchor_top_right->setonly(); return;
	case 3: _anchor_middle_left->setonly(); return;
	case 4: default: _anchor_middle_center->setonly(); return;
	case 5: _anchor_middle_right->setonly(); return;
	case 6: _anchor_bottom_left->setonly(); return;
	case 7: _anchor_bottom_center->setonly(); return;
	case 8: _anchor_bottom_right->setonly(); return;
	}
}

void Resize_Dialog::initialize_content() {
	// Populate content group
	_map_width = new OS_Spinner(0, 0, 0, 0, "Width:");
	_map_height = new OS_Spinner(0, 0, 0, 0, "Height:");
	_anchor_top_left = new OS_Button(0, 0, 0, 0, "@7>");
	_anchor_top_center = new OS_Button(0, 0, 0, 0, "@8>");
	_anchor_top_right = new OS_Button(0, 0, 0, 0, "@9>");
	_anchor_middle_left = new OS_Button(0, 0, 0, 0, "@4>");
	_anchor_middle_center = new OS_Button(0, 0, 0, 0, "@-2square");
	_anchor_middle_right = new OS_Button(0, 0, 0, 0, "@>");
	_anchor_bottom_left = new OS_Button(0, 0, 0, 0, "@1>");
	_anchor_bottom_center = new OS_Button(0, 0, 0, 0, "@2>");
	_anchor_bottom_right = new OS_Button(0, 0, 0, 0, "@3>");
	// Initialize content group's children
	_map_width->align(FL_ALIGN_LEFT);
	_map_width->range(1, 255);
	_map_height->align(FL_ALIGN_LEFT);
	_map_height->range(1, 255);
	_anchor_top_left->type(FL_RADIO_BUTTON);
	_anchor_top_center->type(FL_RADIO_BUTTON);
	_anchor_top_right->type(FL_RADIO_BUTTON);
	_anchor_middle_left->type(FL_RADIO_BUTTON);
	_anchor_middle_center->type(FL_RADIO_BUTTON);
	_anchor_middle_right->type(FL_RADIO_BUTTON);
	_anchor_bottom_left->type(FL_RADIO_BUTTON);
	_anchor_bottom_center->type(FL_RADIO_BUTTON);
	_anchor_bottom_right->type(FL_RADIO_BUTTON);
	_anchor_top_left->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE | FL_ALIGN_CLIP);
	_anchor_top_center->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE | FL_ALIGN_CLIP);
	_anchor_top_right->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE | FL_ALIGN_CLIP);
	_anchor_middle_left->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE | FL_ALIGN_CLIP);
	_anchor_middle_center->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE | FL_ALIGN_CLIP);
	_anchor_middle_right->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE | FL_ALIGN_CLIP);
	_anchor_bottom_left->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE | FL_ALIGN_CLIP);
	_anchor_bottom_center->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE | FL_ALIGN_CLIP);
	_anchor_bottom_right->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE | FL_ALIGN_CLIP);
	int anchor_config;
	global_config.get("resize-anchor", anchor_config, 4);
	anchor(anchor_config);
}

int Resize_Dialog::refresh_content(int ww, int dy) {
	int wgt_w = 0, wgt_h = 22, win_m = 10, wgt_m = 4;
	int ch = (wgt_h + wgt_m) * 2 + wgt_h;
	_content->resize(win_m, dy, ww, ch);

	int wgt_off = win_m + MAX(text_width(_map_width->label(), 2), text_width(_map_height->label(), 2));

	wgt_w = text_width("999", 2) + wgt_h;
	_map_width->resize(wgt_off, dy, wgt_w, wgt_h);
	_map_height->resize(wgt_off, _map_width->y() + _map_width->h() + wgt_m, wgt_w, wgt_h);
	wgt_off += wgt_w + 20;

	wgt_w = wgt_h = 24;
	wgt_m = 2;
	_anchor_top_left->resize(wgt_off, dy, wgt_w, wgt_h);
	_anchor_top_center->resize(wgt_off + wgt_w + wgt_m, dy, wgt_w, wgt_h);
	_anchor_top_right->resize(wgt_off + (wgt_w + wgt_m) * 2, dy, wgt_w, wgt_h);
	dy +=  + wgt_h + wgt_m;
	_anchor_middle_left->resize(wgt_off, dy, wgt_w, wgt_h);
	_anchor_middle_center->resize(wgt_off + wgt_w + wgt_m, dy, wgt_w, wgt_h);
	_anchor_middle_right->resize(wgt_off + (wgt_w + wgt_m) * 2, dy, wgt_w, wgt_h);
	dy +=  + wgt_h + wgt_m;
	_anchor_bottom_left->resize(wgt_off, dy, wgt_w, wgt_h);
	_anchor_bottom_center->resize(wgt_off + wgt_w + wgt_m, dy, wgt_w, wgt_h);
	_anchor_bottom_right->resize(wgt_off + (wgt_w + wgt_m) * 2, dy, wgt_w, wgt_h);

	return ch;
}

Add_Sub_Dialog::Add_Sub_Dialog(const char *t) : Option_Dialog(194, t), _num_metatiles(NULL) {}

Add_Sub_Dialog::~Add_Sub_Dialog() {
	delete _num_metatiles;
}

void Add_Sub_Dialog::initialize_content() {
	// Populate content group
	_num_metatiles = new OS_Spinner(0, 0, 0, 0, "Blocks:");
	// Initialize content group's children
	_num_metatiles->align(FL_ALIGN_LEFT);
	_num_metatiles->range(1, 256);
}

int Add_Sub_Dialog::refresh_content(int ww, int dy) {
	int wgt_h = 22, win_m = 10;
	_content->resize(win_m, dy, ww, wgt_h);

	int wgt_off = win_m + text_width(_num_metatiles->label(), 2);
	int wgt_w = text_width("999", 2) + wgt_h;
	_num_metatiles->resize(wgt_off, dy, wgt_w, wgt_h);

	return wgt_h;
}
