# Cmake will compile this file to create a exe file
# If you want to create a DLL file, you can change the file

cmake_minimum_required(VERSION 3.30)
project(mcts)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES "*.cpp")

add_executable(mcts ${SOURCES})
