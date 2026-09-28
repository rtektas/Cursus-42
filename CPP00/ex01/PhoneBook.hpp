#ifndef PHONEBOOK_HPP
#define PHONEBOOK_HPP

#include "Contact.hpp"

class PhoneBook
{
private:
    Contact contacts[8];
    int     size;       // nombre de contacts actuellement enregistres (0 à 8)
    int     nextIndex;  // position du prochain contact à ecraser (0 à 7)

public:
    PhoneBook();

    void addContact();        // gere l'ajout d'un contact (commande ADD)
    void searchContact() const; // affiche la liste + detail (commande SEARCH)
};

#endif
