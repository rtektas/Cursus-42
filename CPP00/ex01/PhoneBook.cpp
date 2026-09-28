#include <iostream>
#include <sstream>
#include <iomanip>
#include "PhoneBook.hpp"

PhoneBook::PhoneBook() : size(0), nextIndex(0)
{
}

void PhoneBook::addContact()
{
    std::cout << "=== add a new contact ===" << std::endl;

    this->contacts[this->nextIndex].fill();

    if (this->size < 8)
        this->size++;

    this->nextIndex = (this->nextIndex + 1) % 8;

    std::cout << "Contact saved! " << std::endl;
}

static int readIndex()
{
    std::string line;

    std::cout << "Enter a index: ";
    if (!std::getline(std::cin, line))
        return -1;

    if (line.length() != 1 || !std::isdigit(line[0]))
        return -1;

    return line[0] - '0';
}

void PhoneBook::searchContact() const
{
    if (this->size == 0)
    {
        std::cout << "The phonebook is empty." << std::endl;
        return;
    }

    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "|" << std::setw(10) << "Index"
              << "|" << std::setw(10) << "First"
              << "|" << std::setw(10) << "Last"
              << "|" << std::setw(10) << "Nickname"
              << "|" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;

    for (int i = 0; i < this->size; i++)
        this->contacts[i].printSearch(i);

    int index = readIndex();
    if (index < 0 || index >= this->size)
    {
        std::cout << "Invalid index." << std::endl;
        return;
    }

    std::cout << "----- Contact details -----" << std::endl;
    this->contacts[index].printDetails();
}
