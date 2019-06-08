#include <CSVReader.h>
#include <CSVWriter.h>
#include <XMLEntity.h>
#include <XMLReader.h>
#include <MapRouter.h>
#include <StringUtils.h>
#include <unordered_map>
#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <ctime>
#include <limits>
#include <iomanip>

int main(int argc, char *argv[]) {
    std::string command;
    std::vector<std::string> split_command;
    bool done = false;
    bool shortOrfast = false;
    std::ofstream out;
    CCSVWriter writer(out);
    CMapRouter route;
    std::vector<CMapRouter::TNodeID> ShortestPath;
    std::vector<CMapRouter::TPathStep> FastestPath;
    std::vector<std::string> description;
    std::ifstream davis_osm("../data/davis.osm");
    std::ifstream stop_csv("../data/stops.csv");
    std::ifstream routes_csv("../data/routes.csv");
    route.LoadMapAndRoutes(davis_osm, stop_csv, routes_csv);

    while (!done) {
        std::cout << "> ";
        std::getline(std::cin, command);
        if (command.empty()) {
            continue;
        }
        split_command = StringUtils::Split(command);

        if (split_command[0] == "exit") {
            done = true;
        } else if (split_command[0] == "help") {
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

        } else if (split_command[0] == "count") {
            size_t count = route.NodeCount();
            std::cout << count << " nodes" << std::endl;
        } else if (split_command[0] == "node") {
            if (split_command.size() != 2){
                std::cout << "Invalid node parameter, see help." << std::endl;
            } else {
                try {
                    size_t param = std::stoi(split_command[1]);
                    if (param >= 0 and param < route.NodeCount()) {
                        CMapRouter::TNodeID tempID = route.GetSortedNodeIDByIndex(param);
                        CMapRouter::TLocation tempLoc = route.GetSortedNodeLocationByIndex(param);
                        std::string lat = std::to_string(tempLoc.first);
                        std::string lon = std::to_string(tempLoc.second);
                        std::cout << "Node " << param << ": id = " << tempID << " is at " << std::fixed
                                  << std::setprecision(3) << lat << ", " << std::fixed << std::setprecision(3) << lon
                                  << std::endl;
                    } else {
                        std::cout << "Invalid node parameter, see help." << std::endl;
                    }
                } catch (std::exception &Ex) {
                    std::cout << "Invalid node parameter, see help." << std::endl;
                }
            }
        } else if (split_command[0] == "fastest") {
            shortOrfast = true;
            if (split_command.size() != 3){
                std::cout << "Invalid fastest command, see help." << std::endl;
            } else {
                try {
                    size_t param1 = std::stoi(split_command[1]);
                    size_t param2 = std::stoi(split_command[2]);
                    double temp = route.FindFastestPath(param1, param2, FastestPath);
                    int hour = int(temp);
                    double min = (temp - hour) * 60.0;
                    double sec = (min - int(min)) * 60.0;
                    if (temp == std::numeric_limits<double>::max()) {
                        std::cout << "Unable to find fastest path from " << param1 << " to " << param2 << std::endl;
                    } else {
                        if (hour == 0) {
                            std::cout << "Fastest path takes " << int(min) << "min " << std::fixed
                                      << std::setprecision(1) << sec << "sec" << std::endl;
                        } else {
                            std::cout << "Fastest path takes " << hour << "hr " << int(min) << "min " << std::fixed
                                      << std::setprecision(1) << sec << "sec" << std::endl;
                        }
                        route.GetPathDescription(FastestPath, description);
                    }
                } catch (std::exception &Ex) {
                    std::cout << "Invalid fastest command, see help." << std::endl;
                }
            }
        } else if (split_command[0] == "shortest") {
            shortOrfast = false;
            if (split_command.size() != 3){
                std::cout << "Invalid shortest command, see help." << std::endl;
            } else {
                try {
                    size_t param1 = std::stoi(split_command[1]);
                    size_t param2 = std::stoi(split_command[2]);
                    double res = route.FindShortestPath(param1, param2, ShortestPath);
                    if (res == std::numeric_limits<double>::max()) {
                        std::cout << "Unable to find shortest path from " << param1 << " to " << param2 << std::endl;
                    } else {
                        std::cout << "Shortest path is " << std::fixed << std::setprecision(2) << res << "mi"
                                  << std::endl;
                    }
                    route.GetShortDescription(ShortestPath, description);
                } catch (std::exception &Ex) {
                    std::cout << "Invalid shortest command, see help." << std::endl;
                }
            }
        } else if (split_command[0] == "save") {
            if (description.empty()) {
                std::cout << "Failed to save path." << std::endl;
            } else {
                out.open("./saved_path.csv");
                out << writer.WriteRow(description) << "\n";
                out.close();
                std::cout << "File saved." << std::endl;
            }

        } else if (split_command[0] == "print") {
            if (description.empty()){
                std::cout << "No valid path to print." << std::endl;
            } else {
                if (shortOrfast) {
                    for (auto &elem:description) {
                        std::cout << elem << std::endl;
                    }
                } else {
                    for (auto &elem:description) {
                        std::cout << elem << std::endl;
                    }
                }
            }

        } else {
            std::cout << "Unknown command \"" << command << "\" type help for help." << std::endl;
        }
        command.clear();
        split_command.clear();
    }
    return EXIT_SUCCESS;
}
