#include <stdint.h>

class Player {
  public:
    Player();

    //! New population per unit of population
    double getPopGrowthRate();
    //! Capital generated by each unit of population
    double getProductionPerPop();

    //! Number of factories operated by one unit of population
    uint32_t getFactoriesPerPop();
    //! Cost of a factory
    double getFactoryCost();
    //! Capital generated by one factory
    double getProductionPerFactory();
};

