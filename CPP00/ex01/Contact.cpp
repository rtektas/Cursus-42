#include <iostream>
#include <iomanip>     // pour std::setw
#include "Contact.hpp"

static bool isNumber(const std::string &str)
{
    if (str.empty())
        return false;

    for (size_t i = 0; i < str.length(); i++)
    {
        if (!std::isdigit(str[i]))
            return false;
    }
    return true;
}

static bool isEmpty(const std::string &str)
{
    for (size_t i = 0; i < str.length(); i++)
    {
        if (!std::isspace(str[i]))
            return false;
    }
    return true;
}

static std::string formatField(const std::string &str)
{
    if (str.length() > 10)
        return str.substr(0, 9) + ".";           // coupe et ajoute un point
    if (str.length() < 10)
        return std::string(10 - str.length(), ' ') + str; // aligne à droite
    return str;                                   // exactement 10
}

Contact::Contact()
{

}

void Contact::fill()
{
    // FIRST NAME
    while (true)
    {
        std::cout << "First name: ";
        std::getline(std::cin, this->first);

        if (!isEmpty(this->first))
            break;

        std::cout << "first name cannot be empty. Please try again." << std::endl;
    }

    // LAST NAME
    while (true)
    {
        std::cout << "Last name: ";
        std::getline(std::cin, this->last);

        if (!isEmpty(this->last))
            break;

        std::cout << "last name cannot be empty. Please try again." << std::endl;
    }

    // NICKNAME
    while (true)
    {
        std::cout << "Nickname: ";
        std::getline(std::cin, this->nickname);

        if (!isEmpty(this->nickname))
            break;

        std::cout << "nickname cannot be empty. Please try again." << std::endl;
    }

    while (true)
    {
        std::cout << "Phone number: ";
        std::getline(std::cin, this->phoneNumber);

        if (!isEmpty(this->phoneNumber) && isNumber(this->phoneNumber))
            break;

        std::cout << "The phone number must contain only digits and cannot be empty. Please try again." << std::endl;
    }

    while (true)
    {
        std::cout << "Darkest secret: ";
        std::getline(std::cin, this->darkestSecret);

        if (!isEmpty(this->darkestSecret))
            break;

        std::cout << "The secret cannot be empty. Please try again." << std::endl;
    }
}



void Contact::printSearch(int index) const
{
    std::cout << "|" << std::setw(10) << index
              << "|" << formatField(this->first)
              << "|" << formatField(this->last)
              << "|" << formatField(this->nickname)
              << "|" << std::endl;
}

void Contact::printDetails() const
{
    std::cout << "First name:     " << this->first << std::endl;
    std::cout << "Last name:      " << this->last << std::endl;
    std::cout << "Nickname:       " << this->nickname << std::endl;
    std::cout << "Phone number:   " << this->phoneNumber << std::endl;
    std::cout << "Darkest secret: " << this->darkestSecret << std::endl;
}

