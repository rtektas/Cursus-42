#include <iostream>
#include <ctime>
#include <cstdlib>

#include "Bureaucrat.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

int main()
{
    std::srand(std::time(NULL));

    Bureaucrat boss("Boss", 1);
    Bureaucrat mid("Mid", 50);
    Bureaucrat intern("Intern", 150);

    ShrubberyCreationForm sh("home");
    RobotomyRequestForm ro("Bender");
    PresidentialPardonForm pr("Marvin");

    std::cout << boss << std::endl;
    std::cout << mid << std::endl;
    std::cout << intern << std::endl;

    std::cout << "\n--- Try sign/exec Shrubbery with Intern ---" << std::endl;
    intern.signForm(sh);
    intern.executeForm(sh);

    std::cout << "\n--- Sign Shrubbery with Mid then exec with Mid ---" << std::endl;
    mid.signForm(sh);
    mid.executeForm(sh);

    std::cout << "\n--- Robotomy: sign with Mid? exec with Mid? ---" << std::endl;
    mid.signForm(ro);     // devrait réussir (72)
    mid.executeForm(ro);  // devrait échouer (45) car mid=50

    std::cout << "\n--- Robotomy: exec with Boss ---" << std::endl;
    boss.executeForm(ro);

    std::cout << "\n--- Presidential: sign with Mid? exec with Boss ---" << std::endl;
    mid.signForm(pr);     // échoue (25)
    boss.signForm(pr);    // réussit
    boss.executeForm(pr); // réussit

    return 0;
}