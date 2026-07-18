#include "Command.h"
#include "CommandRunner.h"

#include <iostream>
#include <string>
#if _WIN32
#include <conio.h>
#include <io.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

std::string getInput(const CommandRunner& commandRunner)
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

    bool doubleTab = false;

#if _WIN32
    while ((c = _getch()) != EOF)
#else
    while (read(STDIN_FILENO, &c, 1) == 1 && c != EOF)
#endif
    {
        char ch = static_cast<char>(c);
#if _WIN32
        switch(c)
#else
        switch (ch)
#endif
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
                std::cout << "\b \b" << std::flush;
                input.pop_back();
            }
            break;
        }
#if _WIN32
        case 0:
        case 0xE0:
        {
            // TODO: handle correct arrows behaviour
            c = _getch();
            switch (c)
            {
            case 75: // left arrow
            {
                //std::cout << "\033[1D" << std::flush;
                break;
            }
            case 77: // right arrow
            {
                //std::cout << "\033[1C" << std::flush;
                break;
            }
            }
            break;
        }
#else
        case '\033':
        {
            if (read(STDIN_FILENO, &c, 1) != -1)
            {
                read(STDIN_FILENO, &c, 1);
                ch = static_cast<char>(c);
                switch (ch)
                {
                case 'A':
                    input = commandRunner.getPreviousCommand();
                    std::cout << "\033[2K\r$ " << input << std::flush;
                    break;
                case'B':
                    input = commandRunner.getNextCommand();
                    std::cout << "\033[2K\r$ " << input << std::flush;
                    break;
                case 'C':
                    break;
                case 'D':
                    break;
                default:
                    break;
                }
            }
            break;
        }
#endif
        case '\t':
        {
            std::vector<std::string> completionResults = commandRunner.autocomplete(input);
            if (completionResults.empty())
            {
                std::cout << '\a' << std::flush;
            }
            else if (completionResults.size() == 1)
            {
                std::string completionResult = completionResults[0];
                completionResult.push_back(' ');
                std::cout << "\r$ " << completionResult << std::flush;
                input = completionResult;
            }
            else if (!doubleTab)
            {
                if (completionResults[1].find(completionResults[0]) != std::string::npos)
                {
                    std::cout << "\r$ " << completionResults[0] << std::flush;
                    input = completionResults[0];
                }
                else
                {
                    doubleTab = true;
                    std::cout << '\a' << std::flush;
                }
            }
            else
            {
                doubleTab = false;
                std::cout << std::endl;
                for (const auto& res : completionResults)
                {
                    std::cout << res << "  ";
                }
                std::cout << std::endl << "$ " << input;
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

    CommandRunner commandRunner;

    do
    {
        std::string input = getInput(commandRunner);

        if (!input.empty())
        {
            commandRunner.run(input);
        }
    } while (true);

    return 0;
}
