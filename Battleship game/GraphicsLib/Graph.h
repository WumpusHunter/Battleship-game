//------------------------------------------------------------------------------

#ifndef GRAPH_GUARD
#define GRAPH_GUARD 1

//------------------------------------------------------------------------------

#include <stdexcept>
#include <vector>
#include <list>
#include <initializer_list>
#include <string>
#include <sstream>
#include "fltk.h"
#include "Point.h"

//------------------------------------------------------------------------------

namespace Graph_lib {

	//------------------------------------------------------------------------------

	// Defense against ill-behaved Linux macros
	#undef major
	#undef minor

	//------------------------------------------------------------------------------

	// Invariant: 0 <= vis <= 255
	class Color {
	public:
		enum class Color_type {
			red = FL_RED, blue = FL_BLUE, green = FL_GREEN, yellow = FL_YELLOW,
			white = FL_WHITE, black = FL_BLACK, magenta = FL_MAGENTA, cyan = FL_CYAN,
			dark_red = FL_DARK_RED, dark_green = FL_DARK_GREEN, dark_yellow = FL_DARK_YELLOW,
			dark_blue = FL_DARK_BLUE, dark_magenta = FL_DARK_MAGENTA, dark_cyan = FL_DARK_CYAN
		};
		enum class Transparency {
			invisible = 0, visible = 255
		};

		// Construction
		Color(Color_type ct) : col{ static_cast<Fl_Color>(ct) }, vis{ static_cast<unsigned char>(Transparency::visible) } { }
		Color(Color_type ct, Transparency t) : col{ static_cast<Fl_Color>(ct) }, vis{ static_cast<unsigned char>(t) } { }
		Color(unsigned int ct) : col{ ct }, vis{ static_cast<unsigned char>(Transparency::visible) } { }
		Color(Transparency t) : col{ Fl_Color{} }, vis{ static_cast<unsigned char>(t) } { }

		// Access to parameters (writing)
		void set_visibility(Transparency t) { vis = static_cast<unsigned char>(t); }

		// Access to parameters (reading)
		unsigned int as_int() const { return col; }
		unsigned char visibility() const { return vis; }

	private:
		unsigned char vis;		// Visibility
		Fl_Color col;			// Color
	};

	//------------------------------------------------------------------------------

	// Invariant: 0 <= s <= 4, w >= 0
	class Line_style {
	public:
		enum class Line_style_type {
			solid = FL_SOLID,				// -------
			dash = FL_DASH,					// - - - -
			dot = FL_DOT,					// ....... 
			dashdot = FL_DASHDOT,			// - . - . 
			dashdotdot = FL_DASHDOTDOT,		// -..-..
		};

		// Construction
		Line_style(Line_style_type lst) : st{ static_cast<unsigned char>(lst) }, w{ 0 } { }
		Line_style(Line_style_type lst, unsigned int ww) : st{ static_cast<unsigned char>(lst) }, w{ ww } { }
		Line_style(unsigned char s) : st{ s }, w{ 0 } { static constexpr unsigned char r1 = 0, r2 = 4;
			if (st < r1 || r2 < st) throw std::invalid_argument("Bad Line_style: invalid line style type"); }

		// Access to parameters (reading)
		unsigned char style() const { return st; }
		unsigned int width() const { return w; }

	private:
		unsigned char st;	// Style
		unsigned int w;		// Width
	};

	//------------------------------------------------------------------------------

	// Invariant: 0 <= F <= 16
	class Font {
	public:
		enum class Font_type {
			helvetica = FL_HELVETICA,
			helvetica_bold = FL_HELVETICA_BOLD,
			helvetica_italic = FL_HELVETICA_ITALIC,
			helvetica_bold_italic = FL_HELVETICA_BOLD_ITALIC,
			courier = FL_COURIER,
			courier_bold = FL_COURIER_BOLD,
			courier_italic = FL_COURIER_ITALIC,
			courier_bold_italic = FL_COURIER_BOLD_ITALIC,
			times = FL_TIMES,
			times_bold = FL_TIMES_BOLD,
			times_italic = FL_TIMES_ITALIC,
			times_bold_italic = FL_TIMES_BOLD_ITALIC,
			symbol = FL_SYMBOL,
			screen = FL_SCREEN,
			screen_bold = FL_SCREEN_BOLD,
			zapf_dingbats = FL_ZAPF_DINGBATS
		};

		// Construction
		Font(Font_type ft) : f{ static_cast<unsigned char>(ft) } { }
		Font(unsigned char ff) : f{ ff } { static constexpr unsigned char r1 = 0, r2 = 16;
			if (f < r1 || r2 < f) throw std::invalid_argument("Bad Font: invalid font type"); }

		// Access to parameters (reading)
		unsigned char as_char() const { return f; }

	private:
		unsigned char f;	// Font
	};

	//------------------------------------------------------------------------------

	template<typename T>		// Requires Element<T>()
	class Vector_ref {
	public:
		// Type definitions
		using value_type = T;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using reference = value_type&;
		using const_reference = const value_type&;
		using size_type = unsigned int;
		using iterator = typename std::vector<pointer>::iterator;
		using const_iterator = typename std::vector<pointer>::const_iterator;

		// Construction
		Vector_ref() = default;

		// Destruction
		~Vector_ref() { for (auto& ptr : owned) delete ptr; }

		// Insertion and removal
		void push_back(reference val) { vec.push_back(&val); }
		void push_back(pointer ptr)		// Insertion of owned element
		{ vec.push_back(ptr); owned.push_back(ptr); }
		void pop_back(bool own);		// Own determines either removal element is owned or not

		// Access to parameters (writing)
		reference operator[](size_type i) { return *vec[i]; }
		reference front() { return *vec.front(); }
		reference back() { return *vec.back(); }

		// Access to parameters (reading)
		const_reference  operator[](size_type i) const { return *vec[i]; }
		const_reference front() const { return *vec.front(); }
		const_reference back() const { return *vec.back(); }
		size_type size() const { return vec.size(); }

		// Iterator functions
		typename iterator begin() { return vec.begin(); }
		typename iterator end() { return vec.end(); }
		typename const_iterator cbegin() const { return vec.cbegin(); }
		typename const_iterator cend() const { return vec.cend(); }

	private:
		std::vector<pointer> vec;		// All elements
		std::vector<pointer> owned;		// Owned elements
	};

	// Removes last element; if own is true removes last owned element as well
	template<typename T>		// Requires Element<T>()
	void Vector_ref<T>::pop_back(bool own)
	{
		if (vec.empty() || own && owned.empty())	// Handle of empty vector
			throw std::out_of_range("Bad Vector_ref: can't remove from empty vector");
		vec.pop_back();
		// Removal of last owned element
		if (own) {
			delete owned.back();
			owned.pop_back();
		}
	}

	//------------------------------------------------------------------------------

	template<typename T>		// Requires Element<T>()
	class List_ref {
	public:
		// Type definitions
		using value_type = T;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using reference = value_type&;
		using const_reference = const value_type&;
		using size_type = int;
		using iterator = typename std::list<pointer>::iterator;
		using const_iterator = typename std::list<pointer>::const_iterator;

		// Construction
		List_ref() = default;

		// Destruction
		~List_ref() { for (auto& ptr : owned) delete ptr; }

		// Insertion and removal
		void push_front(reference val) { lst.push_front(&val); }
		void push_front(pointer ptr)	// Insertion of owned element
		{ lst.push_front(ptr); owned.push_front(ptr); }
		void push_back(reference val) { lst.push_back(&val); }
		void push_back(pointer ptr)		// Insertion of owned element
		{ lst.push_back(ptr); owned.push_back(ptr); }
		void pop_front(bool own);		// own determines either removal element is owned or not
		void pop_back(bool own);		// own determines either removal element is owned or not

		// Access to parameters (writing)
		reference front() { return *lst.front(); }
		reference back() { return *lst.back(); }

		// Access to parameters (reading)
		const_reference front() const { return *lst.front(); }
		const_reference back() const { return *lst.back(); }
		size_type size() const { return lst.size(); }

		// Iterator functions
		typename iterator begin() { return lst.begin(); }
		typename iterator end() { return lst.end(); }
		typename const_iterator cbegin() const { return lst.cbegin(); }
		typename const_iterator cend() const { return lst.cend(); }

	private:
		std::list<pointer> lst;		// All elements
		std::list<pointer> owned;	// Owned elements
	};

	// Removes first element; if own is true, then removes first owned element as well
	template<typename T>		// Requires Element<T>()
	void List_ref<T>::pop_front(bool own)
	{
		if (lst.empty() || own && owned.empty())	// Handle of empty list
			throw std::out_of_range("Bad List_ref: can't remove from empty list");
		lst.pop_front();
		// Removal of first owned element
		if (own) {
			delete owned.front();
			owned.pop_front();
		}
	}

	// Removes last element; if own is true removes last owned element as well
	template<typename T>		// Requires Element<T>()
	void List_ref<T>::pop_back(bool own)
	{
		if (lst.empty() || own && owned.empty())	// Handle of empty list
			throw std::out_of_range("Bad List_ref: can't remove from empty list");
		lst.pop_back();
		// Removal of last owned element
		if (own) {
			delete owned.back();
			owned.pop_back();
		}
	}

	//------------------------------------------------------------------------------

	class Shape {	// Deals with color and style, and holds sequence of lines
	public:
		// Drawing of shape
		void draw() const;					// Deal with color, style and draw_lines

		// Access to parameters (writing)
		void set_color(Color lc) { lcolor = lc; }
		void set_style(Line_style ls) { lstyle = ls; }
		void set_fill_color(Color fc) { fcolor = fc; }
		virtual void move(int dx, int dy);	// Add dx to x and dy to y coordinates

		// Access to parameters (reading)
		Color color() const { return lcolor; }
		Line_style style() const { return lstyle; }
		Color fill_color() const { return fcolor; }
		Point point(unsigned int i) const { return points[i]; }
		unsigned int number_of_points() const { return points.size(); }

		// Destruction
		virtual ~Shape() { }

		// Forbidden copying
		Shape(const Shape&) = delete;
		Shape& operator=(const Shape&) = delete;

	protected:
		// Construction
		Shape() : lcolor{ fl_color() }, lstyle{ 0 }, fcolor{ Color::Transparency::invisible } { }
		// Constructs shape with points from lst
		Shape(std::initializer_list<Point> lst) : lcolor{ fl_color() }, lstyle{ 0 },
			fcolor{ Color::Transparency::invisible } { for (const auto& p : lst) add(p); }

		// Drawing of shape
		virtual void draw_lines() const;	// Draw appropriate lines

		// Access to parameters (writing)
		void add(Point p) { points.push_back(p); }
		void set_point(unsigned int i, Point p) { points[i] = p; }

	private:
		std::vector<Point> points;	// Vertices of shape
		Color lcolor;				// Color of lines
		Line_style lstyle;			// Style of lines
		Color fcolor;				// Fill color of lines
	};

	//------------------------------------------------------------------------------

	typedef double Fct(double);		// Function-type

	//------------------------------------------------------------------------------

	// Invariant: r2 > r1, count > 0
	class Function : public Shape {
	public:
		// Construction (function's parameters aren't stored)
		Function(Fct f, double r1, double r2, Point orig, unsigned int count = 100, double xscale = 25, double yscale = 25);
	};

	//------------------------------------------------------------------------------

	struct Line : Shape {
	public:
		// Constructs line with beginning at p1 and ending at p2
		Line(Point p1, Point p2) { add(p1); add(p2); }
	};

	//------------------------------------------------------------------------------

	// Invariant: w >= 0, h >= 0
	class Rectangle : public Shape {
	public:
		// Constructs rectangle with top-left angle at xy, of size ww * hh
		Rectangle(Point xy, unsigned int ww, unsigned int hh) : w{ ww }, h{ hh } { add(xy); }
		// Constructs rectangle with top-left angle at p1 and bottom-right angle at p2
		Rectangle(Point p1, Point p2) : w{ static_cast<unsigned int>(p2.x) - static_cast<unsigned int>(p1.x) },
			h{ static_cast<unsigned int>(p2.y) - static_cast<unsigned int>(p1.y) } { add(p1); }

		// Drawing of shape
		void draw_lines() const;

		// Access to parameters (reading)
		unsigned int width() const { return w; }
		unsigned int height() const { return h; }

	private:
		unsigned int w;		// Width
		unsigned int h;		// Height
	};

	//------------------------------------------------------------------------------

	struct Open_polyline : Shape {		// Open sequence of lines
	public:
		// Construction
		using Shape::Shape;

		// Drawing of shape
		void draw_lines() const;

		// Access to parameters (writing)
		using Open_polyline::Shape::add;
	};

	//------------------------------------------------------------------------------

	struct Closed_polyline : Open_polyline {	// Closed sequence of lines
	public:
		// Construction
		using Open_polyline::Open_polyline;

		// Drawing of shape
		void draw_lines() const;
	};

	//------------------------------------------------------------------------------

	// Invariant: number_of_points >= 3, non-intersecting sequence of lines
	class Polygon : public Closed_polyline {	// Closed sequence of non-intersecting lines
	public:
		// Construction
		using Closed_polyline::Closed_polyline;

		// Drawing of shape
		void draw_lines() const;

		// Access to parameters (writing)
		void add(Point p);
	};

	//------------------------------------------------------------------------------

	// Invariant: number_of_points % 2 == 0
	class Lines : public Shape {		// Independent sequence of lines
	public:
		// Construction
		Lines() {}
		// Constructs lines using pairs of points from lst
		Lines(std::initializer_list<Point> lst) : Shape{ lst }
		{
			if (lst.size() % 2)		// Handle of odd number of points
				throw std::invalid_argument("Bad Lines: odd number of points");
		}

		// Drawing of shape
		void draw_lines() const;

		// Access to parameters (writing)
		void add(Point p1, Point p2) { Shape::add(p1); Shape::add(p2); }
	};

	//------------------------------------------------------------------------------

	// Converts val into string
	template<typename T>		// Requires Output_operator<T>()
	std::string to_string(const T& val)
	{
		std::ostringstream os;
		os << val;
		return os.str();
	}

	constexpr unsigned int min_font_sz = 14;		// Min default font size

	// Invariant: fnt_sz >= 0
	class Text : public Shape {
	public:
		// Constructs text with bottom-left angle of first letter at xy, labeled l
		Text(Point xy, const std::string& l) : lab{ l }, fnt{ fl_font() },
			fnt_sz{ static_cast<unsigned int>(min_font_sz < fl_size() ? fl_size() : min_font_sz) } { add(xy); }

		// Drawing of shape
		void draw_lines() const;

		// Access to parameters (writing)
		void set_label(const std::string& l) { lab = l; }
		void set_font(Font f) { fnt = f; }
		void set_font_size(unsigned int sz) { fnt_sz = sz; }

		// Access to parameters (reading)
		std::string label() const { return lab; }
		Font font() const { return fnt; }
		unsigned int font_size() const { return fnt_sz; }

	private:
		std::string lab;		// Label
		Font fnt;				// Font
		unsigned int fnt_sz;	// Size of font (at least 14 points)
	};

	//------------------------------------------------------------------------------

	// Invariant: length >= 0, num_of_notches >= 0
	class Axis : public Shape {
	public:
		enum class Orientation { x, y, z };

		// Construction
		Axis(Orientation dir, Point xy, unsigned int length, unsigned int num_of_notches = 0, const std::string& lab = "");

		// Drawing of shape
		void draw_lines() const;

		// Access to parameters (writing)
		void move(int dx, int dy);
		void set_color(Color c);

		Text label;
		Lines notches;
	};

	//------------------------------------------------------------------------------

	// Invariant: r >= 0
	class Circle : public Shape {
	public:
		// Constructs circle with center at xy, of radius rr
		Circle(Point xy, unsigned int rr) : r{ rr }
		{ add(Point{ xy.x - static_cast<int>(r), xy.y - static_cast<int>(r) }); }

		// Drawing of shape
		void draw_lines() const;

		// Access to parameters (writing)
		void set_radius(unsigned int rr) { r = rr; }

		// Access to parameters (reading)
		unsigned int radius() const { return r; }
		Point center() const { return Point{ point(0).x + static_cast<int>(r), point(0).y + static_cast<int>(r) }; }

	private:
		unsigned int r;		// Radius
	};

	//------------------------------------------------------------------------------

	// Invariant: w >= 0, h >= 0
	class Ellipse : public Shape {
	public:
		// Constructs ellipse with center at xy, of major axis ww and minor axis hh
		Ellipse(Point p, unsigned int ww, unsigned int hh) : w{ ww }, h{ hh }
		{ add(Point{ p.x - static_cast<int>(ww), p.y - static_cast<int>(hh) }); }

		// Drawing of shape
		void draw_lines() const;

		// Access to parameters (writing)
		void set_major(unsigned int ww) { w = ww; }
		void set_minor(unsigned int hh) { h = hh; }

		// Access to parameters (reading)
		unsigned int major() const { return w; }
		unsigned int minor() const { return h; }
		Point center() const { return Point{ point(0).x + static_cast<int>(w), point(0).y + static_cast<int>(h) }; }
		Point focus1() const { return w > h ? Point{ center().x - focus_distance(), center().y } : Point{ center().x, center().y - focus_distance() }; }
		Point focus2() const { return w > h ? Point{ center().x + focus_distance(), center().y } : Point{ center().x, center().y + focus_distance() }; }

	private:
		unsigned int w;		// Width (major axis)
		unsigned int h;		// Height (minor axis)

		// Helper function
		int focus_distance() const		// Determines distance from foci to center
		{ return static_cast<int>(sqrt(abs(double(w) * double(w) - double(h) * double(h)))); }
	};

	//------------------------------------------------------------------------------

	struct Marked_polyline : Open_polyline {
	public:
		// Constructs marked polyline, marked with m
		Marked_polyline(const std::string& m) : mark{ Point{}, m } {}

		// Drawing of shape
		void draw_lines() const;

		// Access to parameters (writing)
		void set_color(Color c);
		void move(int dx, int dy);

		Text mark;
	};

	//------------------------------------------------------------------------------

	struct Marks : Marked_polyline {
	public:
		// Constructs marks, marked with m (each symbol of m is mark with its own point)
		Marks(const std::string& m) : Marked_polyline{ m }
		{ Open_polyline::set_color(Color::Transparency::invisible); }

		// Access to parameters (writing)
		void set_color(Color c) { mark.set_color(c); }
	};

	//------------------------------------------------------------------------------

	struct Mark : Marks {
	public:
		// Constructs mark with center at xy, marked with m
		Mark(Point xy, char m) : Marks{ std::string{ m } } { add(xy); }
	};

	//------------------------------------------------------------------------------

	// Invariant: w >= 0, h >= 0
	class Bad_image : public Fl_Image {
	public:
		// Constructs image of size w * h
		Bad_image(unsigned int w, unsigned int h) : Fl_Image{ w, h, 0 } { }

		// Drawing of image
		void draw(int x, int y, int, int, int, int) { draw_empty(x, y); }
	};

	struct Suffix {		// Suffixes of image
	public:
		enum class Encoding { none, jpg, gif, bmp };
	};

	// Helper function
	Suffix::Encoding get_encoding(const std::string& f);

	// Invariant: w >= 0, h >= 0
	class Image : public Shape {
	public:
		// Construction
		Image(Point xy, std::string name, Suffix::Encoding suf = Suffix::Encoding::none);

		// Destruction
		~Image() { delete pi; }

		// Drawing of image
		void draw_lines() const;

		// Access to parameters (writing)
		void set_mask(Point xy, unsigned int ww, unsigned int hh) { w = ww; h = hh; cx = xy.x; cy = xy.y; }
		void move(int dx, int dy) { Shape::move(dx, dy); pi->draw(point(0).x, point(0).y); }

	private:
		// "Masking box"
		unsigned int w, h;	// Width and height
		int cx, cy;			// Relative position (cx, cy)
		Fl_Image* pi;		// Address of image
		Text lab;			// Label (used to message errors)
	};

	//------------------------------------------------------------------------------

	// Invariant: cell_w >= 0, cell_h >= 0, h_num >= 0, v_num >= 0
	class Grid : public Shape {
	public:
		// Construction
		Grid(Point xy, unsigned int cell_w, unsigned int cell_h, unsigned int h_num, unsigned int v_num);

		// Drawing of shape
		void draw_lines() const;

		// Access to parameters (writing)
		void set_color(Color c);
		void set_fill_color(Color c);
		void set_style(Line_style ls);
		void move(int dx, int dy);

		// Access to parameters (reading)
		unsigned int width() const;
		unsigned int height() const;
		unsigned int size() const { return grid.size(); }

	private:
		Vector_ref<Rectangle> grid;
	};

	// Helper function
	bool is_grid(const Grid& grid, const Rectangle& cell);

	//------------------------------------------------------------------------------

}	// of namespace Graph_lib

//------------------------------------------------------------------------------

#endif

//------------------------------------------------------------------------------