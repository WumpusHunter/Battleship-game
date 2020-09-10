// Batlleship game

//------------------------------------------------------------------------------

#include "GraphicsLib/Simple_window.h"
#include "Game_graph.h"
using namespace Graph_lib;

//------------------------------------------------------------------------------

int main()
{
    // Window with top-left angle at (100, 100), of size 600 * 600, labeled "Battleship game"
    Simple_window win{ Point{ 100, 100 }, 600, 600, "Battleship game" };
    // Marked grid with top-left angle of grid at (50, 50), of size 50 * 50 for each
    // cell, with 10 * 10 horizontal and vertical lines, marked with "0123456789ABCDEFGHIJ"
    Marked_grid mgrid{ Point{ 50, 50 }, 50, 50, 10, 10, "0123456789ABCDEFGHIJ" };
    // Fleet with frame's top-left angle at (50, 50), of size
    // 500 * 500 for its frame, and of size 50 * 50 for each cell
    Fleet fleet{ Point{ 50, 50 }, 500, 500, 50, 50 };

    // Modify parameters of graphics
    mgrid.set_color(Color::Color_type::black);
    mgrid.set_fill_color(Color::Color_type::blue);
    fleet.set_color(Color::Color_type::black);
    fleet.set_fill_color(40);
    fleet.random_location();
    
    // Connection of graphics with window
    win.attach(mgrid);
    win.attach(fleet);

    win.wait_for_button();
}

//------------------------------------------------------------------------------