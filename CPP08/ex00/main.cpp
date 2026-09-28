#include <iostream>
#include <vector>
#include <list>
#include "easyfind.hpp"

int main()
{
    try
    {
        std::vector<int> v;

        v.push_back(10);
        v.push_back(20);
        v.push_back(30);

        std::cout << "=== Vector ===" << std::endl;

        std::vector<int>::iterator it = easyfind(v, 20);
        std::cout << "Found: " << *it << std::endl;

        std::cout << "Searching 42..." << std::endl;
        easyfind(v, 42); // exception

    }
    catch (std::exception& e)
    {
        std::cout << "Value not found" << std::endl;
    }

    try
    {
        std::list<int> lst;

        lst.push_back(1);
        lst.push_back(2);
        lst.push_back(3);

        std::cout << "\n=== List ===" << std::endl;

        std::list<int>::iterator it = easyfind(lst, 2);
        std::cout << "Found: " << *it << std::endl;
    }
    catch (std::exception& e)
    {
        std::cout << "Value not found" << std::endl;
    }

    return 0;
}