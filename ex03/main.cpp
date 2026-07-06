#include "Bureaucrat.hpp"
#include "Intern.hpp"

int main(void)
{
    Intern intern;
    Bureaucrat b("Alice", 1);

    // Test 1: create and use each valid form name 
    std::cout << "--- Valid form names ---" << std::endl;
    std::string names[3] = {"shrubbery creation", "robotomy request", "presidential pardon"};
    for (int i = 0; i < 3; i++)
    {
        AForm* form = intern.makeForm(names[i], "Bender");
        if (form)
        {
            b.signForm(*form);
            b.executeForm(*form);
            delete form;
        }
        std::cout << std::endl;
    }

    // Test 2: unknown form name
    std::cout << "--- Unknown form name ---" << std::endl;
    AForm* bad = intern.makeForm("burn the building down", "HQ");
    if (bad)
        delete bad;

    // Test 3: multiple robotomies in the same run
    std::cout << "\n--- Multiple robotomies same run ---" << std::endl;
    for (int i = 0; i < 4; i++)
    {
        AForm* r = intern.makeForm("robotomy request", "Subject");
        b.signForm(*r);
        b.executeForm(*r);
        delete r;
    }

    return (0);
}