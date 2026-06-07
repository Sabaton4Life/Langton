#pragma once

#include <vector>
#include <cstdint>

class Grid {
public:
    Grid(int rows, int cols);
    Grid(const Grid& other);
    Grid& operator=(const Grid& other);

    bool get(int r, int c) const;
    void set(int r, int c, bool value);
    void flip(int r, int c);

    int getRows() const { return rows_; }
    int getCols() const { return cols_; }

    const std::vector<bool>& data() const { return data_; }
    std::vector<bool>& data() { return data_; }

private:
    int rows_, cols_;
    std::vector<bool> data_;

    int index(int r, int c) const;
    bool isValid(int r, int c) const;
};
