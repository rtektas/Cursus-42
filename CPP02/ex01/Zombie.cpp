#include <iostream>
#include "Zombie.hpp"

Zombie::Zombie()
{
}

Zombie::Zombie(const std::string& name) : _name(name)
{
}

Zombie::~Zombie()
{
    std::cout << "Zombie " << _name << " destroyed." << std::endl;
}

void Zombie::setName(const std::string& name)
{
    _name = name;
}

void Zombie::announce() const
{
    std::cout << _name << ": BraiiiiiiinnnzzzZ..." << std::endl;
}
