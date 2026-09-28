#include "ClapTrap.hpp"
#include "ScavTrap.hpp"
#include "FragTrap.hpp"

int main()
{

    ClapTrap a("Clap");
    ScavTrap b("Scav");
    FragTrap c("Frag");

    a.attack("target");
    b.attack("target");
    c.attack("target");

    b.guardGate();
    c.highFivesGuys();

    return 0;
}
