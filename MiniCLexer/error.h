#pragma once

#include <string>
#include <sstream>
#include <ostream>
#include <exception>

class Error: public std::exception
{
    public:
        Error(std::string prefix = "Error")
            : text_(std::move(prefix += ": "))
        {}

        virtual const char* what() const noexcept override { return text_.c_str(); }
        void append(const std::string& s) { text_ += s; }

    private:
        std::string text_;
};

template <class T>
Error&& operator<<(Error&& error, const T& argument)
{
    std::stringstream ss;
    ss << argument;
    error.append(ss.str());
    return std::forward<Error>(error);
}

#define MAKE_ERROR(name) class name: public Error { public: name(): Error(#name) {} };
MAKE_ERROR(SyntaxError)
MAKE_ERROR(ParsingError)
#undef MAKE_ERROR