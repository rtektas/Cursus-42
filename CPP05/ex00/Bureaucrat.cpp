#include "Bureaucrat.hpp"


Bureaucrat::Bureaucrat(const std::string& name, int grade)
    : name(name), grade(grade)
{
    std::cout << "Bureaucrat " << name << " created with grade " << grade << std::endl;
}

// Canonical form
Bureaucrat::Bureaucrat(const Bureaucrat& other)
    : name(other.name), grade(other.grade)
{
    std::cout << "Bureaucrat copy constructor called" << std::endl;
}

Bureaucrat& Bureaucrat::operator=(const Bureaucrat& other)
{
    std::cout << "Bureaucrat assignment operator called" << std::endl;
    if (this != &other)
        this->grade = other.grade;
    return *this;
}

Bureaucrat::~Bureaucrat()
{
    std::cout << "Bureaucrat " << name << " destroyed" << std::endl;
}


std::string Bureaucrat::getName() const
{
    return name;
}

int Bureaucrat::getGrade() const
{
    return grade;
}


void Bureaucrat::incrementGrade()
{
    std::cout << name << " increment grade" << std::endl;
    grade--;
}

void Bureaucrat::decrementGrade()
{
    std::cout << name << " decrement grade" << std::endl;
    grade++;
}

const char* Bureaucrat::GradeTooHighException::what() const throw()
{
    return "Grade too high!";
}

const char* Bureaucrat::GradeTooLowException::what() const throw()
{
    return "Grade too low!";
}

std::ostream& operator<<(std::ostream& os, const Bureaucrat& b)
{
    os << b.getName() << ", bureaucrat grade " << b.getGrade();
    return os;
}
