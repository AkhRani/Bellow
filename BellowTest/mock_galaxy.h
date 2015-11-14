#ifndef MOCK_GALAXY_H
#define MOCK_GALAXY_H

#include <vector>
#include <memory>
#include "star_system.h"
#include "util.h"

class MockGalaxy : public IGalaxy {
public:
  void AddStarSystem(IGame& game, double x, double y) {
    int id = m_Systems.size();
    m_Systems.push_back(std::unique_ptr<StarSystem>(new StarSystem(game, id, x, y)));
  }

  StarSystem* GetStarSystem(int id) {
    if (CheckId(id, m_Systems)) {
      return m_Systems.at(id).get();
    }
    return nullptr;
  }

private:
  std::vector<std::unique_ptr<StarSystem> > m_Systems;
};

#endif