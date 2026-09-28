#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>

class RPN
{
private:
    std::stack<int> _stack;

public:
    RPN();

    int calculate(const std::string& expression);
};

#endif