#include <iostream>
#include <string>

int main()
{
    std::string     brain = "HI THIS IS BRAIN";
    std::string*    stringPTR = &brain;
    std::string&    stringREF = brain;

    std::cout << "Addresses:" << std::endl;
    std::cout << "  brain      : " << &brain << std::endl;
    std::cout << "  stringPTR  : " << stringPTR << std::endl;
    std::cout << "  stringREF  : " << &stringREF << std::endl;

    std::cout << std::endl;

    std::cout << "Values:" << std::endl;
    std::cout << "  brain      : " << brain << std::endl;
    std::cout << "  *stringPTR : " << *stringPTR << std::endl;
    std::cout << "  stringREF  : " << stringREF << std::endl;

    return 0;
}
