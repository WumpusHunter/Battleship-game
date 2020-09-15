#include "RandGenerator/Generator.h"
#include "Game_graph.h"

//------------------------------------------------------------------------------

namespace Graph_lib {

	//------------------------------------------------------------------------------

	// Constructs marked grid with top-left angle of grid at xy, of size
	// cell_w * cell_h for each cell, with h_num horizontal and v_num
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

	// Sets vis as visibility of grid and marks
	void Marked_grid::set_visibility(Color::Transparency vis)
	{
		Grid::set_visibility(vis);	// Set visibility to grid
		lab.set_visibility(vis);	// Set visibility to marks
	}

	//------------------------------------------------------------------------------

	// Constructs ship with top-left angle of head at xy, of cell_w * cell_h
	// size for each cell, of kind k, and oriented with o
	Ship::Ship(Point xy, unsigned int cell_w, unsigned int cell_h, Kind k, Orientation o)
		: kind{ k }, orient{ Orientation::horizontal }
	{
		// Fill of cells using horizontal orientation
		for (unsigned int i = 0; i < static_cast<unsigned int>(kind); ++i)
			cells.push_back(new Ship_cell
				{ Point{ static_cast<int>(xy.x + cell_w * i), xy.y }, cell_w, cell_h });
		if (o == Orientation::vertical)		// Rotate if not guess orientation
			rotate();
		add(xy);		// Top-left angle of head
	}

	// Draws cells of ship
	void Ship::draw_lines() const
	{
		for (unsigned int i = 0; i < cells.size(); ++i)
			cells[i].draw();
	}

	// Marks ship's cell if shot resulted in hit; returns result of shot (hit or miss);
	// Shot considered to be successful if passed point has same location as one of ship's cells
	Ship_cell::State Ship::shot(Point xy)
	{
		// Check for result of shot
		for (unsigned int i = 0; i < cells.size(); ++i)
			if (cells[i].point(0) == xy) {
				cells[i].state = Ship_cell::State::hit;		// Mark hitted cell
				return Ship_cell::State::hit;
			}
		return Ship_cell::State::miss;
	}

	// Marks all cells of ship as missed
	void Ship::restore()
	{
		for (unsigned int i = 0; i < cells.size(); ++i)
			cells[i].state = Ship_cell::State::miss;
	}

	// Rotates ship to opposite orientation (i.e., horizontal => vertical)
	void Ship::rotate()
	{
		// Set orientation to opposite
		orient = orient == Orientation::horizontal ? Orientation::vertical : Orientation::horizontal;
		// Difference by x- and y-coordinates
		const int dx = orient == Orientation::horizontal ? cells.front().width() : -static_cast<int>(cells.front().width()),
			dy = orient == Orientation::vertical ? cells.front().height() : -static_cast<int>(cells.front().height());
		// Rotation of cells
		for (unsigned int i = 0; i < cells.size(); ++i)
			cells[i].move(dx * i, dy * i);
	}

	// Sets c as line color for cells of ship
	void Ship::set_color(Color c)
	{
		Shape::set_color(c);		// Update color of shape
		for (unsigned int i = 0; i < cells.size(); ++i)
			cells[i].set_color(c);
	}

	// Sets c as fill color for cells of ship
	void Ship::set_fill_color(Color c)
	{
		Shape::set_fill_color(c);	// Update fill color of shape
		for (unsigned int i = 0; i < cells.size(); ++i)
			cells[i].set_fill_color(c);
	}

	// Sets ls as line style for cells of ship
	void Ship::set_style(Line_style ls)
	{
		Shape::set_style(ls);		// Update line style of shape
		for (unsigned int i = 0; i < cells.size(); ++i)
			cells[i].set_style(ls);
	}

	// Sets vis as visibility for cells of ship
	void Ship::set_visibility(Color::Transparency vis)
	{
		Shape::set_visibility(vis);	// Update visibility of shape
		for (unsigned int i = 0; i < cells.size(); ++i)
			cells[i].set_visibility(vis);
	}

	// Moves cells of ship dx by x-coordinate and dy by y-coordinate
	void Ship::move(int dx, int dy)
	{
		Shape::move(dx, dy);		// Update location of shape
		for (unsigned int i = 0; i < cells.size(); ++i)
			cells[i].move(dx, dy);
	}

	// Determines either all cells of ship are hitted or not
	bool Ship::is_sunk() const
	{
		// Check for any missed cells in ship
		for (unsigned int i = 0; i < cells.size(); ++i)
			if (cells[i].state == Ship_cell::State::miss)
				return false;
		return true;
	}

	//------------------------------------------------------------------------------

	// Constructs fleet with top-left angle of its frame at xy, of
	// its size ww * hh, and of size cell_w * cell_h for each cell
	Fleet::Fleet(Point xy, unsigned int ww, unsigned int hh, unsigned int cell_w, unsigned int cell_h)
		: fleet{}, w{ ww }, h{ hh }
	{
		// Fill of fleet with ships
		using Kind = Ship::Kind; using Orient = Ship::Orientation;
		fleet.push_back(new Ship{ xy, cell_w, cell_h, Kind::Battleship, Orient::horizontal });
		fleet.push_back(new Ship{ xy, cell_w, cell_h, Kind::Cruiser, Orient::horizontal });
		fleet.push_back(new Ship{ xy, cell_w, cell_h, Kind::Cruiser, Orient::horizontal });
		fleet.push_back(new Ship{ xy, cell_w, cell_h, Kind::Destroyer, Orient::horizontal });
		fleet.push_back(new Ship{ xy, cell_w, cell_h, Kind::Destroyer, Orient::horizontal });
		fleet.push_back(new Ship{ xy, cell_w, cell_h, Kind::Destroyer, Orient::horizontal });
		fleet.push_back(new Ship{ xy, cell_w, cell_h, Kind::Torpedo_boat, Orient::horizontal });
		fleet.push_back(new Ship{ xy, cell_w, cell_h, Kind::Torpedo_boat, Orient::horizontal });
		fleet.push_back(new Ship{ xy, cell_w, cell_h, Kind::Torpedo_boat, Orient::horizontal });
		fleet.push_back(new Ship{ xy, cell_w, cell_h, Kind::Torpedo_boat, Orient::horizontal });
		add(xy);		// Top-left angle of frame
	}

	// Draws ships of fleet
	void Fleet::draw_lines() const
	{
		for (unsigned int i = 0; i < fleet.size(); ++i)
			fleet[i].draw();
	}

	//------------------------------------------------------------------------------

	// Determines if ship1 is overlapped by ship2 using some of their vertices
	bool is_overlap(const Ship& ship1, const Ship& ship2)
	{
		// Size of cells
		int cell_w1 = static_cast<int>(ship1.cell_width()), cell_h1 = static_cast<int>(ship1.cell_height());
		int cell_w2 = static_cast<int>(ship2.cell_width()), cell_h2 = static_cast<int>(ship2.cell_height());
		for (unsigned int i = 0; i < static_cast<unsigned int>(ship1.ship_kind()); ++i)
			for (unsigned int j = 0; j < static_cast<unsigned int>(ship2.ship_kind()); ++j) {
				/*
				   Points of cells     Shortcuts
				     tl___tr          tl - top-left
				      |   |           tr - top-right
				      |___|           dl - down-left
				     dl   dr          dr - down-right
				*/
				Point tl1 = ship1[i].point(0), tr1 = Point{ tl1.x + cell_w1, tl1.y },
					dl1 = Point{ tl1.x, tl1.y + cell_h1 }, dr1{ tl1.x + cell_w1, tl1.y + cell_h1 };
				Point tl2 = ship2[j].point(0), tr2 = Point{ tl2.x + cell_w2, tl2.y },
					dl2 = Point{ tl2.x, tl2.y + cell_h2 }, dr2{ tl2.x + cell_w2, tl2.y + cell_h2 };
				// Check for possible cases of overlapping
				if (tl1 == tl2 || tl1 == tr2 || tl1 == dl2 || tl1 == dr2)
					return true;
				if (tr1 == tl2 || tr1 == tr2 || tr1 == dl2 || tr1 == dr2)
					return true;
				if (dl1 == tl2 || dl1 == tr2 || dl1 == dl2 || dl1 == dr2)
					return true;
				if (dr1 == tl2 || dr1 == tr2 || dr1 == dl2 || dr1 == dr2)
					return true;
			}
		return false;
	}

	// Rotates ship randomly
	void random_rotate(Ship& ship)
	{
		static constexpr int leave = 0, rotate = 1;		// Keys for random rotation
		if (randint(leave, rotate) == rotate)
			ship.rotate();
	}

	// Moves ship to xy
	void move_to(Ship& ship, Point xy)
	{
		ship.move(xy.x - ship.point(0).x, xy.y - ship.point(0).y);
	}

	// Moves ship randomly in frame with top-left angle at xy, and of size w * h
	void random_move(Ship& ship, Point xy, unsigned int w, unsigned int h)
	{

		// Size of ship
		const unsigned int cell_w = ship.cell_width(), cell_h = ship.cell_height(),
			ship_w = ship.orientation() == Ship::Orientation::horizontal ?
			cell_w * static_cast<int>(ship.ship_kind()) : cell_w,
			ship_h = ship.orientation() == Ship::Orientation::vertical ?
			cell_h * static_cast<int>(ship.ship_kind()) : cell_h;
		// Random x and y coordinates
		const int rand_x = xy.x + cell_w * randint(0, static_cast<int>((w - ship_w) / cell_w)),
			rand_y = xy.y + cell_h * randint(0, static_cast<int>((h - ship_h) / cell_h));
		move_to(ship, Point{ rand_x, rand_y });     // Move to random point
	}

	//------------------------------------------------------------------------------

	// Locates ships of fleet randomly inside frame in such way that
	// none of them are overlapped by each other with their sides
	void Fleet::random_location()
	{
		for (unsigned int i = 0; i < fleet.size(); ++i) {
			do { // Relocate ship while it's overlapped by other ship
				random_rotate(fleet[i]);
				random_move(fleet[i], point(0), w, h);
			} while (std::find_if(fleet.cbegin(), std::next(fleet.cbegin(), i), [this, i](const Ship* s)
				{ return is_overlap(fleet[i], *s); }) != std::next(fleet.cbegin(), i));
		}
	}

	// Shots at each ship of fleet untill first hit
	Ship_cell::State Fleet::shot(Point xy)
	{
		// Search for first hitted ship
		for (unsigned int i = 0; i < fleet.size(); ++i)
			if (fleet[i].shot(xy) == Ship_cell::State::hit)
				return Ship_cell::State::hit;
		return Ship_cell::State::miss;
	}

	// Restores all ships of fleet
	void Fleet::restore()
	{
		for (unsigned int i = 0; i < fleet.size(); ++i)
			fleet[i].restore();
	}

	// Sets c as line color for ships of fleet
	void Fleet::set_color(Color c)
	{
		Shape::set_color(c);		// Update color of shape
		for (unsigned int i = 0; i < fleet.size(); ++i)
			fleet[i].set_color(c);
	}

	// Sets c as fill color for ships of fleet
	void Fleet::set_fill_color(Color c)
	{
		Shape::set_fill_color(c);	// Update fill color of shape
		for (unsigned int i = 0; i < fleet.size(); ++i)
			fleet[i].set_fill_color(c);
	}

	// Sets ls as line style for ships of fleet
	void Fleet::set_style(Line_style ls)
	{
		Shape::set_style(ls);		// Update line style of shape
		for (unsigned int i = 0; i < fleet.size(); ++i)
			fleet[i].set_style(ls);
	}

	// Sets vis as visiblity for ships of fleet
	void Fleet::set_visibility(Color::Transparency vis)
	{
		Shape::set_visibility(vis);	// Update visibility of shape
		for (unsigned int i = 0; i < fleet.size(); ++i)
			fleet[i].set_visibility(vis);
	}

	// Moves ships of fleet dx by x-coordinate and dy by y-coordinate
	void Fleet::move(int dx, int dy)
	{
		Shape::move(dx, dy);		// Update location of shape
		for (unsigned int i = 0; i < fleet.size(); ++i)
			fleet[i].move(dx, dy);
	}

	// Determines either all ships of fleet are sunken or not
	bool Fleet::is_sunk() const
	{
		// Search for any not sunken ship in fleet
		for (unsigned int i = 0; i < fleet.size(); ++i)
			if (!fleet[i].is_sunk())
				return false;
		return true;
	}

	//------------------------------------------------------------------------------

}

//------------------------------------------------------------------------------