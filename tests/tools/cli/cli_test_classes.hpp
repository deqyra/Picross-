#ifndef TESTS__TOOLS__CLI__CLI_TEST_CLASSES_HPP
#define TESTS__TOOLS__CLI__CLI_TEST_CLASSES_HPP

#include "../../../tools/cli/cli_menu.hpp"
#include "../../../tools/cli/cli_command.hpp"
#include "../../../tools/cli/cli_streams.hpp"
#include "../../../tools/cli/cli_input.hpp"
#include "../../../tools/cli/menu_command.hpp"
#include "../../../tools/cli/command_sequence.hpp"

class TestCLIState {};

using TestMenu = CLIMenu<TestCLIState>;
using TestCommand = CLICommand<TestCLIState>;
using TestCommandPtr = std::shared_ptr<TestCommand>;
using TestMenuCommand = MenuCommand<TestCLIState>;
using TestCommandSequence = CommandSequence<TestCLIState>;

class TestCommand1 : public TestCommand
{
    public:
        TestCommand1() : TestCommand() {}
        virtual std::string getTooltip() {return "TestCommand1";}
        virtual int run(TestCLIState& state, CLIStreams& streams)
        {
            streams.out() << "TestCommand1 was run.\n";
            return 0;
        }
};

class TestCommand2 : public TestCommand
{
    public:
        TestCommand2() : TestCommand() {}
        virtual std::string getTooltip() {return "TestCommand2";}
        virtual int run(TestCLIState& state, CLIStreams& streams)
        {
            streams.out() << "TestCommand2 was run.\n";
            return 0;
        }
};

class TestExitCommand : public TestCommand
{
    public:
        TestExitCommand() : TestCommand() {}
        virtual std::string getTooltip() {return "Exit";}
        virtual int run(TestCLIState& state, CLIStreams& streams)
        {
            streams.out() << "Successful exit.\n";
            return 0;
        }
};

TestMenu makeBasicTestMenu();
TestCommandSequence makeBasicTestCommandSequence();

#endif//TESTS__TOOLS__CLI__CLI_TEST_CLASSES_HPP