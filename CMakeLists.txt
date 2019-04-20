cmake_minimum_required(VERSION 3.13)
project(Homework2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Homework2 main.cpp FileSystemTree.cpp DirectoryListing.cpp
        DirectoryListing.h FileSystemTree.h StringUtils.cpp StringUtils.h)