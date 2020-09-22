//------------------------------------------------------------------------------

#ifndef WINDOW_GUARD
#define WINDOW_GUARD 1

//------------------------------------------------------------------------------

#include <string>
#include <vector>
#include "fltk.h"
#include "Point.h"

//------------------------------------------------------------------------------

namespace Graph_lib {

	//------------------------------------------------------------------------------

	class Shape;		// "Forward declare" of Shape
	class Widget;		// "Forward declare" of Widget

	//------------------------------------------------------------------------------

	// Invariant: w >= 0, h >= 0
	class Window : public Fl_Double_Window {
	public:
		// Construction
		Window(unsigned int w, unsigned int h, const std::string& lab);		// Default location (xy)
		Window(Point xy, unsigned int w, unsigned int h, const std::string& lab);

		// Destruction
		virtual ~Window() { }

		// Access to data
		//void resize(unsigned int ww, unsigned int hh) { w = ww, h = hh; size(ww, hh); }
		void set_label(const std::string& s) { label(s.c_str()); }
		unsigned int x_max() const { return w; }
		unsigned int y_max() const { return h; }
		std::string get_label() const { return std::string{ label() }; }

		// Conection with with shapes and widgets
		void attach(Shape& s);		// Add s to shapes
		void attach(Widget& w);		// Add w to window
		void detach(Shape& s);		// Remove s from shapes 
		void detach(Widget& w);		// Remove w from window
		void put_on_top(Shape& p);	// Put p on top of other shapes

	protected:
		// Drawing of graphics
		void draw();

	private:
		std::vector<Shape*> shapes;		// Attached shapes
		unsigned int w, h;				// Width and height (size)

		// Helper function
		void init();					// Initial parameters of window
	};

	//------------------------------------------------------------------------------

	// Helper functions
	inline int gui_main() { return Fl::run(); }		// Invoke GUI library's main event loop
	inline unsigned int x_max() { return Fl::w(); }	// Width of screen in pixels
	inline unsigned int y_max() { return Fl::h(); }	// Height of screen in pixels

	//------------------------------------------------------------------------------

}	// End of namespace Graph_lib

//------------------------------------------------------------------------------

#endif

//------------------------------------------------------------------------------