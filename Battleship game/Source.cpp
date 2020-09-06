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
    // Marked grid with top-left angle of grid at (50, 50), of size 50 * 50 for each
    // cell, with 10 * 6 horizontal and vertical lines, marked with "012345ABCDEFGHIJ"
    Marked_grid mgrid{ Point{ 50, 50 }, 50, 50, 10, 6, "012345ABCDEFGHIJ" };
    // Ship with top-left angle of head at (50, 50), of 50 * 50 size
    // for each cell, of kind Cruiser, and oriented vertically
    Ship ship1{ Point{ 50, 50 }, 50, 50, Ship::Kind::Cruiser, Ship::Orientation::vertical };

    // Modify parameters of graphics
    mgrid.set_color(Color::Color_type::black);
    mgrid.set_fill_color(Color::Color_type::blue);
    ship1.set_color(Color::Color_type::black);
    ship1.set_fill_color(Color::Color_type::black);
    
    // Connection of graphics with window
    win.attach(mgrid);
    win.attach(ship1);
    
    win.wait_for_button();
    random_move(ship1, mgrid.point(0), mgrid.width(), mgrid.height());
    win.wait_for_button();
    random_move(ship1, mgrid.point(0), mgrid.width(), mgrid.height());
    win.wait_for_button();
    random_move(ship1, mgrid.point(0), mgrid.width(), mgrid.height());
    win.wait_for_button();
    random_move(ship1, mgrid.point(0), mgrid.width(), mgrid.height());
    win.wait_for_button();
    random_move(ship1, mgrid.point(0), mgrid.width(), mgrid.height());
    win.wait_for_button();
}

//------------------------------------------------------------------------------