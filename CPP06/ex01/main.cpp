#include <iostream>
#include "Serializer.hpp"

int main()
{
    Data data;
    data.name = "Resul";
    data.value = 42;

    Data* originalPtr = &data;

    uintptr_t raw = Serializer::serialize(originalPtr);
    Data* restoredPtr = Serializer::deserialize(raw);

    std::cout << "Original pointer:    " << originalPtr << std::endl;
    std::cout << "Serialized value:    " << raw << std::endl;
    std::cout << "Deserialized pointer:" << restoredPtr << std::endl;

    std::cout << std::endl;
    std::cout << "Original data name:  " << originalPtr->name << std::endl;
    std::cout << "Original data value: " << originalPtr->value << std::endl;

    std::cout << std::endl;
    std::cout << "Restored data name:  " << restoredPtr->name << std::endl;
    std::cout << "Restored data value: " << restoredPtr->value << std::endl;

    return 0;
}