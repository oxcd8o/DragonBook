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
    Token newToken(position_.line, position_.symbol);

    while (isspace(peekChar())) {
        getChar();
    }

    int c = peekChar();

    /*
     * First we read next appropriate token.
     * Number = [0-9]+
     * Id = [a-zA-Z][a-zA-Z0-9]*
     * Service = defined by isServiceSymbol()
     */
    if (isdigit(c)) {
        do {
            newToken << c;
            getChar();
            c = peekChar();
        } while (isdigit(c));
        newToken.type(Token::Type::Number);
    } else if (isalpha(c)) {
        do {
            newToken << c;
            getChar();
            c = peekChar();
        } while (isalnum(c));
        newToken.type(Token::Type::Id);
    } else if (isServiceSymbol(c)) {
        newToken << c;
        getChar();
        int f = peekChar();
        if (isFollowupSymbol(c, f)) {
            newToken << f;
            getChar();
        }
        newToken.type(Token::Type::Service);
    } else if (c == EOF) {
        // pass
    } else {
        throw ParsingError()
            << "Unexpected symbol " << c << " at "
            << position_.line << ";" << position_.symbol;
    }

    /*
     * Second we need to consume more characters in two special cases:
     * comments (only // style right now) and string literals.
     */
    if (newToken.type() == Token::Type::Service) {
        if (newToken.token() == "//") {
            for (c = peekChar(); c != '\n' && c != EOF; c = peekChar()) {
                newToken << c;
                getChar();
            }
            newToken.type(Token::Type::Comment);
        } else if (newToken.token() == "\"") {
            newToken.erase();
            newToken.type(Token::Type::Literal);
            bool finished = false;
            for (c = peekChar(); !finished && c != EOF; c = peekChar()) {
                newToken << c;
                getChar();
                if (c == '"') {
                    finished = true;
                }
            }

            if (!finished) {
                throw ParsingError()
                    << "String literal without closing <\"> at "
                    << newToken.line() << ";" << newToken.position();
            }
        }
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
        {'&', {'&'}},
        {'=', {'='}},
        {'/', {'/', '*'}},
        {'*', {'/'}}
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