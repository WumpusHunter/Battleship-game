//------------------------------------------------------------------------------

#include "Window.h"
#include "Graph.h"
#include "GUI.h"

//------------------------------------------------------------------------------

namespace Graph_lib {

	//------------------------------------------------------------------------------

	// Constructs window at default location, of size ww * hh, titled with lab
	Window::Window(unsigned int ww, unsigned int hh, const std::string& lab)
		: Fl_Double_Window{ ww, hh, lab.c_str() }, w{ ww }, h{ hh }
	{
		init();
	}

	// Constructs window at xy, of size ww * hh, titled with lab
	Window::Window(Point xy, unsigned int ww, unsigned int hh, const std::string& lab)
		: Fl_Double_Window{ xy.x, xy.y, ww, hh, lab.c_str() }, w{ ww }, h{ hh }
	{
		init();
	}

	// Initializes window with default parameters
	void Window::init()
	{
		resizable(this);
		show();
	}

	//------------------------------------------------------------------------------

	// Draws window, widgets and shapes
	void Window::draw()
	{
		Fl_Double_Window::draw();
		// Draw shapes
		for (unsigned int i = 0; i < shapes.size(); ++i)
			shapes[i]->draw();
	}

	// Attaches w to window
	void Window::attach(Widget& w)
	{
		begin();			// Begin attaching new widgets to window
		w.attach(*this);	// Let widget to create itself and to attach window
		end();				// Stop attaching new widgets to window
	}

	// Detaches w from window
	void Window::detach(Widget& w)
	{
		w.hide();		// Remove w from window
	}

	// Attaches s to window
	void Window::attach(Shape& s)
	{
		shapes.push_back(&s);		// Add s to shapes
	}

	// Detathes s from window
	void Window::detach(Shape& s)
	{
		// Remove s from shapes
		auto pos = std::remove(shapes.begin(), shapes.end(), &s);
		shapes.erase(pos, shapes.end());
	}

	// Puts s on top of other shapes
	void Window::put_on_top(Shape& s)
	{
		// Make s last attached to window shape
		auto pos = std::find(shapes.begin(), shapes.end(), &s);
		if (pos != shapes.end())	// Shift all shapes from s to left by one
			std::rotate(pos, std::next(pos), shapes.end());
	}

	//------------------------------------------------------------------------------

}	// End of namespace Graph_lib

//------------------------------------------------------------------------------