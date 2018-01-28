#pragma once

#include "token.h"

#include <istream>
#include <unordered_set>

class Parser
{
    public:
        Parser(std::istream* is);

        Token peek();
        Token get();

        inline operator bool() const { return currentToken_; }

    private:
        static bool isServiceSymbol(char c);
        static bool isFollowupSymbol(char c, char f);

        inline int peekChar() const { return is_->peek(); }
        int getChar();

        std::istream* is_;
        struct Position {
            int64_t line;
            int64_t symbol;
        } position_;
        Token currentToken_;
};
