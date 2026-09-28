#include "Base.hpp"
#include <cstdlib>
#include <ctime>

int main()
{
    std::srand(std::time(0));

    Base* ptr = generate();

    identify(ptr);
    identify(*ptr);

    delete ptr;
    return 0;
}