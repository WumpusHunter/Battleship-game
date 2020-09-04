#pragma once
#include "GraphicsLib/Graph.h"

//------------------------------------------------------------------------------

namespace Graph_lib {

	//------------------------------------------------------------------------------

	// Invariant: cell_w >= 0, cell_h >= 0, h_num >= 0, v_num >= 0
	class Marked_grid : public Grid {
	public:
		// Construction
		Marked_grid(Point xy, unsigned int cell_w, unsigned int cell_h,
			unsigned int h_num, unsigned int v_num, const std::string& mark);

		// Drawing of shape
		void draw_lines() const;

		// Access to parameters (writing)
		void set_color(Color c);

	private:
		Marks lab;		// Label (mark)
	};

	//------------------------------------------------------------------------------

}

//------------------------------------------------------------------------------