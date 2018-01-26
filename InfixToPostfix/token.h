#pragma once

#include <cstdint>

#include <iosfwd>

class Token
{
    public:
        enum class Type { None, Digit, OperationPrivileged, OperationSimple, Other };

    public:
        Token(char token = '\0', int64_t position = 0);

        Token::Type type() const;
        int64_t position() const;

        explicit operator std::string() const;
        operator bool() const;

    private:
        char token_;
        size_t position_;
        Token::Type type_;
};

std::ostream& operator<<(std::ostream& os, const Token& t);