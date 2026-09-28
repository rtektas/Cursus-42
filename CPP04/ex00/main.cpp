#include <iostream>
#include "Animal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"
#include "WrongAnimal.hpp"
#include "WrongCat.hpp"

int main()
{
    std::cout << "===== polymorphism =====" << std::endl;
    {
        const Animal* meta = new Animal();
        const Animal* j = new Dog();
        const Animal* i = new Cat();

        std::cout << "meta type: " << meta->getType() << std::endl;
        std::cout << "j type: " << j->getType() << std::endl;
        std::cout << "i type: " << i->getType() << std::endl;

        meta->makeSound(); // Animal
        j->makeSound();    // Dog (virtual)
        i->makeSound();    // Cat (virtual)

        delete meta;
        delete j;
        delete i;
    }

    std::cout << "\n===== no polymorphism (no virtual) =====" << std::endl;
    {
        const WrongAnimal* wa = new WrongAnimal();
        const WrongAnimal* wc = new WrongCat();

        std::cout << "wa type: " << wa->getType() << std::endl;
        std::cout << "wc type: " << wc->getType() << std::endl;

        wa->makeSound(); // WrongAnimal
        wc->makeSound(); // WrongAnimal (PAS virtual => mauvais comportement)

        delete wa;
        delete wc;
    }

    return 0;
}
