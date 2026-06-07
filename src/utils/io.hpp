#pragma once

#include <string>
#include <cstdint>
#include <vector>

class PPMExporter {
public:
    static void exportBinary(const std::vector<bool>& grid, int rows, int cols,
                             const std::string& filename);
};
