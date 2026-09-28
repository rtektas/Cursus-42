#include "ClapTrap.hpp"

int main()
{
    ClapTrap a("Bob");

    a.attack("Alice");
    a.takeDamage(5);
    a.beRepaired(3);

    a.takeDamage(20);   // Bob meurt ici
    a.attack("Alice"); // Ne peut plus attaquer
    a.beRepaired(5);   // Ne peut plus se réparer

    return 0;
}
