add_executable(test_grid test_grid.cpp)
target_link_libraries(test_grid PRIVATE langton_seq_lib)
add_test(NAME GridTest COMMAND test_grid)

add_executable(test_ant test_ant.cpp)
target_link_libraries(test_ant PRIVATE langton_seq_lib)
add_test(NAME AntTest COMMAND test_ant)

add_executable(test_correctness test_correctness.cpp)
target_link_libraries(test_correctness PRIVATE langton_seq_lib)
add_test(NAME CorrectnessTest COMMAND test_correctness)
