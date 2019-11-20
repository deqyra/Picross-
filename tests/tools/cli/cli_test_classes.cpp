#include "cli_test_classes.hpp"

#include "../../../tools/cli/cli_menu.hpp"
#include "../../../tools/cli/cli_command.hpp"
#include "../../../tools/cli/cli_streams.hpp"
#include "../../../tools/cli/cli_input.hpp"
#include "../../../tools/cli/menu_command.hpp"
#include "../../../tools/cli/command_sequence.hpp"

TestMenu makeBasicTestMenu()
{
    std::vector<TestCommandPtr> commands = {
        std::make_shared<TestCommand1>(),
        std::make_shared<TestCommand2>()
    };

    return TestMenu(commands, "Test menu", "Exit", std::make_shared<TestExitCommand>());
}

TestCommandSequence makeBasicTestCommandSequence()
{
    TestMenu testMenu = makeBasicTestMenu();
    
    std::vector<TestCommandPtr> sequenceCommands = {
        std::make_shared<TestMenuCommand>(testMenu),
        std::make_shared<TestCommand1>(),
        std::make_shared<TestCommand2>()
    };

    return TestCommandSequence(sequenceCommands, "Test sequence");
}
