#include "ant.hpp"

std::pair<int, int> Ant::getNextPosition() const {
    int dx = 0, dy = 0;
    switch (dir) {
        case Direction::NORTH: dy = -1; break;
        case Direction::EAST:  dx = 1;  break;
        case Direction::SOUTH: dy = 1;  break;
        case Direction::WEST:  dx = -1; break;
    }
    return {x + dx, y + dy};
}
