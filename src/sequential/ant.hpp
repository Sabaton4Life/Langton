#pragma once

#include <cstdint>
#include <array>

enum class Direction : uint8_t {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

struct Ant {
    int x, y;
    Direction dir;
    int id;

    Ant(int x = 0, int y = 0, Direction dir = Direction::NORTH, int id = 0)
        : x(x), y(y), dir(dir), id(id) {}

    void rotateRight() {
        dir = static_cast<Direction>((static_cast<int>(dir) + 1) % 4);
    }

    void rotateLeft() {
        dir = static_cast<Direction>((static_cast<int>(dir) + 3) % 4);
    }

    std::pair<int, int> getNextPosition() const;
};
