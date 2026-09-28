#include <iostream>
#include "Zombie.hpp"

Zombie::Zombie(const std::string& name) : _name(name)
{
    std::cout << "Zombie " << _name << " constructed." << std::endl;
}

Zombie::~Zombie()
{
    std::cout << "Zombie " << _name << " destroyed." << std::endl;
}

void Zombie::announce() const
{
    std::cout << _name << ": BraiiiiiiinnnzzzZ..." << std::endl;
}
