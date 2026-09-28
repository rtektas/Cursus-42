#include "Zombie.hpp"

void randomChump(const std::string& name)
{
    Zombie tmp(name);
    tmp.announce();
}
