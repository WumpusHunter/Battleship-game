//------------------------------------------------------------------------------

#ifndef POINT_GUARD
#define POINT_GUARD

//------------------------------------------------------------------------------

#include <iostream>

//------------------------------------------------------------------------------

namespace Graph_lib {

	//------------------------------------------------------------------------------

	struct Point {		// 2D coordinate
		// Construction
		Point(int xx, int yy) : x{ xx }, y{ yy } { }
		Point() :x{ 0 }, y{ 0 } { }

		// Access to data (writing)
		Point& operator+=(Point d) { x += d.x; y += d.y; return *this; }

		int x, y;		// X and y coordinates
	};

	// Helper functions
	inline bool operator==(Point a, Point b) { return a.x == b.x && a.y == b.y; }
	inline bool operator!=(Point a, Point b) { return !(a == b); }
	// Writes p into os; format: (x, y)
	inline std::ostream& operator<<(std::ostream& os, const Point& p)
	{ return os << '(' << p.x << ", " << p.y << ')'; }

}

//------------------------------------------------------------------------------

#endif

//------------------------------------------------------------------------------