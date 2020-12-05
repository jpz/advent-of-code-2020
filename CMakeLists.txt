cmake_minimum_required(VERSION 3.10)

project(AdventOfCode2020)

# copy data/* files to build directory
file(GLOB DATAFILES data/*)
file(COPY ${DATAFILES} DESTINATION .)


add_executable(day1a src/day1a.cpp)
add_executable(day1b src/day1b.cpp)
add_executable(day2a src/day2a.cpp)
add_executable(day2b src/day2b.cpp)


