add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark PRIVATE langton_seq_lib langton_mpi_lib MPI::MPI_CXX)
