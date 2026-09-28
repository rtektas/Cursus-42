#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>

class BitcoinExchange
{
private:
    std::map<std::string, float> _data;

public:
    BitcoinExchange();

    void loadDatabase(const std::string& filename);
    void processInput(const std::string& filename);

    bool isValidDate(const std::string& date);
};

#endif