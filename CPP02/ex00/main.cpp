#include "Zombie.hpp"

int main()
{
    Zombie* z = newZombie("Resul");
    z->announce();

    randomChump("Bilal");

    delete z;
    return 0;
}
