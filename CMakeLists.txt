set(CMAKE_CXX_STANDARD 14)
cmake_minimum_required(VERSION 3.13)
project(ECS34)

set(CMAKE_CXX_STANDARD 14)
include_directories(${EXPAT_INCLUDE_DIRS} ./lib ./include)

include(FindEXPAT)
find_package(EXPAT REQUIRED)

set(
        SOURCE_FILES
        src/main.cpp
        #src/speedtest.cpp
        src/MapRouter.cpp
        src/CSVReader.cpp
        src/CSVWriter.cpp
        src/XMLReader.cpp
        src/XMLWriter.cpp
        src/StringUtils.cpp

        lib/libcsv.c
)

add_executable(ECS34 ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${EXPAT_LIBRARIES})

#Seek help from this post. Literally just copied the whole thing.
#https://stackoverflow.com/questions/10860366/how-do-i-link-expat-into-a-project