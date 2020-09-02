//------------------------------------------------------------------------------

#include "GUI.h"

//------------------------------------------------------------------------------

namespace Graph_lib {

	// Invariant: w >= 0, h >= 0
	class Simple_window : public Window {
	public:
		// Construction
		Simple_window(Point xy, unsigned int w, unsigned int h, const std::string& lab);

		// Action function
		bool wait_for_button();

		// Widget
		Button next_button;

	private:
		bool button_pushed;		// Determines either next_button is pushed

		// Callback function
		static void cb_next(Address, Address pw);	// Callback function for next_button

		// Action function
		void next();								// Action function for next_button
	};

}

//------------------------------------------------------------------------------