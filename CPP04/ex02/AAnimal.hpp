#ifndef AANIMAL_HPP
#define AANIMAL_HPP

#include <iostream>
#include <string>

class AAnimal
{
protected:
    std::string type;

public:
    AAnimal();
    AAnimal(const AAnimal& other);
    AAnimal& operator=(const AAnimal& other);
    virtual ~AAnimal(); // virtuel obligatoire

    std::string getType() const;

    // PURE VIRTUELLE => classe abstraite => impossible de faire AAnimal a;
    virtual void makeSound() const = 0;
};

#endif
