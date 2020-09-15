// Batlleship game

//------------------------------------------------------------------------------

#include "Game_GUI.h"
using namespace Graph_lib;

//------------------------------------------------------------------------------

int main()
{
	// Game window with top-left angle at (100, 100), of
	// size 1200 * 600, labeled with "Battleship game"
	Battleship game{ Point{ 100, 100 }, 1200, 600, "Battleship game" };
	return gui_main();
}

//------------------------------------------------------------------------------