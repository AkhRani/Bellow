#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include <string>

/** Per-player information.
* This class will encapsulate or aggregate racials, tech advances,
* which systems have been explored, etc.
*/
class Player {
public:
  //! Basic Constructor
  Player(const std::string &name);

  //! Player name
  const std::string &GetName() const { return m_Name; } ;

  //! Ecological investment to "build" one population
  uint32_t GetPopCost();
  //! New population per unit of population
  double GetPopGrowthRate();
  //! Capital generated by each unit of population
  double GetProductionPerPop();

  //! Number of factories operated by one unit of population
  uint32_t GetFactoriesPerPop();
  //! Cost of a factory
  uint32_t GetFactoryCost();
  //! Capital generated by one factory
  double GetProductionPerFactory();

private:
  Player();
  std::string m_Name;
};

#endif