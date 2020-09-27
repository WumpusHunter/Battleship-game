#include "Game_GUI.h"
#include "RandGenerator/Generator.h"

//------------------------------------------------------------------------------

namespace Graph_lib {

	//------------------------------------------------------------------------------

	// Constants
	constexpr unsigned int but_w = 100;		// Width of buttons
	constexpr unsigned int but_h = 30;		// Height of buttons
	constexpr unsigned int cell_w = 50;		// Width of cells
	constexpr unsigned int cell_h = 50;		// Height of cells
	constexpr unsigned int h_num = 10;		// Number of horizontal lines
	constexpr unsigned int v_num = 10;		// Number of vertical lines
	constexpr unsigned int x_offset = 100;	// Offset by x-coordinate between grids
	const Color frame = Color::Color_type::black;	// Color of frame
	const Color empty = Color::Color_type::blue;	// Color of empty cell
	const Color hit = Color::Color_type::red;		// Color of hitted cell
	const Color miss = Color::Color_type::white;	// Color of missed cell
	const std::string marks = "0123456789ABCDEFGHIJ";
	const std::string help_msg = R"( Rules from the classic Russian version:
1. Locate your fleet on your grid in such a way that the ships don't overlap
each other or go consecutive using their sides or angles. In our case, this
is done randomly.
2. Decide with the opposite player who is to shot first. In our case, you
always shot first.
3. Start to alternate shots with the opposite player until one of you won't
sink the entire target fleet. The one who did it first wins. The one who had
a "hit" continue shooting; the one who had a "miss" passes the turn to shot
to the opposite player.
 Fleet from the classic Russian version:
Number		Kind of ship		Size
1		Battleship		4
2		Cruiser			3
3		Destroyer		2
4		Torpedo boat		1
 The usual grid from the classic Russian version is a 10x10 grid of squares.)";

	//------------------------------------------------------------------------------

	// Constructs window with top-left angle at xy, of size w * h, and labeled with lab
	Battleship::Battleship(Point xy, unsigned int w, unsigned int h, const std::string& lab)
		: Window{ xy, cell_w * (h_num * 2 + 2) + x_offset, cell_h * (v_num + 2), lab },
		game_menu{ Point{ 0, 0 }, but_w, but_h, Menu::Kind::horizontal, "" },
		menu_but{ Point{ 0, 0 }, but_w, but_h, "Game menu", cb_menu },
		restart_but{ Point{ 0, 0 }, 0, 0, "New game", cb_restart },
		quit_but{ Point{ 0, 0 }, 0, 0, "Quit", cb_quit },
		help_but{ Point{ but_w, 0 }, but_w, but_h, "Help", cb_help },
		help_box{ Point{ 0, but_h }, cell_w * (h_num * 2 + 2) + x_offset, cell_h * (v_num + 2) - but_h, "" },
		target_group{ Point{ cell_w * (h_num + 2) + x_offset, cell_h * 2 }, cell_w, cell_h, h_num, v_num, "", cb_cell },
		player_field{ Point{ cell_w, cell_h * 2 }, cell_w, cell_h, h_num, v_num, marks },
		target_field{ Point{ cell_w * (h_num + 2) + x_offset, cell_h * 2 }, cell_w, cell_h, h_num, v_num, marks },
		player{ Point{ cell_w, cell_h * 2 }, cell_w * h_num, cell_h * v_num, cell_w, cell_h },
		target{ Point{ cell_w * (h_num + 2) + x_offset, cell_h * 2 }, cell_w * h_num, cell_h * v_num, cell_w, cell_h }
	{
		// Connection of buttons and menu
		game_menu.attach(restart_but);
		game_menu.attach(quit_but);
		// Connection of widgets and window
		attach(target_group);
		attach(game_menu);
		attach(menu_but);
		attach(help_but);
		attach(help_box);
		// Connection of graphics and window
		attach(player_field);
		attach(target_field);
		attach(player);
		attach(target);
		// Initial parameters of widgets
		game_menu.hide();
		help_box.hide();
		help_box.put(help_msg);
		// Initial parameters of graphics
		player_field.set_color(frame);
		player_field.set_fill_color(empty);
		target_field.set_color(frame);
		target_field.set_fill_color(empty);
		player.set_color(miss);
		target.set_color(miss);
		target.set_visibility(Color::Transparency::invisible);
		player.random_location();
		target.random_location();
	}

	// Calls action function of menu button
	void Battleship::cb_menu(Address pw, Address own)
	{
		reference_to<Battleship>(own).menu();
	}

	// Calls action function of cell button
	void Battleship::cb_cell(Address pw, Address own)
	{
		reference_to<Battleship>(own).cell(pw);
	}

	// Calls action function of restart and menu buttons
	void Battleship::cb_restart(Address pw, Address own)
	{
		reference_to<Battleship>(own).restart();
		reference_to<Battleship>(own).menu();
	}

	// Calls action function of quit and menu buttons
	void Battleship::cb_quit(Address pw, Address own)
	{
		reference_to<Battleship>(own).quit();
		reference_to<Battleship>(own).menu();
	}

	// Calls action function of help button
	void Battleship::cb_help(Address pw, Address own)
	{
		reference_to<Battleship>(own).help();
	}

	// Hides menu button and shows game menu or vice versa
	void Battleship::menu()
	{
		// Hide menu button and show game menu
		if (menu_but.visible()) {
			menu_but.hide();
			game_menu.show();
			help_but.move(static_cast<int>(but_w), 0);
		}
		// Hide game menu and show menu button
		else {						// Invisible menu button
			game_menu.hide();
			menu_but.show();
			help_but.move(-static_cast<int>(but_w), 0);
		}
	}

	// Deactivates and updates state of cell at address of pw
	void Battleship::cell(Address pw)
	{
		for (unsigned int i = 0; i < target_group.selection.size(); ++i)
			if (target_group.selection[i].widget() == pw) {
				target_group.selection[i].deactivate();
				return player_shot(i);			// Update state of correspondent cell
			}
	}

	// Redefines parameters of game as at its start
	void Battleship::restart()
	{
		target_group.activate();			// Initial parameters of widgets
		// Initial parameters of graphics
		player_field.set_fill_color(empty);
		target_field.set_fill_color(empty);
		target.set_visibility(Color::Transparency::invisible);
		player.restore();
		target.restore();
		player.random_location();
		target.random_location();
		Window::redraw();
	}

	// Closes game window
	void Battleship::quit()
	{
		Window::hide();
	}

	// Shows help message from help box
	void Battleship::help()
	{
		// Hide help box and enable player to play game
		if (help_box.visible()) {
			restart_but.activate();
			target_group.show();
			help_box.hide();
			// Show graphics
			player_field.set_visibility(Color::Transparency::visible);
			target_field.set_visibility(Color::Transparency::visible);
			player.set_visibility(Color::Transparency::visible);
			player.set_fill_color(Color::Transparency::invisible);
			if (target.is_sunk() || player.is_sunk()) {		// Game ended
				// Show target fleet
				target.set_visibility(Color::Transparency::visible);
				target.set_fill_color(Color::Transparency::invisible);
			}
		}
		// Show help box and disable player to play game
		else {							// Invisible help box
			restart_but.deactivate();
			help_box.show();
			target_group.hide();
			// Hide graphics
			player_field.set_visibility(Color::Transparency::invisible);
			target_field.set_visibility(Color::Transparency::invisible);
			player.set_visibility(Color::Transparency::invisible);
			target.set_visibility(Color::Transparency::invisible);
		}
		Window::redraw();
	}

	enum class Direction {		// Directions from cell
		left = 0, top_left = 1, up = 2, top_right = 3,
		right = 4, down_right = 5, down = 6, down_left = 7
	};

	// Finds next index from ind directed in direction of dir;
	// ind for invalid directions and out of range access
	unsigned int next_index(unsigned int ind, Direction dir)
	{
		switch (dir) {
		case Direction::left:
			return ind % h_num != 0 ? ind - 1 : ind;
		case Direction::top_left:
			return ind / v_num != 0 && ind % h_num != 0 ? ind - h_num - 1 : ind;
		case Direction::up:
			return ind / v_num != 0 ? ind - v_num : ind;
		case Direction::top_right:
			return ind / v_num != 0 && ind % h_num != h_num - 1 ? ind - h_num + 1 : ind;
		case Direction::right:
			return ind % h_num != h_num - 1 ? ind + 1 : ind;
		case Direction::down_right:
			return ind / v_num != v_num - 1 && ind % h_num != h_num - 1 ? ind + h_num + 1 : ind;
		case Direction::down:
			return ind / v_num != v_num - 1 ? ind + v_num : ind;
		case Direction::down_left:
			return ind / v_num != v_num - 1 && ind % h_num != 0 ? ind + h_num - 1 : ind;
		default:
			return ind;		// Invalid direction
		}
	}

	// Determines either cell is empty or not
	bool is_empty_cell(const Rectangle& cell)
	{
		return cell.fill_color().as_int() == empty.as_int();
	}

	// Finds index of next empty cell directed from cell indexed with ind in field
	unsigned int next_empty_index(const Marked_grid& field, unsigned int ind)
	{
		// Possible directions from cell
		std::vector<Direction> directs{ Direction::left, Direction::up, Direction::right, Direction::down };
		std::random_shuffle(directs.begin(), directs.end());
		for (unsigned int i = 0; i < directs.size(); ++i)
			if (is_empty_cell(field[next_index(ind, directs[i])]))
				return next_index(ind, directs[i]);
		return ind;			// None of cells is empty
	}

	// Updates state of empty cell in player field
	void Battleship::target_shot()
	{
		Ship_cell::State res_shot = Ship_cell::State::miss;		// Result of next shot
		unsigned int ind = 0;									// Index of next shooted cell
		static std::vector<unsigned int> hit_inds;				// Serie of current hitted cells		
		// Shot at random cell
		if (hit_inds.size() == 0) {
			do ind = randint(player_field.size() - 1);
			while (!is_empty_cell(player_field[ind]));
		}
		else {	// Shot at around area of hitted ship
			ind = randint(hit_inds.size() - 1);
			// All cells in around area of next cell are shooted
			if (next_empty_index(player_field, hit_inds[ind]) == hit_inds[ind]) {
				hit_inds.erase(std::next(hit_inds.begin(), ind));
				return target_shot();
			}
			ind = next_empty_index(player_field, hit_inds[ind]);
		}
		res_shot = player.shot(player_field[ind].point(0));		// Shot at correspondent cell
		render(player_field, ind, res_shot);
		// Continue to shot if hitted
		if (res_shot == Ship_cell::State::hit && !update()) {
			hit_inds.push_back(ind);
			return target_shot();
		}
	}

	// Updates state of cell indexed with ind in target field
	void Battleship::player_shot(unsigned int ind)
	{
		static Ship_cell::State res_shot = Ship_cell::State::hit;	// Result of next shot
		// Continue to shot if hitted
		if (res_shot == Ship_cell::State::hit) {
			res_shot = target.shot(target_field[ind].point(0));		// Shot at correspondent cell
			render(target_field, ind, res_shot);
			// Pass turn to shot to target if missed
			if (res_shot == Ship_cell::State::miss)
				target_shot();
			else {
				around_area(ind);
				update();
			}
		}
		res_shot = Ship_cell::State::hit;		// Pass turn to shot to player
	}

	// Updates state of game; true if game is over
	bool Battleship::update()
	{
		// Check for winner in battle
		if (target.is_sunk() || player.is_sunk()) {
			target_group.deactivate();
			// Show target fleet
			target.set_visibility(Color::Transparency::visible);
			target.set_fill_color(Color::Transparency::invisible);
			return true;
		}
		return false;
	}

	// Deactivates around area of cell indexed with ind in target group
	void Battleship::around_area(unsigned int ind)
	{
		for (int i = static_cast<int>(Direction::top_left); i <= static_cast<int>(Direction::down_left); i += 2)
			if (target_group.selection[next_index(ind, static_cast<Direction>(i))].active())
				target_group.selection[next_index(ind, static_cast<Direction>(i))].deactivate();
	}

	// Renders shots at cell indexed with ind in field
	void Battleship::render(Marked_grid& field, unsigned int ind, Ship_cell::State shot)
	{
		// Render correspondent cell consider to its state
		if (shot == Ship_cell::State::hit) {
			field[ind].set_fill_color(Color::Color_type::red);
			// Render around area of hitted cell
			for (int i = static_cast<int>(Direction::top_left); i <= static_cast<int>(Direction::down_left); i += 2)
				if (is_empty_cell(field[next_index(ind, static_cast<Direction>(i))]))
					field[next_index(ind, static_cast<Direction>(i))].set_fill_color(Color::Color_type::white);
		}
		else field[ind].set_fill_color(Color::Color_type::white);
		Window::redraw();
	}

	//------------------------------------------------------------------------------

}

//------------------------------------------------------------------------------