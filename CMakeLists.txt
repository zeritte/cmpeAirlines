cmake_minimum_required(VERSION 3.5)
project(project2)

set(CMAKE_CXX_STANDARD 14)

add_executable(project2 main.cpp InputParser.cpp InputParser.h Passenger.cpp Passenger.h AppManager.cpp AppManager.h Counter.cpp Counter.h)