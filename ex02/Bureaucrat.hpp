#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP

#include <string>
#include <iostream>
#include "AForm.hpp" // CHANGED: was Form.hpp

class Bureaucrat {
    private:
    const std::string   name;
    int                 grade;

    public:
    // OCF
    Bureaucrat();
    Bureaucrat(const std::string& b_name, int b_grade);
    Bureaucrat(const Bureaucrat& other);
    Bureaucrat& operator=(const Bureaucrat& other);
    ~Bureaucrat();

    // Getters
    const std::string&  getName() const;
    int                 getGrade() const;

    // Grade methods
    void incrementGrade();
    void decrementGrade();

    // Form methods
    void signForm(AForm& form);                    // CHANGED: was Form&
    void executeForm(AForm const& form) const;     // NEW

    // Exceptions
    class GradeTooHighException : public std::exception {
        public:
        const char* what() const throw();
    };
    class GradeTooLowException : public std::exception {
        public:
        const char* what() const throw();
    };
};

std::ostream& operator<<(std::ostream& out, const Bureaucrat& b);

#endif // BUREAUCRAT_HPP