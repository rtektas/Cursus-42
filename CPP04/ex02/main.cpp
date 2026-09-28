#include "Cat.hpp"
#include "Dog.hpp"
#include <iostream>

int	main(void)
{
	const AAnimal	*dog = new Dog();
	const AAnimal	*cat = new Cat();
	delete			dog;
	delete			cat;
		Dog a;

	std::cout << "===== Polymorphism with abstract =====" << std::endl;
	std::cout << dog->getType() << " says: ";
	dog->makeSound();
	std::cout << cat->getType() << " says: ";
	cat->makeSound();
	std::cout << "\n===== Deep copy test =====" << std::endl;
	{
		a.setIdea(0, "Bone!");
		Dog b(a);
		b.setIdea(0, "More bones!");
		std::cout << "a idea[0] = " << a.getIdea(0) << std::endl;
		std::cout << "b idea[0] = " << b.getIdea(0) << std::endl;
	}
	return (0);
}
