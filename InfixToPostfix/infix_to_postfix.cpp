#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <unordered_map>

/*
 * statement -> <EOF>
 *              expression
 * 
 * expression -> factor <OpSimple> expression
 *               factor
 * 
 * factor -> <digit> <OpPrivileged> factor
 *           <digit> <EOF>
 * 
 * <digit> -> '0' | '1' | '2' | ... | '9'
 * 
 * <OpSimple> -> '+' | '-'
 * 
 * <OpPrivileged> -> '*' | '/'
 * 
 * <EOF> -> '\0'
 * 
 */

class Token
{
    public:
        enum class Type { None, Digit, OperationPrivileged, OperationSimple, Other };

    public:
        Token(char token = '\0', int64_t position = 0)
            : token_(token)
            , position_(position)
            , type_(Token::Type::None)
        {
            if (isdigit(token_)) {
                type_ = Token::Type::Digit;
            } else if (token_ == '*' || token_ == '/') {
                type_ = Token::Type::OperationPrivileged;
            } else if (token_ == '-' || token_ == '+') {
                type_ = Token::Type::OperationSimple;
            } else if (token_ != '\0') {
                type_ = Token::Type::Other;
            }
        }

        Token::Type type() const { return type_; }
        int64_t position() const { return position_; }

        explicit operator std::string() const { return {token_}; }
        operator bool() const { return token_ != '\0'; }

    private:
        char token_;
        size_t position_;
        Token::Type type_;
};

std::ostream& operator<<(std::ostream& os, const Token& t)
{
    os << static_cast<std::string>(t);
    return os;
}

class Parser
{
    public:
        struct ParsingError: public std::runtime_error
        {
            ParsingError(const std::string& error)
                : std::runtime_error(error)
            {}
        };

    public:
        Parser(const std::string& line);

        Token peek();
        Token get();
        inline bool eol() const { return it_ == line_.end(); }

        bool expression();
        bool factor();

        void throwSyntaxError(const std::string& error, Token t) const;

        explicit operator std::string() const { return representation_.str(); }

    private:
        std::string line_;
        decltype(std::declval<std::string>().begin()) it_;

        std::stringstream representation_;
};

Parser::Parser(const std::string& line)
    : line_(line)
    , it_(line_.begin())
{
    Token start = peek();
    if (!expression()) {
        throwSyntaxError("Expected single digit or expression", start);
    }
}

bool Parser::expression()
{
    std::cerr << "Parser::expression()" << std::endl;

    factor();

    Token op = peek();

    if (op.type() == Token::Type::OperationSimple) {
        get();
        Token tmp = peek();
        if (!expression()) {
            throwSyntaxError("Expected expression", tmp);
        }
        representation_ << op;
    } else if (op) {
        return false;
    }

    return true;
}

bool Parser::factor()
{
    Token left = get();
    Token op = peek();

    if (!left || left.type() != Token::Type::Digit) {
        throwSyntaxError("Expected digit", left);
    }
    representation_ << left;

    if (op.type() == Token::Type::OperationPrivileged) {
        get();
        factor();
        representation_ << op;
    }

    return true;
}

Token Parser::peek()
{
    if (eol()) {
        return {};
    }
    return {*it_, it_ - line_.begin()};
}

Token Parser::get()
{
    Token t = peek();
    if (t) {
        ++it_;
    }
    return t;
}

void Parser::throwSyntaxError(const std::string& error, Token t) const
{
    std::stringstream ss;
    ss << error << " at line:\n'" << line_ << "'\n"
       << std::string(t.position() + 1, ' ') << '^';

    throw ParsingError(ss.str());
}

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