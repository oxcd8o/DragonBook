#include "token.h"

#include <ostream>

Token::Token()
    : token_("")
    , line_(0)
    , position_(0)
{}

Token::Token(const std::string& token, int64_t line, int64_t position)
    : token_(token)
    , line_(line)
    , position_(position)
{}

std::ostream& operator<<(std::ostream& os, const Token& t)
{
    os << "Token('" << t.token() << "', " << t.line() << ", " << t.position() << ")";
    return os;
}

Token& operator<<(Token& t, int c)
{
    t.token_.append(1, static_cast<char>(c));
    return t;
}