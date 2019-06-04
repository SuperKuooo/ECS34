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
#include <limits>

/*
int main(int argc, char *argv[]){
    std::string command;
    std::vector<std::string> split_command;
    bool done = false;
    bool NoException = true;
    CMapRouter route;
    std::vector<CMapRouter::TNodeID> ShortestPath;
    std::vector<CMapRouter::TPathStep> FastestPath;
    std::ifstream davis_osm("../data/davis.osm");
    std::ifstream stop_csv("../data/stops.csv");
    std::ifstream routes_csv("../data/routes.csv");
    route.LoadMapAndRoutes(davis_osm, stop_csv, routes_csv);

    struct InputError : std::exception{
        const char* what() const noexcept {
            return "Invalid command, see help.";
        }
    };

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
                         "print    Prints the steps for the last calculated path" << std::endl;

        } else if (split_command[0] == "count"){
            size_t count = route.NodeCount();
            std::cout << count << " nodes" << std::endl;
        } else if (split_command[0] == "node") {
            try {
                size_t param = std::stoi(split_command[1]);
                if (param >= 0 and param < route.NodeCount()){
                    CMapRouter::TNodeID tempID = route.GetSortedNodeIDByIndex(param);
                    CMapRouter::TLocation tempLoc = route.GetSortedNodeLocationByIndex(param);
                    std::string lat = std::to_string(tempLoc.first);
                    std::string lon = std::to_string(tempLoc.second);
                    std::cout << "Node " << param << ": id = " << tempID << " is at " << lat << ", " << lon << std::endl;
                } else {
                    std::cout << "Invalid node parameter, see help." << std::endl;
                }
            } catch (std::exception &Ex) {
                NoException = false;
                std::cout << "Invalid node parameter, see help." << std::endl;
            }
        } else if (split_command[0] == "fastest"){
            try {
                size_t param1 = std::stoi(split_command[1]);
                size_t param2 = std::stoi(split_command[2]);
                double res = route.FindFastestPath(param1, param2, FastestPath);
                if (res == std::numeric_limits<double>::max()){
                    std::cout << "Unable to find fastest path from " << param1 << " to " << param2 << std::endl;
                } else {
                    std::cout << "Fastest path takes " << res << std::endl;
                }
            } catch (std::exception &Ex) {
                NoException = false;
                std::cout << "Invalid fastest command, see help." << std::endl;
            }
        } else if (split_command[0] == "shortest"){
            try {
                size_t param1 = std::stoi(split_command[1]);
                size_t param2 = std::stoi(split_command[2]);
                double res = route.FindShortestPath(param1, param2, ShortestPath);
                if (res == std::numeric_limits<double>::max()){
                    std::cout << "Unable to find shortest path from " << param1 << " to " << param2 << std::endl;
                } else {
                    std::cout << "Shortest path is " << res << std::endl;
                }
            } catch (std::exception &Ex) {
                NoException = false;
                std::cout << "Invalid shortest command, see help." << std::endl;
            }
        } else if (split_command[0] == "save"){
            std::ofstream out("Saved_Path");

        } else if (split_command[0] == "print"){

        } else {
            std::cout << "Unknown command " << command << " type help for help." << std::endl;
        }
    }
    return EXIT_SUCCESS;
} */


int main() {
    CMapRouter route;
    std::ifstream davis_osm("../data/davis.osm");
    std::ifstream stop_csv("../data/stops.csv");
    std::ifstream routes_csv("../data/routes.csv");
    std::vector<CMapRouter::TPathStep> path_ID;
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

    //auto find_short = std::clock();
    double temp = route.FindFastestPath(265024841, 4399280681, path_ID);
    int hour = int(temp);
    double min = (temp - hour) * 60.0;
    double sec = (min - int(min)) * 60;

    std::cout << hour << "hr " << int(min) << "min " << sec << "sec" << std::endl;

    //route.GetShortDescription(path_ID, path_str);
    //duration = (find_short - load) / (double) CLOCKS_PER_SEC;

    for (auto element:path_ID) {
        std::cout << element.first << "  " << element.second << std::endl;
    }
    std::cout << "Find Short Time: " << duration << '\n';

    return EXIT_SUCCESS;
}