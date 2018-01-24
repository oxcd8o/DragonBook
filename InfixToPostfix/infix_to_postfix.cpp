#include <iostream>
#include <string>
#include <exception>
#include <sstream>

/*
 * "2 + 3 - 4" -> "23+4-"
 * "2 + 3 * 4" -> "34*2+"
 * 
 * 
 * 
 */

class ParsingError: public std::exception
{
    public:
        ParsingError(const std::string& line, size_t position, const char* error)
        {
            std::stringstream ss;
            ss << error << " at line:\n'" << line << "'\n"
               << std::string(position, ' ') << '^';
            what_ = std::move(ss.str());
        }

        virtual const char* what() const noexcept override { return what_.c_str(); }

    private:
        std::string what_;

};

class Token
{
    public:
        Token(char token = '\0'): token_(token) {}
        explicit operator std::string() const { return std::string({token_}); }
    private:
        char token_;
};

std::ostream& operator<<(std::ostream& os, const Token& t)
{
    os << static_cast<std::string>(t);
    return os;
}

class Parser
{
    public:
        Parser(const std::string& line);

        char peek();
        inline bool eol() const { return it_ == line_.end(); }

        Token digit();
        Token operationPrivileged();
        Token operationSimple();
        void expression();

        explicit operator std::string() const { return representation_.str(); }

    private:
        std::string line_;
        std::string::const_iterator it_;

        std::stringstream representation_;
};

Parser::Parser(const std::string& line)
    : line_(line)
    , it_(line_.begin())
{
    expression();
}

void Parser::expression()
{
    representation_ << digit();

    if (eol()) {
        return;
    }

    Token op = operationSimple();
    expression();
    
    representation_ << op;
}

Token Parser::digit()
{
    char c = peek();
    if (!isdigit(c)) {
        throw ParsingError(line_, it_ - line_.begin(), "Digit expected");
    }
    return Token(c);
}

Token Parser::operationSimple()
{
    char c = peek();
    if (c != '-' && c != '+') {
        throw ParsingError(line_, it_ - line_.begin(), "'+' or '-' expected");
    }
    return Token(c);
}

char Parser::peek()
{
    if (eol()) {
        throw ParsingError(line_, line_.size(), "Parser went out of line");
    }

    char c = *it_;
    ++it_;
    return c;
}

std::ostream& operator<<(std::ostream& os, const Parser& p)
{
    os << static_cast<std::string>(p);
    return os;
}

int main()
{
    std::string line;
    while (std::cin) {
        std::getline(std::cin, line);
        try {
            std::cout << Parser(line) << std::endl;
        } catch (const ParsingError& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    return 0;
}