#ifndef ROBOTOMYREQUESTFORM_HPP
#define ROBOTOMYREQUESTFORM_HPP

#include "AForm.hpp"
#include <cstdlib>  // std::rand, std::srand
#include <ctime>    // std::time

class RobotomyRequestForm : public AForm {
    private:
    std::string target;

    
    public:
    RobotomyRequestForm();
    RobotomyRequestForm(const std::string& target);
    RobotomyRequestForm(const RobotomyRequestForm& other);
    RobotomyRequestForm& operator=(const RobotomyRequestForm& other);
    ~RobotomyRequestForm();

    // methods
    void action() const;
};

#endif // ROBOTOMYREQUESTFORM_HPP
