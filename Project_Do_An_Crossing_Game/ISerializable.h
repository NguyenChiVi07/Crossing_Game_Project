#pragma once
#include "IGameState.h"
#include <fstream>

class ISerializable //: public IGameState 
{
public:
  virtual void saveToFile(std::ofstream &out) = 0;

  virtual void loadFromFile(std::ifstream &in) = 0;

  virtual ~ISerializable() = default;
};