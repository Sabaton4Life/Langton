#include "grid.hpp"
#include <stdexcept>

Grid::Grid(int rows, int cols)
    : rows_(rows), cols_(cols), data_(rows * cols, false) {}

Grid::Grid(const Grid& other)
    : rows_(other.rows_), cols_(other.cols_), data_(other.data_) {}

Grid& Grid::operator=(const Grid& other) {
    if (this != &other) {
        rows_ = other.rows_;
        cols_ = other.cols_;
        data_ = other.data_;
    }
    return *this;
}

bool Grid::get(int r, int c) const {
    if (!isValid(r, c)) return false;
    return data_[index(r, c)];
}

void Grid::set(int r, int c, bool value) {
    if (isValid(r, c)) {
        data_[index(r, c)] = value;
    }
}

void Grid::flip(int r, int c) {
    if (isValid(r, c)) {
        data_[index(r, c)] = !data_[index(r, c)];
    }
}

int Grid::index(int r, int c) const {
    return r * cols_ + c;
}

bool Grid::isValid(int r, int c) const {
    return r >= 0 && r < rows_ && c >= 0 && c < cols_;
}
