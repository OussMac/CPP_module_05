#ifndef SHRUBBERYCREATIONFORM_HPP
#define SHRUBBERYCREATIONFORM_HPP

#include "AForm.hpp"
#include <fstream>

class ShrubberyCreationForm : public AForm {
    private:
    std::string target;

    public:
    ShrubberyCreationForm(const std::string& target); // constructor from a string
    // OCF
    ShrubberyCreationForm();
    ShrubberyCreationForm(const ShrubberyCreationForm& other);
    ShrubberyCreationForm& operator=(const ShrubberyCreationForm& other);
    ~ShrubberyCreationForm();
    // methods
    void action() const;
};

#endif // SHRUBBERYCREATIONFORM_HPP
