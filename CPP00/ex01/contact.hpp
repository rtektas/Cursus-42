#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <string>

class Contact
{
private:
    std::string first;
    std::string last;
    std::string nickname;
    std::string phoneNumber;
    std::string darkestSecret;

public:
    Contact();

    void fill();
    void printSearch(int index) const;
    void printDetails() const;
};

#endif
