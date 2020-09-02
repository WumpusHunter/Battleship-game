// Batlleship game

//------------------------------------------------------------------------------

#include "GraphicsLib/Graph.h"
#include "GraphicsLib/Simple_window.h"
using namespace Graph_lib;

//------------------------------------------------------------------------------

int main()
{
    // Window with top-left angle at (100, 100), of size 600 * 400, labeled "Window"
    Simple_window win{ Point{ 100, 100 }, 600, 400, "Window" };
    win.wait_for_button();
}

//------------------------------------------------------------------------------