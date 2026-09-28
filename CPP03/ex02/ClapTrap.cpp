#include "ClapTrap.hpp"


ClapTrap::ClapTrap()
{
    name = "Default";
    hitPoints = 10;
    energyPoints = 10;
    attackDamage = 0;

    std::cout << "ClapTrap " << name << " created (default)." << std::endl;
}


ClapTrap::ClapTrap(const std::string& name) : name(name)
{
    hitPoints = 10;
    energyPoints = 10;
    attackDamage = 0;

    std::cout << "ClapTrap " << name << " created." << std::endl;
}

// Constructeur de copie
ClapTrap::ClapTrap(const ClapTrap& other)
{
    *this = other;
    std::cout << "ClapTrap " << name << " copied." << std::endl;
}

// Opérateur d'affectation
ClapTrap& ClapTrap::operator=(const ClapTrap& other)
{
    if (this != &other)
    {
        name = other.name;
        hitPoints = other.hitPoints;
        energyPoints = other.energyPoints;
        attackDamage = other.attackDamage;
    }
    return *this;
}


ClapTrap::~ClapTrap()
{
    std::cout << "ClapTrap " << name << " destroyed." << std::endl;
}

// Attaque
void ClapTrap::attack(const std::string& target)
{
    if (hitPoints <= 0)
    {
        std::cout << "ClapTrap " << name << " is dead and cannot attack." << std::endl;
        return;
    }
    if (energyPoints <= 0)
    {
        std::cout << "ClapTrap " << name << " has no energy left to attack." << std::endl;
        return;
    }

    energyPoints--;
    std::cout << "ClapTrap " << name << " attacks " << target
              << ", causing " << attackDamage
              << " points of damage!" << std::endl;
}

// Prendre des dégâts
void ClapTrap::takeDamage(unsigned int amount)
{
    if (hitPoints <= 0)
    {
        std::cout << "ClapTrap " << name << " is already dead." << std::endl;
        return;
    }

    hitPoints -= amount;
    if (hitPoints < 0)
        hitPoints = 0;

    std::cout << "ClapTrap " << name << " takes "
              << amount << " points of damage!" << std::endl;
}

// Se réparer
void ClapTrap::beRepaired(unsigned int amount)
{
    if (hitPoints <= 0)
    {
        std::cout << "ClapTrap " << name << " is dead and cannot repair itself." << std::endl;
        return;
    }
    if (energyPoints <= 0)
    {
        std::cout << "ClapTrap " << name << " has no energy left to repair." << std::endl;
        return;
    }

    energyPoints--;
    hitPoints += amount;

    std::cout << "ClapTrap " << name << " repairs itself for "
              << amount << " hit points!" << std::endl;
}
