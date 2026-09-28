#include "Base.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <exception>

Base::~Base()
{
}

Base* generate(void)
{
    int random = std::rand() % 3;

    if (random == 0)
    {
        std::cout << "Generated: A" << std::endl;
        return new A;
    }
    else if (random == 1)
    {
        std::cout << "Generated: B" << std::endl;
        return new B;
    }
    else
    {
        std::cout << "Generated: C" << std::endl;
        return new C;
    }
}

void identify(Base* p)
{
    if (dynamic_cast<A*>(p))
        std::cout << "Identified from pointer: A" << std::endl;
    else if (dynamic_cast<B*>(p))
        std::cout << "Identified from pointer: B" << std::endl;
    else if (dynamic_cast<C*>(p))
        std::cout << "Identified from pointer: C" << std::endl;
}

void identify(Base& p)
{
    try
    {
        (void)dynamic_cast<A&>(p);
        std::cout << "Identified from reference: A" << std::endl;
        return;
    }
    catch (std::exception& e)
    {
    }

    try
    {
        (void)dynamic_cast<B&>(p);
        std::cout << "Identified from reference: B" << std::endl;
        return;
    }
    catch (std::exception& e)
    {
    }

    try
    {
        (void)dynamic_cast<C&>(p);
        std::cout << "Identified from reference: C" << std::endl;
        return;
    }
    catch (std::exception& e)
    {
    }
}