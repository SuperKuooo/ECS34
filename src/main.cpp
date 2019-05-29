//
// Created by superkuo on 5/27/19.
//
#include <CSVReader.h>
#include <CSVWriter.h>
#include <XMLEntity.h>
#include <XMLReader.h>
#include <XMLWriter.h>
#include <MapRouter.h>
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
    std::clock_t start;
    double duration;

    start = std::clock();

    if (route.LoadMapAndRoutes(davis_osm, stop_csv, routes_csv)) {
        return EXIT_FAILURE;
    } else
        std::cout << "Finished Load" << std::endl;

    duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;

    std::cout << "Load Time: " << duration << '\n';


    return EXIT_SUCCESS;
}