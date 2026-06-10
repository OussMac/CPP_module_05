#include "Form.hpp"
#include "Bureaucrat.hpp"

Form::Form(const std::string& f_name, int f_reqGrade, int f_execGrade)
    : name(f_name), isSigned(false), reqGrade(f_reqGrade), execGrade(f_execGrade)
{
    if (f_reqGrade < 1 || f_execGrade < 1)
        throw Form::GradeTooHighException();
    if (f_reqGrade > 150 || f_execGrade > 150)
        throw Form::GradeTooLowException();
}

Form::Form(): name("Form"), isSigned(false), reqGrade(20), execGrade(10) {}

Form::Form(const Form& other) : name(other.name), isSigned(other.isSigned), reqGrade(other.reqGrade), execGrade(other.execGrade) {}

Form& Form::operator=(const Form& other){
    if (this != &other)
    {
        this->isSigned = other.isSigned;
    }
    return (*this);
}

Form::~Form(){}

const std::string& Form::getName() const
{
    return this->name;
}

bool Form::getSignature() const
{
    return this->isSigned;
}

int Form::getReqGrade() const
{
    return this->reqGrade;
}

int Form::getExecGrade() const
{
    return this->execGrade;
}


void Form::beSigned(const Bureaucrat& b)
{
    if (b.getGrade() <= reqGrade)
    {
        this->isSigned = true;
    }
    else
    {
        throw Form::GradeTooLowException();
    }
}

const char *Form::GradeTooHighException::what() const throw()
{
    return "Form Grade Is Too High.";
}

const char *Form::GradeTooLowException::what() const throw()
{
    return "Form Grade Is Too Low.";
}

std::ostream& operator<<(std::ostream &out, const Form& f)
{
    out << "Form: " << f.getName() << " | signed: " << f.getSignature() << " | sign grade required: " << f.getReqGrade() << " | exec grade required: " << f.getExecGrade();
    return out;
}