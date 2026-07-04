#include "Bureaucrat.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

int main(void)
{
    // ── Test 1: execute before signing ─────────────────────────────────
    std::cout << "--- Execute unsigned form ---" << std::endl;
    try
    {
        Bureaucrat b("Alice", 1);
        ShrubberyCreationForm s("home");
        b.executeForm(s); // fails: not signed
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }

    // ── Test 2: grade too low to sign ──────────────────────────────────
    std::cout << "--- Grade too low to sign ---" << std::endl;
    try
    {
        Bureaucrat b("Bob", 150);
        ShrubberyCreationForm s("garden");
        b.signForm(s);    // fails: grade 150 > required 145
        b.executeForm(s); // fails: not signed
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }

    // ── Test 3: Shrubbery — sign and execute ───────────────────────────
    std::cout << "--- Shrubbery ---" << std::endl;
    try
    {
        Bureaucrat b("Alice", 1);
        ShrubberyCreationForm s("home");
        b.signForm(s);
        b.executeForm(s); // creates home_shrubbery file
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }

    // ── Test 4: Robotomy ───────────────────────────────────────────────
    std::cout << "--- Robotomy ---" << std::endl;
    try
    {
        Bureaucrat b("Alice", 1);
        RobotomyRequestForm r("Bender");
        b.signForm(r);
        b.executeForm(r); // 50% success
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }

    // ── Test 5: Robotomy grade too low to execute ──────────────────────
    std::cout << "--- Robotomy grade too low to execute ---" << std::endl;
    try
    {
        Bureaucrat b("Bob", 72);
        RobotomyRequestForm r("Bender");
        b.signForm(r);    // grade 72 == required 72, ok
        b.executeForm(r); // fails: grade 72 > required exec 45
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }

    // ── Test 6: Presidential Pardon ────────────────────────────────────
    std::cout << "--- Presidential Pardon ---" << std::endl;
    try
    {
        Bureaucrat b("Alice", 1);
        PresidentialPardonForm p("Marvin");
        b.signForm(p);
        b.executeForm(p);
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }

    // ── Test 7: Presidential Pardon — grade too low to sign ────────────
    std::cout << "--- Presidential Pardon grade too low to sign ---" << std::endl;
    try
    {
        Bureaucrat b("Bob", 50);
        PresidentialPardonForm p("Trillian");
        b.signForm(p);    // fails: grade 50 > required 25
        b.executeForm(p); // fails: not signed
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }

    // ── Test 8: operator<< on AForm ────────────────────────────────────
    std::cout << "--- operator<< ---" << std::endl;
    try
    {
        PresidentialPardonForm p("Ford");
        std::cout << p << "\n";
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }

    return (0);
}