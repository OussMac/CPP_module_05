#ifndef AFORM_HPP
#define AFORM_HPP

#include <string>
#include <iostream>
#include <exception>

class Bureaucrat;

class AForm {
    private:
    const std::string   name;
    bool                isSigned;
    const int           reqGrade;
    const int           execGrade;
    
    public:
    // OCF
    AForm();
    AForm(const std::string& f_name, int f_reqGrade, int f_execGrade);
    AForm(const AForm& other);
    AForm& operator=(const AForm& other);
    virtual ~AForm();
    
    // Getters
    const std::string&  getName() const;
    bool                getSignature() const;
    int                 getReqGrade() const;
    int                 getExecGrade() const;
    
    // Form methods
    void beSigned(const Bureaucrat& b);

    // execute with checks
    void execute(Bureaucrat const& executor) const;
    // action for each subclass
    virtual void action() const = 0;
    
    // Exceptions
    class GradeTooHighException : public std::exception {
        public:
        const char* what() const throw();
    };
    class GradeTooLowException : public std::exception {
        public:
        const char* what() const throw();
    };
    class FormNotSignedException : public std::exception { // NEW: thrown when executing unsigned form
        public:
        const char* what() const throw();
    };
};

std::ostream& operator<<(std::ostream& out, const AForm& f);

#endif // AFORM_HPP