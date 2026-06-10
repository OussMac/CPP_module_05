#include "Bureaucrat.hpp"

int main(void)
{
    std::cout << "--- Valid Construction & operator<< ---" << std::endl;
    try
    {
        Bureaucrat b("Alice", 75);
        std::cout << b << "\n";
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    std::cout << "--- Grade Too High on Construction ---" << std::endl;
    try
    {
        Bureaucrat b("Bob", 0);
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    std::cout << "--- Grade Too Low on Construction ---" << std::endl;
    try
    {
        Bureaucrat b("Charlie", 151);
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    std::cout << "--- Increment At Grade 1 ---" << std::endl;
    try
    {
        Bureaucrat b("Dave", 1);
        std::cout << b << "\n";
        b.incrementGrade();
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    std::cout << "--- Decrement At Grade 150 ---" << std::endl;
    try
    {
        Bureaucrat b("Eve", 150);
        std::cout << b << "\n";
        b.decrementGrade();
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    std::cout << "--- Valid Increment & Decrement ---" << std::endl;
    try
    {
        Bureaucrat b("Frank", 50);
        std::cout << b << "\n";
        b.incrementGrade();
        std::cout << b << "\n";
        b.decrementGrade();
        std::cout << b << "\n";
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    return (0);
}