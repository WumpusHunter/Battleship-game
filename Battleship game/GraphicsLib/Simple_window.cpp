//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include "Simple_window.h"

//------------------------------------------------------------------------------

namespace Graph_lib {

    //------------------------------------------------------------------------------

    // Constructs window with top-left angle at xy, of size w * h, labeled lab
    Simple_window::Simple_window(Point xy, unsigned int w, unsigned int h, const std::string& lab)
        : Window{ xy, w, h, lab },
        next_button{ Point{ static_cast<int>(x_max()) - 70, 0 }, 70, 20, "Next", cb_next },
        button_pushed{ false }
    {
        attach(next_button);        // Connect window and widget
    }

    // Handles all evenets, quits when next button is pressed
    bool Simple_window::wait_for_button()
    {
        show();         // Show window after pressed next_button
        button_pushed = false;
#if 1
        // Wait until next_button isn't pressed
        while (!button_pushed)
            Fl::wait();
        Fl::redraw();   // Redraw window after pressed next_button
#else
        Fl::run();      // Handle of closing window by X button to enable it
#endif
        return button_pushed;
    }

    // Callback function for next_button
    void Simple_window::cb_next(Address, Address pw)
    {
        reference_to<Simple_window>(pw).next();     // Call of action function
    }

    // Action function for next_button
    void Simple_window::next()
    {
        button_pushed = true;
        hide();         // Hide window after pressed next_button
    }

    //------------------------------------------------------------------------------

}

//------------------------------------------------------------------------------