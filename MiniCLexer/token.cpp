#include "token.h"

#include <ostream>
#include <unordered_map>

Token::Token()
    : line_(0)
    , position_(0)
    , type_(Type::None)
{}

Token::Token(int64_t line, int64_t position)
    : line_(line)
    , position_(position)
    , type_(Type::None)
{}

std::ostream& operator<<(std::ostream& os, const Token::Type& t)
{
    static const std::unordered_map<int64_t, std::string> typeNames {
        {static_cast<int64_t>(Token::Type::None),    "Token::Type::None"},
        {static_cast<int64_t>(Token::Type::Number),  "Token::Type::Number"},
        {static_cast<int64_t>(Token::Type::Id),      "Token::Type::Id"},
        {static_cast<int64_t>(Token::Type::Service), "Token::Type::Service"}
    };

    os << typeNames.at(static_cast<int64_t>(t));
    return os;
}

std::ostream& operator<<(std::ostream& os, const Token& t)
{
    os << "Token('" << t.token() << "', " << t.line() << ", " << t.position() 
       << ", " << t.type() << ")";
    return os;
}

Token& operator<<(Token& t, int c)
{
    t.token_.append(1, static_cast<char>(c));
    return t;
}