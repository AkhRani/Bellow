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

  StarSystem* GetStarSystem(int id) override {
    if (CheckId(id, m_Systems)) {
      return m_Systems.at(id).get();
    }
    return nullptr;
  }

  // For now, same as galaxy's implementation
  bool Move(Position& position, double speed, int target) override {
    StarSystem *pTarget = GetStarSystem(target);
    double dx = pTarget->m_Position.x - position.x;
    double dy = pTarget->m_Position.y - position.y;
    double distance = sqrt(dx * dx + dy * dy);
    if (distance <= speed) {
      position = pTarget->m_Position;
      return true;
    }
    else {
      double angle = atan2(dy, dx);
      position.x += speed * cos(angle);
      position.y += speed * sin(angle);
      return false;
    }
  }

private:
  std::vector<std::unique_ptr<StarSystem> > m_Systems;
};

#endif