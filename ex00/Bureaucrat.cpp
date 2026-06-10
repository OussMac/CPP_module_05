#include "Bureaucrat.hpp"



Bureaucrat::Bureaucrat() : name("Bureaucrat John"), grade(50){}

Bureaucrat::Bureaucrat(const std::string& b_name, int b_grade) : name(b_name) {
    if (b_grade < 1)
        throw GradeTooHighException();
    else if (b_grade > 150)
        throw GradeTooLowException();
    this->grade = b_grade;
}

Bureaucrat::Bureaucrat(const Bureaucrat& other) : name(other.name){
    this->grade = other.grade;
}

Bureaucrat& Bureaucrat::operator=(const Bureaucrat &other){
    if (this != &other)
        this->grade = other.grade;
    return *this;
}

Bureaucrat::~Bureaucrat(){};

const std::string& Bureaucrat::getName() const{
    return (this->name);
}

int Bureaucrat::getGrade() const{
    return (this->grade);
}

void Bureaucrat::incrementGrade(){
    if (grade - 1 < 1)
        throw GradeTooHighException();
    this->grade--;
}

void Bureaucrat::decrementGrade(){
    if (grade + 1 > 150)
        throw GradeTooLowException();
    this->grade++;
}

// exception what implementation explain

const char* Bureaucrat::GradeTooHighException::what() const throw(){
    return ("Grade Is Too High.");
}

const char* Bureaucrat::GradeTooLowException::what() const throw(){
    return ("Grade Is Too Low.");
}


std::ostream& operator<<(std::ostream &out,const Bureaucrat& b){
    out << b.getName() << ", bureaucrat grade " << b.getGrade() << ".";
    return out;
}
