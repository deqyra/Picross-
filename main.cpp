#include <iostream>
#include <vector>
#include <memory>

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

#include "cmake_defines.hpp"

void runCLIApp();

int main(int argc, char** argv)
{
	runCLIApp();
	return 0;
}

void runCLIApp()
{
	Picross::CLIState state = Picross::CLIState(std::cin, std::cout, std::cerr);

	std::vector<std::shared_ptr<Picross::CLICommand>> manipulateGridMenuCommands = {
		std::make_shared<Picross::SolveCommand>(),
		std::make_shared<Picross::ModifyGridCommand>(),
		std::make_shared<Picross::SaveGridCommand>()
	};

	Picross::CLIMenu manipulateGridMenu = Picross::CLIMenu(state, manipulateGridMenuCommands, "Grid manipulation menu", "Close grid", nullptr);

	std::vector<std::shared_ptr<Picross::CLICommand>> newGridCommands =	{
		std::make_shared<Picross::CreateGridCommand>(),
		std::make_shared<Picross::MenuCommand>(manipulateGridMenu)
	};

	std::vector<std::shared_ptr<Picross::CLICommand>> openGridCommands = {
		std::make_shared<Picross::LoadGridCommand>(),
		std::make_shared<Picross::MenuCommand>(manipulateGridMenu)
	};

	std::vector<std::shared_ptr<Picross::CLICommand>> mainMenuCommands = {
		std::make_shared<Picross::CommandSequence>(newGridCommands, "Create new grid"),
		std::make_shared<Picross::CommandSequence>(openGridCommands, "Open grid from XML file")
	};

	Picross::CLIMenu mainMenu = Picross::CLIMenu(state, mainMenuCommands, "Main menu", "Exit", nullptr);

	state.out() << PROJECT_NAME << " " << PROJECT_VERSION << " " << COPYRIGHT_NOTICE << std::endl << std::endl;
	mainMenu.show();

	state.out() << "Thank you for using Picross Engine. Bye!" << std::endl;
}