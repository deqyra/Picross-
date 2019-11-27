#include <iostream>
#include <vector>
#include <memory>

#include "tools/cli/cli_menu.hpp"
#include "tools/cli/cli_command.hpp"
#include "tools/cli/command_sequence.hpp"
#include "tools/cli/menu_command.hpp"
#include "picross_cli/picross_cli_state.hpp"
#include "picross_cli/cli_create_grid_command.hpp"
#include "picross_cli/cli_load_grid_command.hpp"
#include "picross_cli/cli_solve_grid_command.hpp"
#include "picross_cli/cli_save_grid_command.hpp"
#include "picross_cli/cli_modify_grid_command.hpp"

#include "cmake_defines.hpp"

using Picross::PicrossCLIState;
using Picross::CLISolveCommand;
using Picross::CLIModifyGridCommand;
using Picross::CLISaveGridCommand;
using Picross::CLILoadGridCommand;
using Picross::CLICreateGridCommand;

using PicrossCLICommand = CLICommand<PicrossCLIState>;
using PicrossCommandPtr = std::shared_ptr<PicrossCLICommand>;
using PicrossMenuCommand = MenuCommand<PicrossCLIState>;
using PicrossCommandSequence = CommandSequence<PicrossCLIState>;
using PicrossCLIMenu = CLIMenu<PicrossCLIState>;

// Build CLI app structure and run it.
void runCLIApp();

int main(int argc, char** argv)
{
	runCLIApp();
	return 0;
}

void runCLIApp()
{
	// Commands available from the "Manipulate grid" menu:
	std::vector<PicrossCommandPtr> manipulateGridMenuCommands = {
		std::make_shared<CLISolveCommand>(),						// Attempt to solve the grid using a solver
		std::make_shared<CLIModifyGridCommand>(),					// Modify the grid manually
		std::make_shared<CLISaveGridCommand>()						// Save the grid to disk
	};

	// "Manipulate grid" menu, consisting of the commands instantiated above.
	PicrossCLIMenu manipulateGridMenu = PicrossCLIMenu(manipulateGridMenuCommands, "Grid manipulation menu", "Close grid", nullptr);

	// Command sequence for the "Create new grid" option of the main menu:
	std::vector<PicrossCommandPtr> newGridCommands = {
		std::make_shared<CLICreateGridCommand>(),					// 1 - Run the grid creation command.
		std::make_shared<PicrossMenuCommand>(manipulateGridMenu)	// 2 - Display the "Manipulate grid" menu (defined above).
	};

	// Command sequence for the "Open XML grid" option of the main menu:
	std::vector<PicrossCommandPtr> openGridCommands = {
		std::make_shared<CLILoadGridCommand>(),						// 1 - Run the command to read a grid from an XML file.
		std::make_shared<PicrossMenuCommand>(manipulateGridMenu)	// 2 - Display the "Manipulate grid" menu (defined above).
	};

	// Commands available from the main menu: 2 command sequences created from the command vectors defined above.
	std::vector<PicrossCommandPtr> mainMenuCommands = {
		std::make_shared<PicrossCommandSequence>(newGridCommands, "Create new grid"),
		std::make_shared<PicrossCommandSequence>(openGridCommands, "Open grid from XML file")
	};

	// Creation of the main menu using the command vector defined right above.
	PicrossCLIMenu mainMenu = PicrossCLIMenu(mainMenuCommands, "Main menu", "Exit", nullptr);

	std::cout << PROJECT_NAME << " " << PROJECT_VERSION << " " << COPYRIGHT_NOTICE << std::endl;

	// Create a new state and run the menu on it.
	PicrossCLIState state = PicrossCLIState();
	mainMenu.show(state);

	std::cout << "Thank you for using Picross Engine. Bye!" << std::endl;
}