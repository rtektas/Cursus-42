#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include <string>

class Zombie
{
private:
    std::string _name;

public:
    Zombie();                               // constructeur par défaut
    Zombie(const std::string& name);        // constructeur avec nom
    ~Zombie();

    void setName(const std::string& name);  // utilisé dans zombieHorde
    void announce() const;                  // message zombie
};

Zombie* zombieHorde(int N, const std::string& name);

#endif
