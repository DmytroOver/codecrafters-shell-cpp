#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::string command;

  do
  {
      std::cout << "$ ";

      std::cin >> command;

      if (!command.empty())
      {
          std::cout << command << ": command not found" << std::endl;
      }
  } while (true);

  return 0;
}
