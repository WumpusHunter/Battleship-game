//------------------------------------------------------------------------------

#include "GUI.h"

//------------------------------------------------------------------------------

namespace Graph_lib {

	//------------------------------------------------------------------------------

	// Attaches button to win
	void Button::attach(Graph_lib::Window& win)
	{
		// Construction of FLTK's widget
		pw = new Fl_Button{ loc.x, loc.y, static_cast<int>(width), static_cast<int>(height), label.c_str() };
		pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win);
		own = &win;		// Connection with window
	}

	//------------------------------------------------------------------------------

	// Gets integer from input box
	int In_box::get_int() const
	{
		static constexpr int fail_key = -999999;
		const Fl_Input& in_box = reference_to<Fl_Input>(pw);	// Reference to FLTK's widget
		const char* val = in_box.value();						// String value from FLTK's widget
		return isdigit(val[0]) ? atoi(val) : fail_key;			// Handle of non-numeric
	}

	// Gets string from input box
	std::string In_box::get_string() const
	{
		const Fl_Input& in_box = reference_to<Fl_Input>(pw);	// Reference to FLTK's widget
		return std::string{ in_box.value() };
	}

	// Attaches input box to win
	void In_box::attach(Graph_lib::Window& win)
	{
		// Construction of FLTK's widget
		pw = new Fl_Input{ loc.x, loc.y, static_cast<int>(width), static_cast<int>(height), label.c_str() };
		own = &win;		// Connection with window
	}

	//------------------------------------------------------------------------------

	// Puts num into output box
	void Out_box::put(int num)
	{
		Fl_Output& out_box = reference_to<Fl_Output>(pw);	// Reference to FLTK's widget
		std::string num_str = to_string(num);				// Convert num to string
		out_box.value(num_str.c_str());						// Put num into output box
	}

	// Puts str into output box
	void Out_box::put(const std::string& str)
	{
		Fl_Output& out_box = reference_to<Fl_Output>(pw);		// Reference to FLTK's widget
		out_box.value(str.c_str());
	}

	// Gets integer from output box
	int Out_box::get_int() const
	{
		static constexpr int fail_key = -999999;
		const Fl_Output& out_box = reference_to<Fl_Output>(pw);	// Reference to FLTK's widget
		const char* val = out_box.value();						// Value from output box
		return isdigit(val[0]) ? atoi(val) : fail_key;			// Handle of non-numeric
	}

	// Gets string from output box
	std::string Out_box::get_string() const
	{
		const Fl_Output& out_box = reference_to<Fl_Output>(pw);	// Reference to FLTK's widget
		return std::string{ out_box.value() };
	}

	// Attaches output box to win
	void Out_box::attach(Graph_lib::Window& win)
	{
		// Construction of FLTK's widget
		pw = new Fl_Output{ loc.x, loc.y, static_cast<int>(width), static_cast<int>(height), label.c_str() };
		own = &win;		// Connection with window
	}

	//------------------------------------------------------------------------------

	// Attaches but to menu; returns index of but in set of buttons
	unsigned int Menu::attach(Button& but)
	{
		init(but);
		selection.push_back(but);		// Add but to all buttons
		return selection.size() - 1;	// Index of but
	}

	// Attaches but to menu; returns index of but in set of buttons
	unsigned int Menu::attach(Button* but)
	{
		if (but == nullptr)		// Handle of NULL pointer
			throw std::invalid_argument("Bad Menu: can't attach address of NULL");
		init(*but);
		selection.push_back(but);		// Add but to all buttons
		return selection.size() - 1;	// Index of but
	}

	// Sets initial location and size to but
	void Menu::init(Button& but)
	{
		// Set initial size
		but.width = width;
		but.height = height;
		// Set initial location
		switch (kind) {		// Handle of direction
		case Menu::Kind::horizontal:
			but.loc = Point{ loc.x + static_cast<int>(offset), loc.y };
			offset += but.width;
			break;
		case Menu::Kind::vertical:
			but.loc = Point{ loc.x, loc.y + static_cast<int>(offset) };
			offset += but.height;
			break;
		}
	}

	//------------------------------------------------------------------------------

	// Constructs text box with top-left angle at xy, of size w * h, labeled lab
	Text_box::Text_box(Point xy, unsigned int w, unsigned int h, const std::string& lab)
		: Widget{ xy, w, h, lab, nullptr }
	{
	}

	// Puts str into text box
	void Text_box::put(const std::string& str)
	{
		Fl_Text_Display& text_box = reference_to<Fl_Text_Display>(pw);	// Reference to FLTK's widget
		text_box.buffer()->text(str.c_str());				// Put str into text box's buffer
	}

	// Gets text from text box
	std::string Text_box::get() const
	{
		const Fl_Text_Display& text_box = reference_to<Fl_Text_Display>(pw);	// Reference to FLTK's widget
		return std::string{ text_box.buffer()->text() };	// Get text from text box's buffer
	}

	// Attaches text box to win
	void Text_box::attach(Graph_lib::Window& win)
	{
		// Construction of FLTK's widget
		pw = new Fl_Text_Display{ loc.x, loc.y, static_cast<int>(width), static_cast<int>(height), label.c_str() };
		Fl_Text_Display& text_box = reference_to<Fl_Text_Display>(pw);	// Reference to FLTK's widget
		own = &win;		// Connection with window
		text_box.buffer(new Fl_Text_Buffer{});				// Construction of buffer for text box
	}

	//------------------------------------------------------------------------------

	// Constructs group with top-left angle at xy, of h_num * v_num buttons, with size of each
	// button equal to but_w * but_h, labeled with lab, and cb as callback function for buttons
	Group::Group(Point xy, unsigned int but_w, unsigned int but_h,
		unsigned int h_num, unsigned int v_num, const std::string& lab, Callback cb)
		: Widget{ xy, but_w, but_h, lab, cb }, selection{}
	{
		// Fill of grid
		for (unsigned int h_sz = 0; h_sz < h_num; ++h_sz)          // Horizontal line
			for (unsigned int v_sz = 0; v_sz < v_num; ++v_sz)      // Vertical line
				selection.push_back(new Button{ Point{ xy.x + static_cast<int>(but_w * h_sz),
					xy.y + static_cast<int>(but_h * v_sz) }, but_w, but_h, "", cb });
	}

	//------------------------------------------------------------------------------

}	// of namespace Graph_lib

//------------------------------------------------------------------------------