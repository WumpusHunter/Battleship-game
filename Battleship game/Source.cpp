// Batlleship game

//------------------------------------------------------------------------------

#include "GraphicsLib/Simple_window.h"
#include "Game_graph.h"
using namespace Graph_lib;

//------------------------------------------------------------------------------

int main()
{
    // Window with top-left angle at (100, 100), of size 600 * 400, labeled "Window"
    Simple_window win{ Point{ 100, 100 }, 600, 400, "Window" };
    // Marked grid with top-left angle of grid at (150, 150), of size 50 * 50 for
    // each cell, with 6 vertical and 3 horintal lines, marked with "123ABCDEF"
    Marked_grid mgrid{ Point{ 100, 100 }, 50, 50, 10, 10, "0123456789ABCDEFGHIJ" };

    // Modify parameters of graphics
    mgrid.set_color(Color::Color_type::black);
    mgrid.set_fill_color(Color::Color_type::blue);

    // Connection of graphics with window
    win.attach(mgrid);

    win.wait_for_button();
}

//------------------------------------------------------------------------------