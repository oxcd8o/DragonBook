#pragma once

#include <cstdint>
#include <string>
#include <iosfwd>

class Token
{
    public:
        Token();
        Token(const std::string& token, int64_t line, int64_t position);

        operator bool() const { return token_.size(); }

        const std::string& token() const { return token_; }
        int64_t line() const { return line_; }
        int64_t position() const { return position_; }

        friend Token& operator<<(Token& t, int c);

    private:
        std::string token_;
        int64_t line_;
        int64_t position_;
};

std::ostream& operator<<(std::ostream& os, const Token& t);
Token& operator<<(Token& t, int c);