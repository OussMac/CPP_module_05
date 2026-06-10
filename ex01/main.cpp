#include "Bureaucrat.hpp"

int main(void)
{
    // Test Form invalid construction
    std::cout << "--- Form Grade Too High on Construction ---" << std::endl;
    try
    {
        Form f("TooHigh", 0, 10);
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << "\n";
    }

    std::cout << "--- Form Grade Too Low on Construction ---" << std::endl;
    try
    {
        Form f("TooLow", 151, 10);
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << "\n";
    }

    // Test operator<< on form
    std::cout << "--- Form operator<< ---" << std::endl;
    try
    {
        Form f("TaxForm", 50, 25);
        std::cout << f << "\n";
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << "\n";
    }

    // Test successful signing
    std::cout << "--- Successful Signing ---" << std::endl;
    try
    {
        Bureaucrat b("Alice", 30);
        Form f("TaxForm", 50, 25);
        std::cout << f << "\n";
        b.signForm(f);
        std::cout << f << "\n"; // isSigned should now be true
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << "\n";
    }

    // Test failed signing - bureaucrat grade too low
    std::cout << "--- Failed Signing ---" << std::endl;
    try
    {
        Bureaucrat b("Bob", 100);
        Form f("TaxForm", 50, 25);
        b.signForm(f);
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << "\n";
    }

    return (0);
}