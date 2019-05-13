cmake_minimum_required (VERSION 3.0)
project (Games)

add_executable(Game main.cpp)

target_compile_features(Game PUBLIC cxx_std_17)
target_link_libraries(Game sfml-system sfml-window sfml-graphics)
