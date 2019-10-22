#include <iostream>
#include <vector>

#include "cli/cli_menu.hpp"
#include "cli/cli_command.hpp"
#include "cli/cli_state.hpp"
#include "cli/commands/command_sequence.hpp"
#include "cli/commands/menu_command.hpp"
#include "cli/commands/create_grid_command.hpp"
#include "cli/commands/load_grid_command.hpp"
#include "cli/commands/solve_grid_command.hpp"
#include "cli/commands/save_grid_command.hpp"
#include "cli/commands/modify_grid_command.hpp"

void runCLIApp();

int main(int argc, char** argv)
{
	runCLIApp();
	return 0;
}

void runCLIApp()
{
	Picross::CLIState state = Picross::CLIState(std::cin, std::cout, std::cerr);

	std::vector<Picross::CLICommand*> manipulateGridMenuCommands = {
		new Picross::SolveCommand(),
		new Picross::ModifyGridCommand(),
		new Picross::SaveGridCommand()
	};

	Picross::CLIMenu manipulateGridMenu = Picross::CLIMenu(state, manipulateGridMenuCommands, "Grid manipulation menu", "Close grid", nullptr);

	std::vector<Picross::CLICommand*> newGridCommands =	{
		new Picross::CreateGridCommand(),
		new Picross::MenuCommand(manipulateGridMenu)
	};

	std::vector<Picross::CLICommand*> openGridCommands = {
		new Picross::LoadGridCommand(),
		new Picross::MenuCommand(manipulateGridMenu)
	};

	std::vector<Picross::CLICommand*> mainMenuCommands = {
		new Picross::CommandSequence(newGridCommands, "Create a grid"),
		new Picross::CommandSequence(openGridCommands, "Open a grid from XML file")
	};

	Picross::CLIMenu mainMenu = Picross::CLIMenu(state, mainMenuCommands, "Main menu", "Exit", nullptr);

	mainMenu.show();
}