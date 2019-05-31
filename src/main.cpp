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
#include <unordered_map>
#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <ctime>

int main() {
    CMapRouter route;
    std::ifstream davis_osm("../data/davis.osm");
    std::ifstream stop_csv("../data/stops.csv");
    std::ifstream routes_csv("../data/routes.csv");
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
    std::cout << "States: " << route.FindShortestPath(95710382, 2782126874, path_ID) << std::endl;
    route.GetShortDescription(path_ID, path_str);
    duration = (find_short - load) / (double) CLOCKS_PER_SEC;

    for (auto element:path_str) {
        std::cout << element << std::endl;
    }
    std::cout << "Find Short Time: " << duration << '\n';


    return EXIT_SUCCESS;
}