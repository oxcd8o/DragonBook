#pragma once

#include "token.h"

#include <sstream>
#include <string>
#include <stdexcept>

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

        explicit operator std::string() const;

    private:
        Token peek();
        Token get();

        inline bool eol() const { return it_ == line_.end(); }

        bool expression();
        bool factor();

        void throwSyntaxError(const std::string& error, Token t) const;

        std::string line_;
        decltype(std::declval<std::string>().begin()) it_;

        std::stringstream representation_;
};