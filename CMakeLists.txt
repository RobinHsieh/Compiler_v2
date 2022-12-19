cmake_minimum_required(VERSION 3.21)
project(compiler_v2)

set(CMAKE_CXX_STANDARD 98)

add_executable(compiler_v2 main.cpp ScannerTest.cpp ParserTest.cpp ScannerTest.h ParserTest.h)

