#include <iostream>
#include "Zombie.hpp"

int main()
{
    int     count = 5;
    Zombie* horde = zombieHorde(count, "Resul");

    if (!horde)
    {
        std::cout << "Failed to create horde." << std::endl;
        return 1;
    }

    for (int i = 0; i < count; i++)
    {
        std::cout << "Zombie #" << i << " -> ";
        horde[i].announce();
    }

    delete[] horde;
    return 0;
}
