#ifndef POSITION_H
#define POSITION_H

/**
 * Stores the position of a planet or fleet in the galaxy.
 *
 * For now, this is a basic 2D cartesian coordinate.  There is
 * a small chance this may change in the future, so classes other
 * than the Galaxy class should be loosely coupled to this
 * implementation.
 *
 * I might also use pImpl here, if it turns out to help unit tests.
 */
struct Position {
  public:
    //! Basic Constructor
    Position(double x, double y) : x(x), y(y) {};

    //! Update Position
    void Move(double dx, double dy) { x += dx; y += dy; };

    double x;   //!< Coordinate
    double y;   //!< Coordinate

  private:
    Position();
};

#endif