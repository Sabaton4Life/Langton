#include "sequential/ant.hpp"
#include <cassert>
#include <iostream>

int main() {
    std::cout << "Testing Ant class...\n";

    Ant a(5, 5, Direction::NORTH, 0);
    assert(a.x == 5);
    assert(a.y == 5);
    assert(a.dir == Direction::NORTH);

    // Test rotation
    a.rotateRight();
    assert(a.dir == Direction::EAST);

    a.rotateRight();
    assert(a.dir == Direction::SOUTH);

    a.rotateLeft();
    assert(a.dir == Direction::EAST);

    // Test next position
    a.dir = Direction::NORTH;
    auto [nx, ny] = a.getNextPosition();
    assert(nx == 5 && ny == 4); // North = -y

    a.dir = Direction::EAST;
    auto [nx2, ny2] = a.getNextPosition();
    assert(nx2 == 6 && ny2 == 5); // East = +x

    std::cout << "All ant tests passed!\n";
    return 0;
}
