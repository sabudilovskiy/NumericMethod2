cmake_minimum_required(VERSION 3.20)
project(NumericMethod2)
set(CMAKE_CXX_STANDARD 20)

include(conan_libraries/conan_paths.cmake)
find_package(GTest)

add_library(${PROJECT_NAME}_objs
        src/math/matrix.hpp
        src/math/solver.hpp
        src/utils/generator.hpp
        src/math/empty.cpp)



#Unit Tests
include(GoogleTest)
add_executable( ${PROJECT_NAME}_tests
        src/utils/private_cracker.hpp
        tests/gauss_solver.cpp
        tests/rotation_solver.cpp
        tests/make_matrix.cpp
        )
target_link_libraries(${PROJECT_NAME}_tests PRIVATE
        GTest::gtest
        GTest::gtest_main
        ${PROJECT_NAME}_objs)
#Main
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)

include_directories(src)