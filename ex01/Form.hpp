#ifndef FORM_HPP
#define FORM_HPP

#include <string>
#include <iostream>
#include <exception>

class Bureaucrat;

class Form {
    private:
    const std::string name;
    bool        isSigned;
    const int   reqGrade;
    const int   execGrade;
    public:
    Form(const std::string& f_name, int f_reqGrade, int f_execGrade);
    // OCF
    Form();
    Form(const Form& other);
    Form &operator=(const Form& other);
    ~Form();
    // Getters
    const std::string& getName() const;
    bool getSignature() const;
    int getReqGrade() const;
    int getExecGrade() const;
    // form methods.
    void beSigned(const Bureaucrat& b);
    
    // Exceptions
    class GradeTooHighException : public std::exception {
        public:
        const char *what() const throw();
    };
    class GradeTooLowException : public std::exception {
        public:
        const char* what() const throw();
    };
};

std::ostream &operator<<(std::ostream &out,const Form& f);


#endif // FORM_HPP