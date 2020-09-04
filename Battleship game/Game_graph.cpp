#include "Game_graph.h"

//------------------------------------------------------------------------------

namespace Graph_lib {

	//------------------------------------------------------------------------------

	// Constructs marked grid with top-left angle of grid at xy, of size
	// cell_w * cell_h for each cell, with h_num horintal and v_num
	// vertical lines, marked with mark (one symbol for each cell of grid)
	Marked_grid::Marked_grid(Point xy, unsigned int cell_w, unsigned int cell_h,
		unsigned int h_num, unsigned int v_num, const std::string& mark)
		: Grid{ xy, cell_w, cell_h, h_num, v_num }, lab{ mark }
	{
		// Mark of grid
		for (unsigned int i = 0; i < v_num; ++i)		// Vertical line
			lab.add(Point{ xy.x, xy.y + static_cast<int>(cell_h * (i + 1)) });
		for (unsigned int i = 0; i < h_num; ++i)		// Horizontal line
			lab.add(Point{ xy.x + static_cast<int>(cell_w * (i + 1)), xy.y });
		// Default parameters for marks
		lab.mark.set_font_size((cell_w + cell_h) / 4);
		lab.move(-static_cast<int>(cell_w / 2), -static_cast<int>(cell_h / 2));
		//add(xy);		// One cell by x- and y-coordinate from top-left angle of grid
	}

	// Draws grid and marks of marked grid
	void Marked_grid::draw_lines() const
	{
		Grid::draw_lines();		// Draw grid
		lab.draw();				// Draw marks
	}

	// Sets c as color of grid and marks
	void Marked_grid::set_color(Color c)
	{
		Grid::set_color(c);		// Set color to grid
		lab.set_color(c);		// Set color to marks
	}

	//------------------------------------------------------------------------------

}

//------------------------------------------------------------------------------