#include <iostream>
#include <ctime>
#include <cstdlib>

#include "AForm.hpp"
#include "Bureaucrat.hpp"
#include "Intern.hpp"

int main()
{
    std::srand(std::time(NULL));

    Intern someIntern;
    Bureaucrat boss("Boss", 1);

    std::cout << "----- Create forms -----" << std::endl;

    AForm* f1 = 0;
    AForm* f2 = 0;
    AForm* f3 = 0;

    try
    {
        f1 = someIntern.makeForm("shrubbery creation", "garden");
        boss.signForm(*f1);
        boss.executeForm(*f1);
    }
    catch (std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    try
    {
        f2 = someIntern.makeForm("robotomy request", "Bender");
        boss.signForm(*f2);
        boss.executeForm(*f2);
    }
    catch (std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    try
    {
        f3 = someIntern.makeForm("presidential pardon", "Marvin");
        boss.signForm(*f3);
        boss.executeForm(*f3);
    }
    catch (std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    std::cout << "\n----- Unknown form -----" << std::endl;
    try
    {
        AForm* bad = someIntern.makeForm("coffee request", "office");
        delete bad;
    }
    catch (std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    delete f1;
    delete f2;
    delete f3;

    return 0;
}