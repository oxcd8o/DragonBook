#pragma once

#include <cstdint>
#include <string>
#include <iosfwd>

class Token
{
    public:
        enum class Type { None, Number, Id, Service };

    public:
        Token();
        Token(int64_t line, int64_t position);

        operator bool() const { return token_.size(); }

        const std::string& token() const { return token_; }
        int64_t line() const { return line_; }
        int64_t position() const { return position_; }
        Type type() const { return type_; }

        void type(Type t) { type_ = t; }

        friend Token& operator<<(Token& t, int c);

    private:
        std::string token_;
        int64_t line_;
        int64_t position_;
        Type type_;
};

std::ostream& operator<<(std::ostream& os, const Token::Type& t);
std::ostream& operator<<(std::ostream& os, const Token& t);
Token& operator<<(Token& t, int c);