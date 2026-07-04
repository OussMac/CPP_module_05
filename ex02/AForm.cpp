#include "AForm.hpp"
#include "Bureaucrat.hpp"

AForm::AForm(const std::string& f_name, int f_reqGrade, int f_execGrade)
    : name(f_name), isSigned(false), reqGrade(f_reqGrade), execGrade(f_execGrade)
{
    if (f_reqGrade < 1 || f_execGrade < 1)
        throw AForm::GradeTooHighException();
    if (f_reqGrade > 150 || f_execGrade > 150)
        throw AForm::GradeTooLowException();
}

AForm::AForm() : name("AForm"), isSigned(false), reqGrade(20), execGrade(10) {}

AForm::AForm(const AForm& other)
    : name(other.name), isSigned(other.isSigned),
      reqGrade(other.reqGrade), execGrade(other.execGrade) {}

AForm& AForm::operator=(const AForm& other)
{
    if (this != &other)
        this->isSigned = other.isSigned;
    return (*this);
}

AForm::~AForm() {} // NEW: virtual destructor body — empty but must exist

const std::string& AForm::getName() const
{
    return this->name;
}

bool AForm::getSignature() const
{
    return this->isSigned;
}

int AForm::getReqGrade() const
{
    return this->reqGrade;
}

int AForm::getExecGrade() const
{
    return this->execGrade;
}

void AForm::beSigned(const Bureaucrat& b)
{
    if (b.getGrade() <= reqGrade)
        this->isSigned = true;
    else
        throw AForm::GradeTooLowException();
}

// NEW: execute() — guards live here so subclasses don't repeat them
// checks: is form signed? is executor grade high enough?
// then calls action() which goes through the vtable to the right subclass
void AForm::execute(Bureaucrat const& executor) const
{
    if (!this->getSignature())
        throw AForm::FormNotSignedException();
    if (executor.getGrade() > this->getExecGrade())
        throw AForm::GradeTooLowException();
    this->action(); // vtable dispatch → calls the correct subclass action()
}

const char* AForm::GradeTooHighException::what() const throw()
{
    return "Form Grade Is Too High.";
}

const char* AForm::GradeTooLowException::what() const throw()
{
    return "Form Grade Is Too Low.";
}

// NEW
const char* AForm::FormNotSignedException::what() const throw()
{
    return "Form is not signed.";
}

std::ostream& operator<<(std::ostream& out, const AForm& f)
{
    out << "Form: " << f.getName()
        << " | signed: "              << f.getSignature()
        << " | sign grade required: " << f.getReqGrade()
        << " | exec grade required: " << f.getExecGrade();
    return out;
}