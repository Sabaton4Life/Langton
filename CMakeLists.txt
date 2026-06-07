cmake_minimum_required(VERSION 3.18)
project(Langton CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")

# MPI
find_package(MPI REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${MPI_CXX_INCLUDE_DIRS})

# Sequential library
add_library(langton_seq_lib
    src/sequential/grid.cpp
    src/sequential/ant.cpp
    src/sequential/simulator.cpp
    src/utils/io.cpp
)
target_include_directories(langton_seq_lib PUBLIC ${CMAKE_SOURCE_DIR}/src)

# MPI library
add_library(langton_mpi_lib
    src/mpi/domain.cpp
    src/mpi/communicator.cpp
    src/mpi/mpi_simulator.cpp
)
target_include_directories(langton_mpi_lib PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(langton_mpi_lib PUBLIC langton_seq_lib PRIVATE MPI::MPI_CXX)

# Sequential executable
add_executable(langton_seq src/main.cpp)
target_link_libraries(langton_seq PRIVATE langton_seq_lib)

# MPI executable
add_executable(langton_mpi src/main.cpp)
target_link_libraries(langton_mpi PRIVATE langton_mpi_lib MPI::MPI_CXX)
target_compile_definitions(langton_mpi PRIVATE ENABLE_MPI)

# Tests
enable_testing()
add_subdirectory(tests)

# Benchmarking
add_subdirectory(experiments)
