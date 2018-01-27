#include "error.h"

#include <iostream>

int main()
{
    try {
        throw SyntaxError() << 123 << " " << 456;
    } catch (const Error& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}