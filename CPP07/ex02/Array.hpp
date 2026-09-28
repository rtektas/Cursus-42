#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <exception>
#include <cstddef>

template <typename T>
class Array
{
private:
    T*              _data;
    unsigned int    _size;

public:
    // Constructeur par défaut : tableau vide
    Array() : _data(NULL), _size(0)
    {
    }

    // Constructeur avec taille
    Array(unsigned int n) : _data(new T[n]()), _size(n)
    {
    }

    // Constructeur de copie
    Array(const Array& other) : _data(NULL), _size(0)
    {
        *this = other;
    }

    // Opérateur d'affectation
    Array& operator=(const Array& other)
    {
        if (this != &other)
        {
            delete[] _data;

            _size = other._size;
            _data = new T[_size];

            for (unsigned int i = 0; i < _size; i++)
                _data[i] = other._data[i];
        }
        return *this;
    }

    // Destructeur
    ~Array()
    {
        delete[] _data;
    }

    // Accès aux éléments
    T& operator[](unsigned int index)
    {
        if (index >= _size)
            throw std::out_of_range("Index out of range");
        return _data[index];
    }

    // Version const
    const T& operator[](unsigned int index) const
    {
        if (index >= _size)
            throw std::out_of_range("Index out of range");
        return _data[index];
    }

    // Retourne la taille
    unsigned int size() const
    {
        return _size;
    }
};

#endif