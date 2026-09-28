#include "AForm.hpp"
#include "Bureaucrat.hpp"

AForm::AForm() : name("AForm"), isSigned(false), gradeToSign(150), gradeToExecute(150) {}

AForm::AForm(const std::string& name, int gSign, int gExec)
: name(name), isSigned(false), gradeToSign(gSign), gradeToExecute(gExec)
{
    if (gSign < 1 || gExec < 1) throw GradeTooHighException();
    if (gSign > 150 || gExec > 150) throw GradeTooLowException();
}

AForm::AForm(const AForm& other)
: name(other.name), isSigned(other.isSigned),
  gradeToSign(other.gradeToSign), gradeToExecute(other.gradeToExecute) {}

AForm& AForm::operator=(const AForm& other)
{
    if (this != &other)
        isSigned = other.isSigned;
    return *this;
}

AForm::~AForm() {}

std::string AForm::getName() const { return name; }
bool AForm::getIsSigned() const { return isSigned; }
int AForm::getGradeToSign() const { return gradeToSign; }
int AForm::getGradeToExecute() const { return gradeToExecute; }

const char* AForm::GradeTooHighException::what() const throw() { return "Form grade too high"; }
const char* AForm::GradeTooLowException::what() const throw()  { return "Form grade too low"; }
const char* AForm::FormNotSignedException::what() const throw(){ return "Form not signed"; }

void AForm::beSigned(const Bureaucrat& b)
{
    if (b.getGrade() > gradeToSign)
        throw GradeTooLowException();
    isSigned = true;
}

void AForm::execute(const Bureaucrat& executor) const
{
    if (!isSigned)
        throw FormNotSignedException();
    if (executor.getGrade() > gradeToExecute)
        throw GradeTooLowException();
    executeAction();
}

std::ostream& operator<<(std::ostream& os, const AForm& f)
{
    os << "Form " << f.getName()
       << " (sign " << f.getGradeToSign()
       << ", exec " << f.getGradeToExecute()
       << "), signed: " << (f.getIsSigned() ? "yes" : "no");
    return os;
}
