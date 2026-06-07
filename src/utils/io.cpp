#include "io.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void PPMExporter::exportBinary(const std::vector<bool>& grid, int rows, int cols,
                              const std::string& filename) {
    try {
        fs::create_directories(fs::path(filename).parent_path());
    } catch (...) {}

    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    // PPM header (P6 = binary RGB)
    file << "P6\n";
    file << cols << " " << rows << "\n";
    file << "255\n";

    // Write pixel data: 0=white (255), 1=black (0)
    for (size_t i = 0; i < grid.size(); ++i) {
        uint8_t pixel = grid[i] ? 0 : 255;
        file.put(pixel);
        file.put(pixel);
        file.put(pixel);
    }

    file.close();
}
