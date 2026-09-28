#include <iostream>
#include "Bureaucrat.hpp"
#include "Form.hpp"

int main()
{
    std::cout << "===== Create objects =====" << std::endl;
    Bureaucrat boss("Boss", 1);
    Bureaucrat intern("Intern", 150);

    Form tax("TaxForm", 50, 20);

    std::cout << boss << std::endl;
    std::cout << intern << std::endl;
    std::cout << tax << std::endl;

    std::cout << "\n===== Try signing =====" << std::endl;
    intern.signForm(tax); // devrait échouer
    boss.signForm(tax);   // devrait réussir

    std::cout << tax << std::endl;

    std::cout << "\n===== Invalid form test =====" << std::endl;
    try
    {
        Form bad("BadForm", 0, 200);
    }
    catch (std::exception& e)
    {
        std::cout << "Failed to create form: " << e.what() << std::endl;
    }

    return 0;
}
