#ifndef MUTANTSTACK_HPP
#define MUTANTSTACK_HPP

#include <stack>

template <typename T>
class MutantStack : public std::stack<T>
{
public:
    // Types d'itérateurs
    typedef typename std::stack<T>::container_type::iterator iterator;

    // Constructeur
    MutantStack() {}

    // Constructeur copie
    MutantStack(const MutantStack& other) : std::stack<T>(other) {}

    // Opérateur =
    MutantStack& operator=(const MutantStack& other)
    {
        std::stack<T>::operator=(other);
        return *this;
    }

    // Destructeur
    ~MutantStack() {}

    // begin / end
    iterator begin()
    {
        return this->c.begin();
    }

    iterator end()
    {
        return this->c.end();
    }
};

#endif