#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

BitcoinExchange::BitcoinExchange()
{
}

void BitcoinExchange::loadDatabase(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    std::string line;

    getline(file, line); // skip header

    while (getline(file, line))
    {
        std::stringstream ss(line);
        std::string date, value;

        getline(ss, date, ',');
        getline(ss, value);

        _data[date] = std::atof(value.c_str());
    }
}

bool BitcoinExchange::isValidDate(const std::string& date)
{
    return (date.length() == 10 &&
            date[4] == '-' &&
            date[7] == '-');
}

void BitcoinExchange::processInput(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    std::string line;

    getline(file, line); // skip header

    while (getline(file, line))
    {
        std::stringstream ss(line);
        std::string date, valueStr;

        getline(ss, date, '|');
        getline(ss, valueStr);

        // nettoyer espaces
        date.erase(date.find_last_not_of(" ") + 1);
        valueStr.erase(0, valueStr.find_first_not_of(" "));

        if (!isValidDate(date))
        {
            std::cout << "Error: bad input => " << date << std::endl;
            continue;
        }

        float value = std::atof(valueStr.c_str());

        if (value < 0)
        {
            std::cout << "Error: not a positive number." << std::endl;
            continue;
        }

        if (value > 1000)
        {
            std::cout << "Error: too large a number." << std::endl;
            continue;
        }

        std::map<std::string, float>::iterator it = _data.lower_bound(date);

        if (it == _data.end() || it->first != date)
        {
            if (it != _data.begin())
                --it;
        }

        std::cout << date << " => " << value
                  << " = " << it->second * value << std::endl;
    }
}