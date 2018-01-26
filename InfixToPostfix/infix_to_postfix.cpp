#include "parser.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <unordered_map>

int main()
{
    std::string line;
    while (std::cin) {
        std::getline(std::cin, line);
        try {
            std::cout << static_cast<std::string>(Parser(line)) << std::endl;
        } catch (const Parser::ParsingError& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    return 0;
}