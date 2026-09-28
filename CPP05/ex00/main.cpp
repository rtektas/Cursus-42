#include "Bureaucrat.hpp"

int main()
{
    Bureaucrat bob("Bob", 42);

    std::cout << bob << std::endl;

    bob.incrementGrade();
    std::cout << bob << std::endl;

    bob.decrementGrade();
    std::cout << bob << std::endl;

    return 0;
}
