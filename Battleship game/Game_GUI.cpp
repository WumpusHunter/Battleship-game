#include "Game_GUI.h"
#include "RandGenerator/Generator.h"
#include <iostream>

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
	const std::string help_msg = R"( Rules from the classic Russian version:
1. Locate your fleet on your grid in such a way that the ships don't overlap
each other or go consecutive using their sides or angles. In our case, this
is done randomly.
2. Decide with the opposite player who is to shot first. In our case, at the
start of the game you shoot first, then the first shots the one who won the
previous game.
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
		: Window{ xy, w, h, lab },
		game_menu{ Point{ 0, 0 }, but_w, but_h, Menu::Kind::horizontal, "" },
		menu_but{ Point{ 0, 0 }, but_w, but_h, "Game menu", cb_menu },
		restart_but{ Point{ 0, 0 }, 0, 0, "New game", cb_restart },
		quit_but{ Point{ 0, 0 }, 0, 0, "Quit", cb_quit },
		help_but{ Point{ but_w, 0 }, but_w, but_h, "Help", cb_help },
		help_box{ Point{ 0, but_h }, w, h - but_h, "" },
		target_group{ Point{ cell_w * (h_num + 2) + x_offset, cell_h * 2 }, cell_w, cell_h, h_num, v_num, "", cb_cell },
		player_field{ Point{ cell_w, cell_h * 2 }, cell_w, cell_h, h_num, v_num, "0123456789ABCDEFGHIJ" },
		target_field{ Point{ cell_w * (h_num + 2) + x_offset, cell_h * 2 }, cell_w, cell_h, h_num, v_num, "0123456789ABCDEFGHIJ" },
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
		player_field.set_color(Color::Color_type::black);
		player_field.set_fill_color(Color::Color_type::blue);
		target_field.set_color(Color::Color_type::black);
		target_field.set_fill_color(Color::Color_type::blue);
		target.set_visibility(Color::Transparency::invisible);
		player.random_location();
		target.random_location();
	}

	// Callback function of menu button
	void Battleship::cb_menu(Address pw, Address own)
	{
		reference_to<Battleship>(own).menu();
	}

	// Callback function of cell buttons
	void Battleship::cb_cell(Address pw, Address own)
	{
		reference_to<Battleship>(own).cell(pw);
	}

	// Callback function of restart button
	void Battleship::cb_restart(Address pw, Address own)
	{
		reference_to<Battleship>(own).restart();
		reference_to<Battleship>(own).menu();
	}

	// Callback function of quit button
	void Battleship::cb_quit(Address pw, Address own)
	{
		reference_to<Battleship>(own).quit();
		reference_to<Battleship>(own).menu();
	}

	// Callback function of help button
	void Battleship::cb_help(Address pw, Address own)
	{
		reference_to<Battleship>(own).help();
	}

	// Hides menu button and shows game menu or vise versa
	void Battleship::menu()
	{
		// Hide menu button and show game menu
		if (menu_but.visible()) {
			menu_but.hide();
			game_menu.show();
			help_but.move(static_cast<int>(but_w), 0);
			std::cout << "Hide menu button and showed game menu\n";
		}
		// Hide game menu and show menu button
		else {						// Invisible menu button
			game_menu.hide();
			menu_but.show();
			help_but.move(-static_cast<int>(but_w), 0);
			std::cout << "Hide game menu and showed menu button\n";
		}
	}

	// Deactivates and updates state of cell at pw
	void Battleship::cell(Address pw)
	{
		for (unsigned int i = 0; i < target_group.selection.size(); ++i)
			if (target_group.selection[i].widget() == pw) {
				target_group.selection[i].deactivate();
				return shot(i);			// Update state of correspondent cell
			}
	}

	// Redefines parameters of game as at its start
	void Battleship::restart()
	{
		target_group.activate();			// Initial parameters of widgets
		// Initial parameters of graphics
		player_field.set_fill_color(Color::Color_type::blue);
		target_field.set_fill_color(Color::Color_type::blue);
		player.restore();
		target.restore();
		player.random_location();
		target.random_location();
		Window::redraw();
		std::cout << "Started new game\n";
	}

	// Closes game window
	void Battleship::quit()
	{
		Window::hide();
		std::cout << "Quitted from game\n";
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
			std::cout << "Hide help box, and showed graphics and buttons\n";
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
			std::cout << "Hide graphics and buttons, and showed help box\n";
		}
		Window::redraw();
	}

	// Updates state of cell undex index i
	void Battleship::shot(unsigned int i)
	{
		static Ship_cell::State player_shot = Ship_cell::State::hit, target_shot = Ship_cell::State::hit;
		// Continue to shot if hitted
		if (player_shot == Ship_cell::State::hit) {
			player_shot = target.shot(target_field[i].point(0));		// Shot at correspondent cell
			render(target_field, i, player_shot);
			// Pass turn to shot to target if missed
			if (player_shot == Ship_cell::State::miss) {
				target_shot = Ship_cell::State::hit;
				std::cout << "Player shot and missed\n";
				shot(i);
			}
			else {
				std::cout << "Player shot and hitted\n";
				update();
			}
		}
		// Continue to shot if hitted
		else while (target_shot == Ship_cell::State::hit && !update()) {
			// Shot at same cell is prevented
			do i = static_cast<unsigned int>(randint(static_cast<int>(player_field.size() - 1)));
			while (player_field[i].fill_color().as_int() != static_cast<unsigned int>(Color::Color_type::blue));
			target_shot = player.shot(player_field[i].point(0));		// Shot at random cell
			render(player_field, i, target_shot);
			// Pass turn to shot to player if missed
			if (target_shot == Ship_cell::State::miss) {
				player_shot = Ship_cell::State::hit;
				std::cout << "Target shot and missed\n";
			}
			else std::cout << "Target shot and hitted\n";
		}
	}

	// Updates state of game; true if game continues, false if it's over
	bool Battleship::update()
	{
		// Check for winner in battle
		if (target.is_sunk()) {			// Player won
			target_group.deactivate();
			std::cout << "Player won\n";
			return true;
		}
		if (player.is_sunk()) {			// Target won
			target_group.deactivate();
			std::cout << "Target won\n";
			return true;
		}
		return false;
	}

	// Manages rendering of shots at grid's cell under index i
	void Battleship::render(Marked_grid& grid, unsigned int i, Ship_cell::State shot)
	{
		// Render correspondent cell consider to its state
		if (shot == Ship_cell::State::hit)
			grid[i].set_fill_color(Color::Color_type::red);
		else grid[i].set_fill_color(Color::Color_type::white);
		Window::redraw();
	}

	//------------------------------------------------------------------------------

}

//------------------------------------------------------------------------------