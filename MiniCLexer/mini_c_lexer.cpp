#include "error.h"
#include "parser.h"
#include "token.h"

#include <iostream>
#include <sstream>

int main()
{
    try {
        std::string data = "int b = 123 +   765; //123456 66 33 ss\nif (a456 <= \"896\");";
        std::stringstream ss(data);

        Parser p(&ss);
        do {
            std::cout << p.peek() << std::endl;
            p.get();
        } while (p);
    } catch (const Error& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}