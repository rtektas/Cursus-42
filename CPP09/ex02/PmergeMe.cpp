#include "PmergeMe.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <climits>
#include <cctype>

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other)
{
    *this = other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
    if (this != &other)
    {
        _vector = other._vector;
        _deque = other._deque;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

int PmergeMe::parseNumber(const std::string& str) const
{
    if (str.empty())
        throw std::runtime_error("Error");

    for (size_t i = 0; i < str.length(); i++)
    {
        if (!std::isdigit(static_cast<unsigned char>(str[i])))
            throw std::runtime_error("Error");
    }

    std::stringstream ss(str);
    long n;

    ss >> n;
    if (n < 0 || n > INT_MAX)
        throw std::runtime_error("Error");

    return static_cast<int>(n);
}

void PmergeMe::load(int argc, char** argv)
{
    for (int i = 1; i < argc; i++)
    {
        int n = parseNumber(argv[i]);
        _vector.push_back(n);
        _deque.push_back(n);
    }
}

void PmergeMe::binaryInsertVector(std::vector<int>& sorted, int value)
{
    size_t left = 0;
    size_t right = sorted.size();

    while (left < right)
    {
        size_t mid = (left + right) / 2;

        if (value < sorted[mid])
            right = mid;
        else
            left = mid + 1;
    }
    sorted.insert(sorted.begin() + left, value);
}

void PmergeMe::binaryInsertDeque(std::deque<int>& sorted, int value)
{
    size_t left = 0;
    size_t right = sorted.size();

    while (left < right)
    {
        size_t mid = (left + right) / 2;

        if (value < sorted[mid])
            right = mid;
        else
            left = mid + 1;
    }
    sorted.insert(sorted.begin() + left, value);
}

std::vector<int> PmergeMe::fordJohnsonVector(std::vector<int> input)
{
    if (input.size() <= 1)
        return input;

    std::vector<int> mainChain;
    std::vector<int> pending;
    bool hasOdd = false;
    int oddValue = 0;

    if (input.size() % 2 != 0)
    {
        hasOdd = true;
        oddValue = input[input.size() - 1];
        input.pop_back();
    }

    for (size_t i = 0; i < input.size(); i += 2)
    {
        int a = input[i];
        int b = input[i + 1];

        if (a > b)
        {
            mainChain.push_back(a);
            pending.push_back(b);
        }
        else
        {
            mainChain.push_back(b);
            pending.push_back(a);
        }
    }

    mainChain = fordJohnsonVector(mainChain);

    for (size_t i = 0; i < pending.size(); i++)
        binaryInsertVector(mainChain, pending[i]);

    if (hasOdd)
        binaryInsertVector(mainChain, oddValue);

    return mainChain;
}

std::deque<int> PmergeMe::fordJohnsonDeque(std::deque<int> input)
{
    if (input.size() <= 1)
        return input;

    std::deque<int> mainChain;
    std::deque<int> pending;
    bool hasOdd = false;
    int oddValue = 0;

    if (input.size() % 2 != 0)
    {
        hasOdd = true;
        oddValue = input[input.size() - 1];
        input.pop_back();
    }

    for (size_t i = 0; i < input.size(); i += 2)
    {
        int a = input[i];
        int b = input[i + 1];

        if (a > b)
        {
            mainChain.push_back(a);
            pending.push_back(b);
        }
        else
        {
            mainChain.push_back(b);
            pending.push_back(a);
        }
    }

    mainChain = fordJohnsonDeque(mainChain);

    for (size_t i = 0; i < pending.size(); i++)
        binaryInsertDeque(mainChain, pending[i]);

    if (hasOdd)
        binaryInsertDeque(mainChain, oddValue);

    return mainChain;
}

static void printVector(const std::vector<int>& v)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        std::cout << v[i];
        if (i + 1 < v.size())
            std::cout << " ";
    }
    std::cout << std::endl;
}

void PmergeMe::sortAndPrint()
{
    std::cout << "Before: ";
    printVector(_vector);

    clock_t startVector = clock();
    _vector = fordJohnsonVector(_vector);
    clock_t endVector = clock();

    clock_t startDeque = clock();
    _deque = fordJohnsonDeque(_deque);
    clock_t endDeque = clock();

    std::cout << "After:  ";
    printVector(_vector);

    double vectorTime = static_cast<double>(endVector - startVector)
                        / CLOCKS_PER_SEC * 1000000.0;

    double dequeTime = static_cast<double>(endDeque - startDeque)
                       / CLOCKS_PER_SEC * 1000000.0;

    std::cout << "Time to process a range of " << _vector.size()
              << " elements with std::vector : "
              << vectorTime << " us" << std::endl;

    std::cout << "Time to process a range of " << _deque.size()
              << " elements with std::deque  : "
              << dequeTime << " us" << std::endl;
}