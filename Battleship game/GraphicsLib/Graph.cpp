//------------------------------------------------------------------------------

#include <fstream>
#include <map>
#include <algorithm>
#include "Graph.h"

//------------------------------------------------------------------------------

namespace Graph_lib {

	//------------------------------------------------------------------------------

	// Connects points of shape with lines
	void Shape::draw_lines() const
	{
		// Connect points with lines
		if (color().visibility())		// Draw only if color is visible
			for (unsigned int i = 1; i < points.size(); ++i)
				fl_line(points[i - 1].x, points[i - 1].y, points[i].x, points[i].y);
	}

	// Draws lines of shape, sets color of lines and style of lines
	void Shape::draw() const
	{
		const Fl_Color oldc = fl_color();	// Old color
		// Set color and style of lines
		fl_color(lcolor.as_int());
		fl_line_style(lstyle.style(), lstyle.width());
		draw_lines();
		// Reset color and style of lines
		fl_color(oldc);
		fl_line_style(0);		// Default style of lines
	}

	//------------------------------------------------------------------------------

	// Determines either two lines (p1, p2) and (p3, p4) intersect each other;
	// Returns distances from p1 to intersection point if lines are intersected
	inline std::pair<double, double> line_intersect(Point p1, Point p2, Point p3, Point p4, bool& parallel)
	{
		const double x1 = p1.x, x2 = p2.x, x3 = p3.x, x4 = p4.x;	// X-coordinates of lines
		const double y1 = p1.y, y2 = p2.y, y3 = p3.y, y4 = p4.y;	// Y-coordinates of lines
		const double denom = ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));	// Denominator
		if (denom == 0.0) {		// Handle of possible division by zero
			parallel = true;
			return std::make_pair(0.0, 0.0);
		}
		parallel = false;
		// Distances from p1 to intersection point
		return std::make_pair(((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denom,
			((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denom);
	}

	// Determines either two lines (p1, p2) and (p3, p4) intersect each other;
	// Returns true and intersection point if lines are intersected
	bool line_segment_intersect(Point p1, Point p2, Point p3, Point p4, Point& intersection)
	{
		static constexpr double r1 = 0.0, r2 = 1.0;		// Range for determining equation
		bool parallel;									// Parallelism of lines
		// Determine unknown equations for lines
		std::pair<double, double> u = line_intersect(p1, p2, p3, p4, parallel);
		if (parallel || u.first < r1 || u.first > r2 || u.second < r1 || u.second > r2) return false;
		// Determine intersection point
		intersection.x = p1.x + static_cast<int>(u.first * (double(p2.x) - double(p1.x)));
		intersection.y = p1.y + static_cast<int>(u.first * (double(p2.y) - double(p1.y)));
		return true;
	}

	// Adds p to all points
	void Polygon::add(Point p)
	{
		const unsigned int num = number_of_points();
		// Error handling
		if (num && p == point(num - 1))		// Handle of equal points (new and previous)
			throw std::invalid_argument("Bad Polygon: new point equal to previous point");
		if (num > 1) {
			bool parallel;		// Parallelism of lines
			line_intersect(point(num - 1), p, point(num - 2), point(num - 1), parallel);
			if (parallel)					// Handle of parallel lines (new and previous)
				throw std::invalid_argument("Bad Polygon: two points lie in a straight line");
			for (unsigned int i = 1; i < num - 1; ++i) {
				Point intersection{ 0, 0 };
				// Handle of intersected lines (new and all others)
				if (line_segment_intersect(point(num - 1), p, point(i - 1), point(i), intersection))
					throw std::invalid_argument("Bad Polygon: intersection of lines in " + to_string(intersection));
			}
		}
		Closed_polyline::add(p);
	}

	// Connects points of polygon with lines, fills it with color
	void Polygon::draw_lines() const
	{
		static constexpr unsigned int min_num = 3;		// Min number of points
		static const std::string min_num_str = to_string(min_num);
		if (number_of_points() < min_num)		// Handle of non-polygon shapes
			throw std::invalid_argument("Bad Polygon: less than " + min_num_str + " points");
		Closed_polyline::draw_lines();
	}

	//------------------------------------------------------------------------------

	// Connects points of open polyline with lines, fills it with color
	void Open_polyline::draw_lines() const
	{
		// Fill shape with color
		if (fill_color().visibility()) {		// Fill only if fill color is visible
			fl_color(fill_color().as_int());	// Set fill color
			// Map points to fill shape
			fl_begin_complex_polygon();
			for (unsigned int i = 0; i < number_of_points(); ++i)
				fl_vertex(point(i).x, point(i).y);
			fl_end_complex_polygon();
			fl_color(color().as_int());			// Reset fill color
		}
		Shape::draw_lines();		// Connect points with lines
	}

	//------------------------------------------------------------------------------

	// Connects points of closed polyline with lines, fills it with color
	void Closed_polyline::draw_lines() const
	{
		Open_polyline::draw_lines();
		// Draw closing line
		const unsigned int num = number_of_points();
		if (color().visibility() && num > 1)	// Draw only if color is visible
			fl_line(point(num - 1).x, point(num - 1).y, point(0).x, point(0).y);
	}

	//------------------------------------------------------------------------------

	// Moves shape dx by x-coordinate and dy by y-coordinate
	void Shape::move(int dx, int dy)
	{
		for (unsigned int i = 0; i < points.size(); ++i) {
			points[i].x += dx;		// Move by x-coordinate
			points[i].y += dy;		// Move by y-coordinate
		}
	}

	//------------------------------------------------------------------------------

	// Connects pairs of points with lines
	void Lines::draw_lines() const
	{
		// Connect pairs of points with lines
		if (color().visibility())		// Draw only if color is visible
			for (unsigned int i = 1; i < number_of_points(); i += 2)
				fl_line(point(i - 1).x, point(i - 1).y, point(i).x, point(i).y);
	}

	//------------------------------------------------------------------------------

	// Draws label of text; note: don't try to use new line characters
	void Text::draw_lines() const
	{
		const int old_fnt = fl_font();	// Old font
		const int old_sz = fl_size();	// Old size of font
		// Draw label of text
		fl_font(fnt.as_char(), fnt_sz);		// Set new font and font size
		fl_draw(lab.c_str(), point(0).x, point(0).y);
		fl_font(old_fnt, old_sz);			// Reset to old font and font size
	}

	//------------------------------------------------------------------------------

	// Constructs function with first point at xy, graphing f(x) for x in range [r1:r2) using
	// count line segments; x-coordinates are scaled by xscale and y-coordinates by yscale
	Function::Function(Fct f, double r1, double r2, Point xy, unsigned int count, double xscale, double yscale)
	{
		// Error handling
		if (r2 - r1 <= 0)		// Handle of invalid range
			throw std::invalid_argument("Bad Function: invalid graphing range");
		if (count == 0)			// Handle of line segments' number
			throw std::invalid_argument("Bad Function: non-positive graphing count");
		const double dist = (r2 - r1) / count;		// Distance of line segments
		// Graph f(x) for x in range [r1:r2) using count line segments
		for (unsigned int i = 0; i < count; ++i) {
			add(Point{ xy.x + static_cast<int>(r1 * xscale), xy.y - static_cast<int>(f(r1) * yscale) });
			r1 += dist;		// Traverse forward in range
		}
	}

	//------------------------------------------------------------------------------

	// Draws lines of rectangle, fills it with color
	void Rectangle::draw_lines() const
	{
		// Fill shape with color
		if (fill_color().visibility()) {		// Fill only if fill color is visible
			fl_color(fill_color().as_int());	// Set fill color
			fl_rectf(point(0).x, point(0).y, w, h);
			fl_color(color().as_int());			// Reset color
		}
		// Draw lines
		if (color().visibility()) {		// Draw only if color is visible
			fl_color(color().as_int());
			fl_rect(point(0).x, point(0).y, w, h);
		}
	}

	//------------------------------------------------------------------------------

	// Constructs axis with first point at xy, oriented (directed)
	// with dir, of num notches throw length, labeled lab
	// Note: xy is leftmost point for x-axis and lowermost point for y-axis
	Axis::Axis(Orientation dir, Point xy, unsigned int length, unsigned int num, const std::string& lab)
		:label{ Point{ 0, 0 }, lab }
	{
		switch (dir) {		// Handle of axis' direction
		case Axis::Orientation::x:
		{
			// Add points of axis line
			Shape::add(xy);
			Shape::add(Point{ xy.x + static_cast<int>(length), xy.y });
			// Add lines of notches
			if (num > 1) {
				static constexpr int dy = 5;				// Offset by y-coordinate
				const unsigned int dist = length / num;		// Distance between notches
				int x = xy.x + dist;						// Current x-coordinate
				for (unsigned int i = 0; i < num; ++i) {
					notches.add(Point{ x, xy.y }, Point{ x, xy.y - dy });
					x += dist;		// Traverse forward in axis
				}
			}
			label.move(length / 3, xy.y + 20);		// Label under axis
			break;
		}
		case Axis::Orientation::y:
		{
			// Add points of axis line (axis goes up)
			Shape::add(xy);
			Shape::add(Point{ xy.x, xy.y - static_cast<int>(length) });
			// Add lines of notches
			if (num > 1) {
				static constexpr int dx = 5;				// Offset by x-coordinate
				const unsigned int dist = length / num;		// Distance between notches
				int y = xy.y - dist;						// Current y-coordinate
				for (unsigned int i = 0; i < num; ++i) {
					notches.add(Point{ xy.x, y }, Point{ xy.x + dx, y });
					y -= dist;					// Traverse backward in axis
				}
			}
			label.move(xy.x - 10, xy.y - length - 10);		// Label at top of axis
			break;
		}
		case Axis::Orientation::z:		// Handle of unimplemented axis
			throw std::invalid_argument("Bad Axis: unimplemented z-axis");
		}
	}

	// Connects points of axis, draws notches and label
	void Axis::draw_lines() const
	{
		// Draw all elements of axis independently, that is, they can possess different parameters
		Shape::draw_lines();	// Draw axis line
		notches.draw();
		label.draw();
	}

	// Sets c as color of all elements of axis
	void Axis::set_color(Color c)
	{
		Shape::set_color(c);	// Set color for axis line
		notches.set_color(c);
		label.set_color(c);
	}

	// Moves all elements of axis by dx at x-coordinate and by dy at y-coordinate
	void Axis::move(int dx, int dy)
	{
		Shape::move(dx, dy);	// Move axis line
		notches.move(dx, dy);
		label.move(dx, dy);
	}

	//------------------------------------------------------------------------------

	// Draws line segments for circle, fills it with color
	void Circle::draw_lines() const
	{
		static constexpr unsigned int beg = 0, end = 360;	// Start and end angles
		// Fill shape with color
		if (fill_color().visibility()) {		// Fill only if fill color is visible
			fl_color(fill_color().as_int());	// Set fill color
			fl_pie(point(0).x, point(0).y, r + r - 1, r + r - 1, beg, end);
			fl_color(color().as_int());			// Reset color
		}
		// Draw line segments
		if (color().visibility()) {		//  Draw only if color is visible
			fl_color(color().as_int());
			fl_arc(point(0).x, point(0).y, r + r, r + r, beg, end);
		}
	}

	//------------------------------------------------------------------------------

	// Draws line segments for ellipse, fills it with color
	void Ellipse::draw_lines() const
	{
		static constexpr unsigned int beg = 0, end = 360;	// Start and end angles
		// Fill shape with color
		if (fill_color().visibility()) {		// Fill only if fill color is visible
			fl_color(fill_color().as_int());	// Set fill color
			fl_pie(point(0).x, point(0).y, w + w - 1, h + h - 1, beg, end);
			fl_color(color().as_int());			// Reset color
		}
		// Draw line segments
		if (color().visibility()) {		// Draw only if color is visible
			fl_color(color().as_int());
			fl_arc(point(0).x, point(0).y, w + w, h + h, beg, end);
		}
	}

	//------------------------------------------------------------------------------

	// Draws mark with center at xy, labeled lab
	void draw_mark(Point xy, char lab)
	{
		static constexpr int dx = 4;	// Offset by x-coordinate
		static constexpr int dy = 4;	// Offset by y-coordinate
		std::string mark{ lab };
		// Draw mark
		fl_draw(mark.c_str(), xy.x - dx, xy.y + dy);
	}

	// Connects points with lines, draws mark at each point
	void Marked_polyline::draw_lines() const
	{
		Open_polyline::draw_lines();
		// Draw marks at each point
		for (unsigned int i = 0; i < number_of_points(); ++i)
			draw_mark(point(i), mark[i % mark.size()]);
	}

	//------------------------------------------------------------------------------

	std::map<std::string, Suffix::Encoding> suffix_map;		// Map of encoding suffixes

	// Initialization of suffix map
	bool init_suffix_map()
	{
		// Use both, lower and upper case, for encoding suffixes
		suffix_map["jpg"] = Suffix::Encoding::jpg;
		suffix_map["JPG"] = Suffix::Encoding::jpg;
		suffix_map["jpeg"] = Suffix::Encoding::jpg;
		suffix_map["JPEG"] = Suffix::Encoding::jpg;
		suffix_map["gif"] = Suffix::Encoding::gif;
		suffix_map["GIF"] = Suffix::Encoding::gif;
		suffix_map["bmp"] = Suffix::Encoding::bmp;
		suffix_map["BMP"] = Suffix::Encoding::bmp;
		return true;
	}

	// Gets encoding suffix from file named file
	Suffix::Encoding get_encoding(const std::string& file)
	{
		static bool init = init_suffix_map();		// Initialization of suffix map
		// Search for encoding suffix
		auto pos = std::find(file.rbegin(), file.rend(), '.');
		if (pos == file.rend()) return Suffix::Encoding::none;	// Unsuffixed file name
		std::string suf{ pos.base(), file.end() };				// Possible encoding suffix
		return suffix_map[suf];
	}

	// Checks if file named file exists and can be opened for reading
	bool can_open(const std::string& file)
	{
		std::ifstream ifs{ file.c_str() };		// Try to open file for reading
		return bool{ ifs };
	}

	// Constructs image with first point of label at xy, searching
	// for image in file named file, encoded with suf
	Image::Image(Point xy, std::string file, Suffix::Encoding suf)
		: w{ 0 }, h{ 0 }, cx{ 0 }, cy{ 0 }, pi{ nullptr }, lab{ xy, "" }
	{
		static constexpr unsigned int bad_w = 30, bad_h = 20;	// Size of bad image
		add(xy);	// First point of label
		if (!can_open(file)) {		// Handle of unopened files
			lab.set_label("Bad Image: can't open \"" + file + '\"');
			pi = new Bad_image{ bad_w, bad_h };	// "Error image"
			return;
		}
		if (suf == Suffix::Encoding::none)	// Search for encoding if it's unknown
			suf = get_encoding(file);
		switch (suf) {		// Handle of encoding suffix
		case Suffix::Encoding::jpg:
			pi = new Fl_JPEG_Image{ file.c_str() };
			break;
		case Suffix::Encoding::gif:
			pi = new Fl_GIF_Image{ file.c_str() };
			break;
		default:			// Unsupported image encoding
			lab.set_label("Bad Image: unsupported file type \"" + file + '\"');
			pi = new Bad_image{ bad_w, bad_h };	// "Error image"
		}
	}

	// Draws image and its label
	void Image::draw_lines() const
	{
		// Draw label
		if (lab.label() != "")		// Draw only if label isn't empty
			lab.draw_lines();
		// Draw image
		if (w && h)		// Draw with "masking box"
			pi->draw(point(0).x, point(0).y, w, h, cx, cy);
		else			// Draw without "masking box"
			pi->draw(point(0).x, point(0).y);
	}

	//------------------------------------------------------------------------------

	// Constructs grid with top-left angle at xy, of h_num * v_num
	// cells, and with size of each cell equal to cell_w * cell_h
	Grid::Grid(Point xy, unsigned int cell_w, unsigned int cell_h, unsigned int h_num, unsigned int v_num)
		: grid{}
	{
		// Fill of grid
		for (unsigned int h_sz = 0; h_sz < h_num; ++h_sz)          // Horizontal line
			for (unsigned int v_sz = 0; v_sz < v_num; ++v_sz)      // Vertical line
				grid.push_back(new Rectangle{ Point{ xy.x + static_cast<int>(cell_w * h_sz),
					xy.y + static_cast<int>(cell_h * v_sz) }, cell_w, cell_h });
		add(xy);							// Top-left angle of grid
	}

	// Draws cells of grid and fills them with color
	void Grid::draw_lines() const
	{
		// Draw cells of grid
		for (unsigned int i = 0; i < grid.size(); ++i)
			grid[i].draw();
	}

	// Moves grid by dx at x-coordinate and dy at y-coordinate 
	void Grid::move(int dx, int dy)
	{
		Shape::move(dx, dy);
		// Move cells of grid
		for (unsigned int i = 0; i < grid.size(); ++i)
			grid[i].move(dx, dy);
	}

	// Sets c as color of grid's cell's lines
	void Grid::set_color(Color c)
	{
		Shape::set_color(c);
		// Set c as color of lines to cells of grid
		for (unsigned int i = 0; i < grid.size(); ++i)
			grid[i].set_color(c);
	}

	// Sets c as fill color of grid's cells
	void Grid::set_fill_color(Color c)
	{
		Shape::set_fill_color(c);
		// Set c as fill color to cells of grid
		for (unsigned int i = 0; i < grid.size(); ++i)
			grid[i].set_fill_color(c);
	}

	// Sets ls as line style of grid's cell's lines
	void Grid::set_style(Line_style ls)
	{
		Shape::set_style(ls);
		// Set ls as line style to cells of grid
		for (unsigned int i = 0; i < grid.size(); ++i)
			grid[i].set_style(ls);
	}

	// Determines width of grid
	unsigned int Grid::width() const
	{
		// Search for cells with min and max possible x-coordinate
		const auto minmax_x = std::minmax_element(grid.cbegin(), grid.cend(),
			[](const Rectangle* a, const Rectangle* b) { return a->point(0).x < b->point(0).x; });
		const auto min_x = *minmax_x.first, max_x = *minmax_x.second;
		// Difference between min and max x-coordinates of cells, added to width of cell
		return min_x != nullptr && max_x != nullptr ? max_x->point(0).x - min_x->point(0).x + max_x->width() : 0;
	}

	// Determines height of grid
	unsigned int Grid::height() const
	{
		// Search for cells with min and max possible y-coordinate
		const auto minmax_y = std::minmax_element(grid.cbegin(), grid.cend(),
			[](const Rectangle* a, const Rectangle* b) { return a->point(0).y < b->point(0).y; });
		const auto min_y = *minmax_y.first, max_y = *minmax_y.second;
		// Difference between min and max x-coordinates of cells, added to width of cell
		return min_y != nullptr && max_y != nullptr ? max_y->point(0).y - min_y->point(0).y + max_y->height() : 0;
	}

	// Determines either cell is inside grid
	bool is_grid(const Grid& grid, const Rectangle& cell)
	{
		// Validation of x-coordinate (first line) and y-coordinate (second line)
		return grid.point(0).x <= cell.point(0).x && cell.point(0).x + cell.width() <= grid.point(0).x + grid.width()
			&& grid.point(0).y <= cell.point(0).y && cell.point(0).y + cell.height() <= grid.point(0).y + grid.height();
	}

	//------------------------------------------------------------------------------

}	// of namespace Graph_lib

//------------------------------------------------------------------------------