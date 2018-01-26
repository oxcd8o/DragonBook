#include "parser.h"

/*
 * statement -> <EOF>
 *              expression
 * 
 * expression -> factor <OpSimple> expression
 *               factor
 * 
 * factor -> <digit> <OpPrivileged> factor
 *           <digit> <EOF>
 * 
 * <digit> -> '0' | '1' | '2' | ... | '9'
 * 
 * <OpSimple> -> '+' | '-'
 * 
 * <OpPrivileged> -> '*' | '/'
 * 
 * <EOF> -> '\0'
 * 
 */

Parser::Parser(const std::string& line)
    : line_(line)
    , it_(line_.begin())
{
    Token start = peek();
    if (!expression()) {
        throwSyntaxError("Expected single digit or expression", start);
    }
}

bool Parser::expression()
{
    factor();

    Token op = peek();

    if (op.type() == Token::Type::OperationSimple) {
        get();
        Token tmp = peek();
        if (!expression()) {
            throwSyntaxError("Expected expression", tmp);
        }
        representation_ << op;
    } else if (op) {
        return false;
    }

    return true;
}

bool Parser::factor()
{
    Token left = get();
    Token op = peek();

    if (!left || left.type() != Token::Type::Digit) {
        throwSyntaxError("Expected digit", left);
    }
    representation_ << left;

    if (op.type() == Token::Type::OperationPrivileged) {
        get();
        factor();
        representation_ << op;
    }

    return true;
}

Token Parser::peek()
{
    if (eol()) {
        return {};
    }
    return {*it_, it_ - line_.begin()};
}

Token Parser::get()
{
    Token t = peek();
    if (t) {
        ++it_;
    }
    return t;
}

void Parser::throwSyntaxError(const std::string& error, Token t) const
{
    std::stringstream ss;
    ss << error << " at line:\n'" << line_ << "'\n"
       << std::string(t.position() + 1, ' ') << '^';

    throw ParsingError(ss.str());
}