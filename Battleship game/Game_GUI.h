#pragma once
#include "GraphicsLib/GUI.h"
#include "Game_graph.h"

//------------------------------------------------------------------------------

namespace Graph_lib {

	// Invariant: w >= 0, h >= 0
	class Battleship : public Window {
	public:
		// Construction
		Battleship(Point xy, unsigned int w, unsigned int h, const std::string& lab);

	private:
		// Callback functions
		static void cb_menu(Address pw, Address own);
		static void cb_cell(Address pw, Address own);
		static void cb_restart(Address pw, Address own);
		static void cb_quit(Address pw, Address own);
		static void cb_help(Address pw, Address own);

		// Action functions
		void menu();
		void cell(Address pw);
		void restart();
		void quit();
		void help();
		void target_shot();
		void player_shot(unsigned int i);
		bool update();
		void around_area(unsigned int i);
		void render(Marked_grid& grid, unsigned int i, Ship_cell::State shot);

		// Widgets
		Menu game_menu;			// Menu of new game button and quit button
		Button menu_but;		// Menu button
		Button restart_but;		// New game button
		Button quit_but;		// Quit button
		Button help_but;		// Help button
		Text_box help_box;
		// Graphics
		Group target_group;
		Marked_grid player_field, target_field;
		Fleet player, target;
	};

}

//------------------------------------------------------------------------------