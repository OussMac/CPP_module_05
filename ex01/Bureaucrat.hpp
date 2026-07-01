#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP

#include <string>
#include <iostream>
#include "Form.hpp"

class Bureaucrat {
private:
    const std::string name;
    int grade;
public:
    // OCF
    Bureaucrat();
    Bureaucrat(const std::string& b_name, int b_grade); // not in ocf
    Bureaucrat(const Bureaucrat& other);
    Bureaucrat& operator=(const Bureaucrat &other);
    ~Bureaucrat();
    // getters
    const std::string& getName() const;
    int getGrade() const;
    // grade methods
    void incrementGrade();
    void decrementGrade();
    // exception functions
    class GradeTooHighException : public std::exception {
        public:
        const char* what() const throw();
    };
    class GradeTooLowException : public std::exception {
        public:
        const char* what() const throw();
    };
    // form method
    void signForm(Form &form); // might need to pass form obj here.
};

std::ostream& operator<<(std::ostream &out,const Bureaucrat& b);

#endif // BUREAUCRAT_HPP