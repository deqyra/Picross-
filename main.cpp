#include <iostream>
#include <vector>
#include <memory>

#include "tools/cli/cli_menu.hpp"
#include "tools/cli/cli_command.hpp"
#include "cli/picross_cli_state.hpp"
#include "tools/cli/command_sequence.hpp"
#include "tools/cli/menu_command.hpp"
#include "cli/create_grid_command.hpp"
#include "cli/load_grid_command.hpp"
#include "cli/solve_grid_command.hpp"
#include "cli/save_grid_command.hpp"
#include "cli/modify_grid_command.hpp"

#include "cmake_defines.hpp"

using Picross::PicrossCLIState;
using Picross::SolveCommand;
using Picross::ModifyGridCommand;
using Picross::SaveGridCommand;
using Picross::LoadGridCommand;
using Picross::CreateGridCommand;

using PicrossCLICommand = CLICommand<PicrossCLIState>;
using PicrossCommandPtr = std::shared_ptr<PicrossCLICommand>;
using PicrossMenuCommand = MenuCommand<PicrossCLIState>;
using PicrossCommandSequence = CommandSequence<PicrossCLIState>;

void runCLIApp();

int main(int argc, char** argv)
{
	runCLIApp();
	return 0;
}

void runCLIApp()
{
	PicrossCLIState state = PicrossCLIState();

	std::vector<PicrossCommandPtr> manipulateGridMenuCommands = {
		std::make_shared<SolveCommand>(),
		std::make_shared<ModifyGridCommand>(),
		std::make_shared<SaveGridCommand>()
	};

	CLIMenu<PicrossCLIState> manipulateGridMenu = CLIMenu<PicrossCLIState>(manipulateGridMenuCommands, "Grid manipulation menu", "Close grid", nullptr);

	std::vector<PicrossCommandPtr> newGridCommands =	{
		std::make_shared<CreateGridCommand>(),
		std::make_shared<PicrossMenuCommand>(manipulateGridMenu)
	};

	std::vector<PicrossCommandPtr> openGridCommands = {
		std::make_shared<LoadGridCommand>(),
		std::make_shared<PicrossMenuCommand>(manipulateGridMenu)
	};

	std::vector<PicrossCommandPtr> mainMenuCommands = {
		std::make_shared<PicrossCommandSequence>(newGridCommands, "Create new grid"),
		std::make_shared<PicrossCommandSequence>(openGridCommands, "Open grid from XML file")
	};

	CLIMenu<PicrossCLIState> mainMenu = CLIMenu<PicrossCLIState>(mainMenuCommands, "Main menu", "Exit", nullptr);

	std::cout << PROJECT_NAME << " " << PROJECT_VERSION << " " << COPYRIGHT_NOTICE << std::endl;
	mainMenu.show(state);

	std::cout << "Thank you for using Picross Engine. Bye!" << std::endl;
}