#include "ClapTrap.hpp"
#include "ScavTrap.hpp"

int main()
{
    ClapTrap a("Bob");
    ScavTrap b("Jack");

    a.attack("Someone");
    b.attack("Someone");

    b.guardGate();

    return 0;
}
