#include <stdexcept>

// TODO:  subclass runtime error to store failure context
static inline void loadcheck(bool cond) {
  if (!cond) throw(std::runtime_error("load error"));
}