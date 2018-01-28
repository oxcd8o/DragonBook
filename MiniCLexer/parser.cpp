#include "parser.h"
#include "error.h"

#include <sstream>
#include <unordered_map>
#include <unordered_set>

Parser::Parser(std::istream* is)
    : is_(is)
    , position_({0, 0})
{
    get();
}

Token Parser::peek()
{
    return currentToken_;
}

Token Parser::get()
{
    Token newToken("", position_.line, position_.symbol);

    while (isspace(peekChar())) {
        getChar();
    }

    int c = peekChar();

    if (isdigit(c)) {
        do {
            newToken << c;
            getChar();
            c = peekChar();
        } while (isdigit(c));
    } else if (isalpha(c)) {
        do {
            newToken << c;
            getChar();
            c = peekChar();
        } while (isalnum(c));
    } else if (isServiceSymbol(c)) {
        newToken << c;
        getChar();
        int f = peekChar();
        if (isFollowupSymbol(c, f)) {
            newToken << f;
            getChar();
        }
    } else if (c == EOF) {
        // pass
    } else {
        throw ParsingError() << "Unexpected symbol " << c << " at "
                             << position_.line << ";" << position_.symbol;
    }

    Token oldToken = currentToken_;
    currentToken_ = newToken;
    return oldToken;
}

bool Parser::isServiceSymbol(char c)
{
    static const std::unordered_set<char> serviceSymbols {
        '!', '%', '&', '*', '(', ')', '-', '+', '=',
        '\'', '"', ',', ';', '/', '[', ']', '{', '}',
        '|', '<', '>'
    };

    return serviceSymbols.count(c);
}

bool Parser::isFollowupSymbol(char c, char f)
{
    static const std::unordered_map<char, std::unordered_set<char>> followupSymbols {
        {'<', {'<', '='}},
        {'>', {'>', '='}},
        {'-', {'-'}},
        {'+', {'+'}},
        {'!', {'='}},
        {'|', {'|'}},
        {'&', {'&'}}
    };

    return followupSymbols.count(c) && followupSymbols.at(c).count(f);
}

int Parser::getChar()
{
    char c = is_->get();
    if (c == '\n') {
        ++position_.line;
        position_.symbol = 0;
    } else {
        ++position_.symbol;
    }
    return c;
}