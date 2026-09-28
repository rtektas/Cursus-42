#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>

class PmergeMe
{
private:
    std::vector<int> _vector;
    std::deque<int>  _deque;

    int parseNumber(const std::string& str) const;

    std::vector<int> fordJohnsonVector(std::vector<int> input);
    std::deque<int>  fordJohnsonDeque(std::deque<int> input);

    void binaryInsertVector(std::vector<int>& sorted, int value);
    void binaryInsertDeque(std::deque<int>& sorted, int value);

public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    void load(int argc, char** argv);
    void sortAndPrint();
};

#endif