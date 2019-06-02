//
// Created by superkuo on 5/27/19.
//


/*Questions
 * 1. Where to put SImplementation
 * 2. Incomplete deceleration
 * 3.
 *
 */
#include <CSVReader.h>
#include <XMLEntity.h>
#include <XMLReader.h>
#include <MapRouter.h>
#include "StringUtils.h"
#include <unordered_map>
#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <ctime>

/*
int main(int argc, char *argv[]){
    std::string command;
    std::vector<std::string> split_command;
    bool done = false;
    CMapRouter route;
    std::ifstream davis_osm("../data/davis.osm");
    std::ifstream stop_csv("../data/stops.csv");
    std::ifstream routes_csv("../data/routes.csv");
    route.LoadMapAndRoutes(davis_osm, stop_csv, routes_csv);

    while (!done){
        std::cout<< "> ";
        std::getline(std::cin, command);
        split_command = StringUtils::Split(command);

        if (split_command[0] == "exit"){
            done = true;
        } else if (split_command[0] == "help"){
            std::cout << "findroute [--data=path | --resutls=path]\n"
                         "------------------------------------------------------------------------\n"
                         "help     Display this help menu\n"
                         "exit     Exit the program\n"
                         "count    Output the number of nodes in the map\n"
                         "node     Syntax \"node [0, count)\" \n"
                         "         Will output node ID and Lat/Lon for node\n"
                         "fastest  Syntax \"fastest start end\" \n"
                         "         Calculates the time for fastest path from start to end\n"
                         "shortest Syntax \"shortest start end\" \n"
                         "         Calculates the distance for the shortest path from start to end\n"
                         "save     Saves the last calculated path to file\n"
                         "print    Prints the steps for the last calculated path"

        } else if (split_command[0] == "count"){
            size_t count = route.NodeCount();
            std::cout << count << " nodes" << std::endl;
        }

        else {
            std::cout << "Unknown command " << command << " type help for help." << std::endl;
        }
    }
    return EXIT_SUCCESS;
}*/


int main() {
    CMapRouter route;
    std::ifstream davis_osm("../data/davis_xml.xml");
    std::ifstream stop_csv("../data/shortstop.csv");
    std::ifstream routes_csv("../data/short.csv");
    std::vector<CMapRouter::TNodeID> path_ID;
    std::vector<std::string> path_str;
    std::clock_t start;
    double duration;

    start = std::clock();

    if (!route.LoadMapAndRoutes(davis_osm, stop_csv, routes_csv)) {
        return EXIT_FAILURE;
    } else
        std::cout << "Finished Load" << std::endl;

    auto load = std::clock();
    duration = (load - start) / (double) CLOCKS_PER_SEC;
    std::cout << "Load Time: " << duration << '\n';

    auto find_short = std::clock();
    std::cout << "States: " << route.FindShortestPath(1, 6, path_ID) << std::endl;
    route.GetShortDescription(path_ID, path_str);
    duration = (find_short - load) / (double) CLOCKS_PER_SEC;

    for (auto element:path_str) {
        std::cout << element << std::endl;
    }
    std::cout << "Find Short Time: " << duration << '\n';

    return EXIT_SUCCESS;
}