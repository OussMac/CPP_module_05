#include "ShrubberyCreationForm.hpp"

ShrubberyCreationForm::ShrubberyCreationForm() : AForm("ShrubberyCreationForm", 145, 137), target("default") {}

ShrubberyCreationForm::ShrubberyCreationForm(const std::string& target) : AForm("ShrubberyCreationForm", 145, 137), target(target) {}

ShrubberyCreationForm::ShrubberyCreationForm(const ShrubberyCreationForm& other) : AForm(other), target(other.target) {}

ShrubberyCreationForm& ShrubberyCreationForm::operator=(const ShrubberyCreationForm& other)
{
    if (this != &other)
    {
        AForm::operator=(other);
        this->target = other.target;
    }
    return *this;
}

ShrubberyCreationForm::~ShrubberyCreationForm() {}

void ShrubberyCreationForm::action() const
{
    std::ofstream file((target + "_shrubbery").c_str());
    if (!file)
    {
        std::cerr << "Error: could not create shrubbery file\n";
        return ;
    }
    file << "       *       " << "\n"
         << "      ***      " << "\n"
         << "     *****     " << "\n"
         << "    *******    " << "\n"
         << "   *********   " << "\n"
         << "  ***********  " << "\n"
         << "       |       " << "\n"
         << "       |       " << "\n";
    file.close();
}
