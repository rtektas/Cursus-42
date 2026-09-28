#ifndef WHATEVER_HPP
#define WHATEVER_HPP

// SWAP
template <typename T>
void swap(T &a, T &b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

// MIN
template <typename T>
T min(T a, T b)
{
    if (a < b)
        return a;
    return b;
}

// MAX
template <typename T>
T max(T a, T b)
{
    if (a > b)
        return a;
    return b;
}

#endif