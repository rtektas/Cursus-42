#include "Span.hpp"
#include <algorithm>

Span::Span(unsigned int n) : _maxSize(n)
{
}

Span::Span(const Span& other)
{
    *this = other;
}

Span& Span::operator=(const Span& other)
{
    if (this != &other)
    {
        _maxSize = other._maxSize;
        _numbers = other._numbers;
    }
    return *this;
}

Span::~Span()
{
}

void Span::addNumber(int n)
{
    if (_numbers.size() >= _maxSize)
        throw std::exception();

    _numbers.push_back(n);
}

int Span::shortestSpan()
{
    if (_numbers.size() < 2)
        throw std::exception();

    std::vector<int> tmp = _numbers;

    std::sort(tmp.begin(), tmp.end());

    int minSpan = tmp[1] - tmp[0];

    for (unsigned int i = 1; i < tmp.size() - 1; i++)
    {
        int diff = tmp[i + 1] - tmp[i];
        if (diff < minSpan)
            minSpan = diff;
    }

    return minSpan;
}

int Span::longestSpan()
{
    if (_numbers.size() < 2)
        throw std::exception();

    int min = *std::min_element(_numbers.begin(), _numbers.end());
    int max = *std::max_element(_numbers.begin(), _numbers.end());

    return max - min;
}