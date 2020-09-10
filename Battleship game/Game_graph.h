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
		Marks lab;		// Label (marks)
	};

	//------------------------------------------------------------------------------

	// Invariant: cell_w >= 0, cell_h >= 0
	class Ship_cell : public Rectangle {
	public:
		friend class Ship;
		enum class State {
			miss, hit
		};

		// Constructs ship cell with top-left angle at xy, of
		// size cell_w * cell_h, with miss state by default
		Ship_cell(Point xy, unsigned int cell_w, unsigned int cell_h)
			: Rectangle{ xy, cell_w, cell_h }, state{ State::miss } {}

	protected:
		State state;
	};

	//------------------------------------------------------------------------------

	// Invariant: cell_w >= 0, cell_h >= 0
	class Ship : public Shape {
	public:
		enum class Kind {		// Kinds of shape and their length in cells
			Torpedo_boat = 1, Destroyer = 2, Cruiser = 3, Battleship = 4
		};
		enum class Orientation {
			horizontal, vertical
		};

		// Construction
		Ship(Point xy, unsigned int cell_w, unsigned int cell_h, Kind k, Orientation o);

		// Drawing of shape
		void draw_lines() const;

		// Access to parametrs (writing)
		Ship_cell::State shot(Point xy);
		void restore();
		void rotate();
		void set_color(Color c);
		void set_fill_color(Color c);
		void set_style(Line_style ls);
		void move(int dx, int dy);

		// Access to parameters (reading)
		bool is_sunk() const;
		Kind ship_kind() const { return kind; }
		Orientation orientation() const { return orient; }
		unsigned int cell_width() const { return cells.front().width(); }
		unsigned int cell_height() const { return cells.front().height(); }
		const Ship_cell& operator[](unsigned int i) const { return cells[i]; }

	private:
		Vector_ref<Ship_cell> cells;
		Kind kind;
		Orientation orient;
	};

	//------------------------------------------------------------------------------

	// Invariant: w >= 0, h >= 0, cell_w >= 0, cell_h >= 0
	class Fleet : public Shape {
	public:
		// Construction
		Fleet(Point xy, unsigned int ww, unsigned int hh, unsigned int cell_w, unsigned int cell_h);

		// Drawing of shape
		void draw_lines() const;

		// Access to parameters (writing)
		void random_location();
		Ship_cell::State shot(Point xy);
		void restore();
		void set_color(Color c);
		void set_fill_color(Color c);
		void set_style(Line_style ls);
		void move(int dx, int dy);
		Ship& operator[](unsigned int i) { return fleet[i]; }

		// Access to parameters (reading)
		bool is_sunk() const;
		unsigned int frame_width() const { return w; }
		unsigned int frame_height() const { return h; }
		unsigned int number_of_ships() const { return fleet.size(); }
		const Ship& operator[](unsigned int i) const { return fleet[i]; }

	private:
		Vector_ref<Ship> fleet;
		unsigned int w, h;			// Size of frame
	};

	//------------------------------------------------------------------------------

}

//------------------------------------------------------------------------------