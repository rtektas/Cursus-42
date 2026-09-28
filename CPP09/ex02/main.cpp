#include "PmergeMe.hpp"
#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Error" << std::endl;
        return 1;
    }

    try
    {
        PmergeMe sorter;

        sorter.load(argc, argv);
        sorter.sortAndPrint();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}