#include "Command.h"
#include "CommandParser.h"

#include <iostream>
#include <string>
#if _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

std::string getInput(const CommandParser& commandParser)
{
#if !_WIN32
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
#endif

    int c;
    std::string input;
    std::cout << "$ ";

#if _WIN32
    while ((c = _getch()) != EOF)
#else
    while (read(STDIN_FILENO, &c, 1) == 1 && c != EOF)
#endif
    {
        char ch = static_cast<char>(c);
        switch (ch)
        {
        case '\r':
        case '\n':
        {
            std::cout << std::endl;
            return input;
        }
        case '\b':
        case 127:
        {
            if (!input.empty())
            {
                std::cout << "\b \b";
                input.pop_back();
            }
            break;
        }
#if _WIN32
        case 0:
        case 0xE0:
        {
            // TODO: do something with the special char
            ch = _getch();
            break;
        }
#endif
        case '\t':
        {
            std::string autocompletionResult = commandParser.autocomplete(input);
            if (!autocompletionResult.empty())
            {
                autocompletionResult.push_back(' ');
                std::cout << "\r$ " << autocompletionResult;
                input = autocompletionResult;
            }
            else
            {
                std::cout << '\a';
            }
            break;
        }
        default:
        {
            input += ch;
            std::cout << ch;
            break;
        }
        }
    }
    return input;
}

int main() {
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    CommandParser commandParser;
    int result = 0;
    std::unique_ptr<Command> command = nullptr;
    CommandType commandType = CommandType::UNKNOWN;

    do
    {
        std::string input = getInput(commandParser);

        if (!input.empty())
        {
            command = commandParser.getCommand(input);
            if (command != nullptr)
            {
                commandType = command->getType();
                result = command->execute();
            }
        }
    } while (commandType != CommandType::EXIT);

    return result;
}
