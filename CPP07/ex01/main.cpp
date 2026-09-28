#include <iostream>
#include <string>
#include "iter.hpp"

// Fonction pour int
void printInt(int &x)
{
    std::cout << x << std::endl;
}

// Fonction pour string
void printString(std::string &s)
{
    std::cout << s << std::endl;
}

// Fonction qui modifie
void increment(int &x)
{
    x++;
}

int main()
{
    int arr[] = {1, 2, 3, 4};
    std::string words[] = {"hello", "world", "42"};

    std::cout << "=== Print int ===" << std::endl;
    iter(arr, 4, printInt);

    std::cout << "\n=== Increment ===" << std::endl;
    iter(arr, 4, increment);
    iter(arr, 4, printInt);

    std::cout << "\n=== Print string ===" << std::endl;
    iter(words, 3, printString);

    return 0;
}