#include "Intern.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"
#include <iostream>

// helpers ---
static AForm* createShrubbery(const std::string& target)
{
    return new ShrubberyCreationForm(target);
}

static AForm* createRobotomy(const std::string& target)
{
    return new RobotomyRequestForm(target);
}

static AForm* createPardon(const std::string& target)
{
    return new PresidentialPardonForm(target);
}
// helpers ---

Intern::Intern() {}

Intern::Intern(const Intern& other)
{
    (void)other;
}

Intern& Intern::operator=(const Intern& other)
{
    (void)other;
    return *this;
}

Intern::~Intern() {}

AForm* Intern::makeForm(const std::string& formName, const std::string& target) const
{
    const std::string formNames[3] = {
        "shrubbery creation",
        "robotomy request",
        "presidential pardon"
    };
    AForm* (*creators[3])(const std::string&) = {
        createShrubbery,
        createRobotomy,
        createPardon
    };

    for (int i = 0; i < 3; i++)
    {
        if (formNames[i] == formName)
        {
            AForm* form = creators[i](target);
            std::cout << "Intern creates " << form->getName() << "\n";
            return (form);
        }
    }
    std::cout << "Intern couldn't find a form named \"" << formName << "\"\n";
    return (NULL);
}