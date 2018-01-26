#pragma once

#include "token.h"

#include <sstream>
#include <string>

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