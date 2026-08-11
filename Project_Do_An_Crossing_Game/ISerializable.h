#pragma once
#include <fstream>

class ISerializable {
public:
    virtual void saveToFile(std::ofstream& out) = 0;

    virtual void loadFromFile(std::ifstream& in) = 0;

    virtual ~ISerializable() = default;
};