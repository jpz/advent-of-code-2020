cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(AdventOfCode2020)

# copy data/* files to build directory
file(GLOB DATAFILES data/*)
file(COPY ${DATAFILES} DESTINATION .)


add_executable(day1a src/day1a.cpp)
add_executable(day1b src/day1b.cpp)
add_executable(day2a src/day2a.cpp)
add_executable(day2b src/day2b.cpp)
add_executable(day3a src/day3a.cpp)
add_executable(day3b src/day3b.cpp)
add_executable(day4a src/day4a.cpp)
add_executable(day4b src/day4b.cpp)
add_executable(day5a src/day5a.cpp)


