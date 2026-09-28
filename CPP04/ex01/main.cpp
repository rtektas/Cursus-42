#include <iostream>
#include "Dog.hpp"
#include "Cat.hpp"

int main()
{
    std::cout << "===== Deep copy test (Dog) =====" << std::endl;
    {
        Dog a;
        a.setIdea(0, "I want a bone.");

        Dog b(a); // copy constructor
        b.setIdea(0, "I want two bones.");

        std::cout << "a idea[0] = " << a.getIdea(0) << std::endl;
        std::cout << "b idea[0] = " << b.getIdea(0) << std::endl;
		std::cout << "a idea[0] = " << a.getIdea(0) << std::endl;
    }

    std::cout << "\n===== Deep copy test (Cat) =====" << std::endl;
    {
        Cat a;
        a.setIdea(1, "I want to sleep.");

        Cat b;
        b = a; // operator=

        b.setIdea(1, "I want to sleep MORE.");

        std::cout << "a idea[1] = " << a.getIdea(1) << std::endl;
        std::cout << "b idea[1] = " << b.getIdea(1) << std::endl;
    }

    return 0;
}
