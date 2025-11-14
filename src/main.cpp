#include <iostream>
#include <string>

#include "Command.h"
#include "CommandParser.h"

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::string input;
  CommandParser commandParser;
  int result = 0;
  std::unique_ptr<Command> command = nullptr;
  CommandType commandType = CommandType::UNKNOWN;

  do
  {
      std::cout << "$ ";

      std::getline(std::cin, input);

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
