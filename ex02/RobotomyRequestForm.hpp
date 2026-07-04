#ifndef ROBOTOMYREQUESTFORM_HPP
#define ROBOTOMYREQUESTFORM_HPP

#include "AForm.hpp"
#include <cstdlib>  // std::rand, std::srand
#include <ctime>    // std::time

class RobotomyRequestForm : public AForm {
    private:
    std::string target;

    protected:
    void action() const; // drilling noise + 50% success/failure

    public:
    RobotomyRequestForm();
    RobotomyRequestForm(const std::string& target);
    RobotomyRequestForm(const RobotomyRequestForm& other);
    RobotomyRequestForm& operator=(const RobotomyRequestForm& other);
    ~RobotomyRequestForm();
};

#endif // ROBOTOMYREQUESTFORM_HPP
