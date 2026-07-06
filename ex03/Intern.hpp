#ifndef INTERN_HPP
#define INTERN_HPP

#include <string>
#include "AForm.hpp"

class Intern {
    public:
    // OCF
    Intern();
    Intern(const Intern& other);
    Intern& operator=(const Intern& other);
    ~Intern();
    // methods
    AForm* makeForm(const std::string& formName, const std::string& target) const;
};

#endif // INTERN_HPP