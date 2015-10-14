#ifndef MOCK_GALAXY_H
#define MOCK_GALAXY_H

#include <vector>
#include <memory>
#include "star_system.h"

class MockGalaxy : public IStarSystemOwner {
public:
  void AddStarSystem(IGame& game, double x, double y) {
    m_Systems.push_back(std::unique_ptr<StarSystem>(new StarSystem(game, x, y));
  }

  StarSystem* GetStarSystem(int id) {
    return m_Systems.at(id - 1).get();
  }

private:
  std::vector<std::unique_ptr<StarSystem> > m_Systems;
};

#endif