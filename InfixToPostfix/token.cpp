#include "token.h"

#include <iostream>

Token::Token(char token, int64_t position)
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

Token::Type Token::type() const
{
    return type_;
}

int64_t Token::position() const
{
    return position_;
}

Token::operator std::string() const 
{
    return {token_};
}

Token::operator bool() const
{
    return token_ != '\0';
}

std::ostream& operator<<(std::ostream& os, const Token& t)
{
    os << static_cast<std::string>(t);
    return os;
}