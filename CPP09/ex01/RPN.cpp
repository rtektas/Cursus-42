#include "RPN.hpp"
#include <sstream>
#include <cstdlib>
#include <stdexcept>

RPN::RPN()
{
}

int RPN::calculate(const std::string& expression)
{
    std::stringstream ss(expression);
    std::string token;

    while (ss >> token)
    {
        // chiffre
        if (token.length() == 1 && std::isdigit(token[0]))
        {
            _stack.push(token[0] - '0');
        }
        // opérateur
        else if (token == "+" || token == "-" ||
                 token == "*" || token == "/")
        {
            if (_stack.size() < 2)
                throw std::runtime_error("Error");

            int b = _stack.top();
            _stack.pop();

            int a = _stack.top();
            _stack.pop();

            int result;

            if (token == "+")
                result = a + b;
            else if (token == "-")
                result = a - b;
            else if (token == "*")
                result = a * b;
            else
                result = a / b;

            _stack.push(result);
        }
        else
        {
            throw std::runtime_error("Error");
        }
    }

    if (_stack.size() != 1)
        throw std::runtime_error("Error");

    return _stack.top();
}