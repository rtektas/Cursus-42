#include "ShrubberyCreationForm.hpp"
#include <fstream>

ShrubberyCreationForm::ShrubberyCreationForm(const std::string& target)
: AForm("ShrubberyCreationForm", 145, 137), target(target) {}

ShrubberyCreationForm::ShrubberyCreationForm(const ShrubberyCreationForm& other)
: AForm(other), target(other.target) {}

ShrubberyCreationForm& ShrubberyCreationForm::operator=(const ShrubberyCreationForm& other)
{
    if (this != &other)
    {
        AForm::operator=(other);
        target = other.target;
    }
    return *this;
}

ShrubberyCreationForm::~ShrubberyCreationForm() {}

void ShrubberyCreationForm::executeAction() const
{
    std::ofstream out((target + "_shrubbery").c_str());
    if (!out)
        return;

    out << "       _-_\n";
    out << "    /~~   ~~\\\n";
    out << " /~~         ~~\\\n";
    out << "{               }\n";
    out << " \\  _-     -_  /\n";
    out << "   ~  \\\\ //  ~\n";
    out << "_- -   | | _- _\n";
    out << "  _ -  | |   -_\n";
    out << "      // \\\\\n";
}