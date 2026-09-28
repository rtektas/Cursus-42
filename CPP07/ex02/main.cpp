#include <iostream>
#include <string>
#include "Array.hpp"

int main()
{
    try
    {
        std::cout << "=== Empty array ===" << std::endl;
        Array<int> empty;
        std::cout << "empty size = " << empty.size() << std::endl;

        std::cout << "\n=== Int array ===" << std::endl;
        Array<int> numbers(5);

        for (unsigned int i = 0; i < numbers.size(); i++)
            numbers[i] = i * 10;

        for (unsigned int i = 0; i < numbers.size(); i++)
            std::cout << "numbers[" << i << "] = " << numbers[i] << std::endl;

        std::cout << "\n=== Copy constructor ===" << std::endl;
        Array<int> copy(numbers);
        copy[0] = 999;

        std::cout << "original[0] = " << numbers[0] << std::endl;
        std::cout << "copy[0] = " << copy[0] << std::endl;

        std::cout << "\n=== Assignment operator ===" << std::endl;
        Array<int> assigned;
        assigned = numbers;
        assigned[1] = 777;

        std::cout << "original[1] = " << numbers[1] << std::endl;
        std::cout << "assigned[1] = " << assigned[1] << std::endl;

        std::cout << "\n=== String array ===" << std::endl;
        Array<std::string> words(3);
        words[0] = "hello";
        words[1] = "world";
        words[2] = "42";

        for (unsigned int i = 0; i < words.size(); i++)
            std::cout << "words[" << i << "] = " << words[i] << std::endl;

        std::cout << "\n=== Exception test ===" << std::endl;
        std::cout << numbers[42] << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    return 0;
}