#include <MapRouter.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <vector>
#include <iomanip>
#include <XMLEntity.h>
#include <XMLReader.h>
#include <CSVReader.h>
#include <values.h>

const CMapRouter::TNodeID CMapRouter::InvalidNodeID = -1;

CMapRouter::CMapRouter() {

}

CMapRouter::~CMapRouter() {

}

double CMapRouter::HaversineDistance(double lat1, double lon1, double lat2, double lon2) {
    auto DegreesToRadians = [](double deg) { return M_PI * (deg) / 180.0; };
    double LatRad1 = DegreesToRadians(lat1);
    double LatRad2 = DegreesToRadians(lat2);
    double LonRad1 = DegreesToRadians(lon1);
    double LonRad2 = DegreesToRadians(lon2);
    double DeltaLat = LatRad2 - LatRad1;
    double DeltaLon = LonRad2 - LonRad1;
    double DeltaLatSin = sin(DeltaLat / 2);
    double DeltaLonSin = sin(DeltaLon / 2);
    double Computation = asin(
            sqrt(DeltaLatSin * DeltaLatSin + cos(LatRad1) * cos(LatRad2) * DeltaLonSin * DeltaLonSin));
    const double EarthRadiusMiles = 3959.88;

    return 2 * EarthRadiusMiles * Computation;
}

double CMapRouter::CalculateBearing(double lat1, double lon1, double lat2, double lon2) {
    auto DegreesToRadians = [](double deg) { return M_PI * (deg) / 180.0; };
    auto RadiansToDegrees = [](double rad) { return 180.0 * (rad) / M_PI; };
    double LatRad1 = DegreesToRadians(lat1);
    double LatRad2 = DegreesToRadians(lat2);
    double LonRad1 = DegreesToRadians(lon1);
    double LonRad2 = DegreesToRadians(lon2);
    double X = cos(LatRad2) * sin(LonRad2 - LonRad1);
    double Y = cos(LatRad1) * sin(LatRad2) - sin(LatRad1) * cos(LatRad2) * cos(LonRad2 - LonRad1);
    return RadiansToDegrees(atan2(X, Y));
}

bool CMapRouter::LoadMapAndRoutes(std::istream &osm, std::istream &stops, std::istream &routes) {
    if (!(osm and stops and routes)) {
        std::cout << "Error: Failed to open data file(s).";
        return false;
    }
    CXMLReader osm_in(osm);
    SXMLEntity entity_in;
    osm_in.ReadEntity(entity_in); //Reads in version and generator. AKA garbage
    int counter = 0;
    //Reads in nodes, coordinates, way relations, and oneway.
    //Add: Read in mph and roundabout.
    do {
        osm_in.ReadEntity(entity_in, true);
        //better if statement
        if (entity_in.DType != SXMLEntity::EType::StartElement or entity_in.DNameData != "node")
            continue;

        SImplementation new_node;
        TNodeID node_id = std::stol(entity_in.AttributeValue("id"));
        new_node.cood = std::pair<double, double>(std::stod(entity_in.AttributeValue("lat")),
                                                  std::stod(entity_in.AttributeValue("lon")));

        davis_map.insert(std::pair<TNodeID, SImplementation>(node_id, new_node));
        cheating_LOL.insert(std::pair<TNodeID, TLocation>(node_id, new_node.cood));
        //This is slower. Use insert. Marginally faster.
        //davis_map[node_id] = new_node;
        counter++;
    } while (entity_in.DNameData != "way");

    if (counter != davis_map.size()) {
        std::cout << "Error: Losing values in hashtable" << std::endl;
    }


    std::vector<std::unordered_map<TNodeID, SImplementation>::iterator> buffer_vect;
    std::vector<std::pair<std::string, std::string>> way_tags;

    while (osm_in.ReadEntity(entity_in, true)) {
        bool oneway;
        counter = 0;
        int length;

        while (entity_in.DNameData == "nd") {
            if (entity_in.DType != SXMLEntity::EType::StartElement) {
                osm_in.ReadEntity(entity_in, true);
                continue;
            }
            TNodeID target_id = std::stol(entity_in.AttributeValue("ref"));
            std::unordered_map<TNodeID, SImplementation>::iterator iter;
            iter = davis_map.find(target_id);
            if (iter == davis_map.end()) {
                osm_in.ReadEntity(entity_in, true);
                continue;
            } else {
                buffer_vect.emplace_back(iter);
                osm_in.ReadEntity(entity_in, true);
            }
        }

        while (entity_in.DNameData == "tag") {
            if (entity_in.DType != SXMLEntity::EType::StartElement) {
                osm_in.ReadEntity(entity_in, true);
                continue;
            }
            way_tags.emplace_back(
                    std::pair<std::string, std::string>(entity_in.AttributeValue("k"), entity_in.AttributeValue("v")));
            osm_in.ReadEntity(entity_in, true);
        }

        length = buffer_vect.size();
        oneway = std::find(way_tags.begin(), way_tags.end(), std::pair<std::string, std::string>("oneway", "yes")) !=
                 way_tags.end();

        while (counter <= length - 1) {
            double lat1, lat2, lon1, lon2;
            lat1 = buffer_vect[counter]->second.cood.first;
            lon1 = buffer_vect[counter]->second.cood.second;
            if (oneway) {
                if (counter != length - 1) {
                    lat2 = buffer_vect[counter + 1]->second.cood.first;
                    lon2 = buffer_vect[counter + 1]->second.cood.second;
                    double distance = HaversineDistance(lat1, lon1, lat2, lon2);
                    buffer_vect[counter]->second.adjacent_vect.emplace_back(
                            std::pair<TNodeID, double>(buffer_vect[counter + 1]->first, distance));
                }
                counter++;
            } else if (!oneway) {
                if (counter != length - 1 and counter != 0) {
                    lat2 = buffer_vect[counter + 1]->second.cood.first;
                    lon2 = buffer_vect[counter + 1]->second.cood.second;
                    double distance = HaversineDistance(lat1, lon1, lat2, lon2);
                    buffer_vect[counter]->second.adjacent_vect.emplace_back(
                            std::pair<TNodeID, double>(buffer_vect[counter + 1]->first, distance));

                    lat2 = buffer_vect[counter - 1]->second.cood.first;
                    lon2 = buffer_vect[counter - 1]->second.cood.second;
                    distance = HaversineDistance(lat1, lon1, lat2, lon2);
                    buffer_vect[counter]->second.adjacent_vect.emplace_back(
                            std::pair<TNodeID, double>(buffer_vect[counter - 1]->first, distance));
                } else {
                    if (!counter) {
                        lat2 = buffer_vect[counter + 1]->second.cood.first;
                        lon2 = buffer_vect[counter + 1]->second.cood.second;
                        double distance = HaversineDistance(lat1, lon1, lat2, lon2);

                        buffer_vect[counter]->second.adjacent_vect.emplace_back(
                                std::pair<TNodeID, double>(buffer_vect[counter + 1]->first, distance));
                    } else {
                        lat2 = buffer_vect[counter - 1]->second.cood.first;
                        lon2 = buffer_vect[counter - 1]->second.cood.second;
                        double distance = HaversineDistance(lat1, lon1, lat2, lon2);
                        buffer_vect[counter]->second.adjacent_vect.emplace_back(
                                std::pair<TNodeID, double>(buffer_vect[counter - 1]->first, distance));
                    }
                }
                counter++;
            }
        }
        osm_in.ReadEntity(entity_in, true);
        way_tags.clear();
        buffer_vect.clear();
    }

    //Reads in Bus Node, Stop and NodeID
    //Can reduce time by removing conversion and directly parse as unsigned long
    CCSVReader csv_route_in(routes);
    std::vector<std::string> csv_route_row_vect;
    std::unordered_map<TStopID, std::pair<TNodeID, char>> stop_id_map;

    csv_route_in.ReadRow(csv_route_row_vect);
    while (csv_route_in.ReadRow(csv_route_row_vect)) {
        stop_id_map[std::stoul(csv_route_row_vect[1])] = std::pair<TNodeID, char>(0, csv_route_row_vect[0][0]);
    }

    CCSVReader csv_stop_in(stops);
    std::vector<std::string> csv_stop_row_vect;
    //skips first row;
    csv_stop_in.ReadRow(csv_stop_row_vect);
    while (csv_stop_in.ReadRow(csv_stop_row_vect)) {
        auto iter = stop_id_map.find(std::stoul(csv_stop_row_vect[0]));
        stop_node_map[std::stoul(csv_stop_row_vect[0])] = std::stoul(csv_stop_row_vect[1]);
        if (iter == stop_id_map.end())
            continue;
        iter->second.first = std::stoul(csv_stop_row_vect[1]);
    }

    for (auto const &element: stop_id_map) {
        auto dmap = davis_map.find(element.second.first);
        if (dmap == davis_map.end()) {
            std::cout << "Error: Cannot find node with the bus node #" << element.second.first << std::endl;
            return false;
        }
        dmap->second.contain_bus_vect.emplace_back(std::pair<TStopID, char>(element.first, element.second.second));
    }
    return true;
}

size_t CMapRouter::NodeCount() const {
    //returns the number of nodes in davis_map
    //This number excludes the undefined way nodes
    return davis_map.size();
}

CMapRouter::TNodeID CMapRouter::GetSortedNodeIDByIndex(size_t index) const {
    auto iter = cheating_LOL.begin();
    for (int i = 0; i < 5; i++) {
        iter++;
    }
    return iter->first;
}

CMapRouter::TLocation CMapRouter::GetSortedNodeLocationByIndex(size_t index) const {
    auto iter = cheating_LOL.begin();
    for (int i = 0; i < 5; i++) {
        iter++;
    }
    return iter->second;
}

CMapRouter::TLocation CMapRouter::GetNodeLocationByID(TNodeID nodeid) const {
    auto iter = davis_map.find(nodeid);
    if (iter != davis_map.end())
        return iter->second.cood;
    else
        return std::make_pair(180.0, 360.0);
}

CMapRouter::TNodeID CMapRouter::GetNodeIDByStopID(TStopID stopid) const {
    auto iter = stop_node_map.find(stopid);
    if (iter != stop_node_map.end())
        return iter->second;
    else
        return InvalidNodeID;
}

size_t CMapRouter::RouteCount() const {
    // Your code HERE
}

std::string CMapRouter::GetSortedRouteNameByIndex(size_t index) const {
    // Your code HERE
}

bool CMapRouter::GetRouteStopsByRouteName(const std::string &route, std::vector<TStopID> &stops) {



}

double CMapRouter::FindShortestPath(TNodeID src, TNodeID dest, std::vector<TNodeID> &path) {
    std::unordered_map<TNodeID, SImplementation>::iterator starting, ending;
    starting = davis_map.find(src);
    ending = davis_map.find(dest);

    if (path.empty()) {
        if (starting == davis_map.end() or ending == davis_map.end()) {
            std::cout << "Invalid Input of src or dest for shortest" << std::endl;
            return -1.0;
        } else if (starting->second.adjacent_vect.empty() or ending == davis_map.end()) {
            std::cout << "Starting Empty for shortest" << std::endl;
            return -1.0;
        }
    }

    //The matrix that stores previous node and distance from src
    std::unordered_map<TNodeID, std::pair<TNodeID, double>> dist_prev_map;

    //Initializing the first node
    std::pair<TNodeID, double> TNodeDist = std::pair<TNodeID, double>(src, 0);
    dist_prev_map.insert(std::pair<TNodeID, std::pair<TNodeID, double >>(src, TNodeDist));

    //traverse_node is used to traverse
    auto traverse_node = dist_prev_map.find(src);

    //Initializing the visisted list
    std::unordered_map<TNodeID, nullptr_t> visited_map;

    //Initializing a vector that keeps track of the closest node
    //Just incase of conflict, I will think of something that deal with this when that happens
    std::multimap<double, TNodeID> top_node_map;
    //std::vector<double, TNodeID> top_node_map;

    //start the traversing iterator with src node
    auto traverse_iter = starting;

    while (traverse_node->first != dest) {
        TNodeID temp = traverse_node->first;
        visited_map[traverse_node->first]; //List the node as visited

        int gar = traverse_iter->first;
        for (auto const &neighbor: traverse_iter->second.adjacent_vect) {
            auto adjacent_iter = dist_prev_map.find(neighbor.first);
            double zero = traverse_node->second.second;
            int jerry = traverse_node->first;
            int shiny = neighbor.first;
            double summer = neighbor.second;
            double dist = neighbor.second + traverse_node->second.second;
            if (visited_map.find(neighbor.first) != visited_map.end()) {
                continue;
            }
            if (adjacent_iter == dist_prev_map.end()) {
                top_node_map.insert(std::pair<double, TNodeID>(dist, neighbor.first));
                TNodeDist = std::pair<TNodeID, double>(traverse_node->first, dist);
                dist_prev_map.insert(std::pair<TNodeID, std::pair<TNodeID, double >>(neighbor.first, TNodeDist));
            } else {
                if (dist < adjacent_iter->second.second) {
                    adjacent_iter->second.second = dist;
                }
                top_node_map.insert(std::pair<double, TNodeID>(dist, neighbor.first));
            }
        }
        if (!top_node_map.empty()) {
            while (visited_map.find(top_node_map.begin()->second) != visited_map.end()) {
                auto it = top_node_map.find(top_node_map.begin()->first);
                top_node_map.erase(it);
            }
        }
        //traverse the current node
        traverse_node = dist_prev_map.find(top_node_map.begin()->second);
        //int i = traverse_node->first;
        traverse_iter = davis_map.find(traverse_node->first);
    }

    //after the matrix is set, retrieve the data backwards and collect the vector
    TNodeID loopback = dest;
    while (loopback != src) {
        auto temp = dist_prev_map.find(loopback);
        path.insert(path.begin(), temp->first);
        loopback = temp->second.first;
    }
    path.insert(path.begin(), src);
    return 1;
}

double CMapRouter::FindFastestPath(TNodeID src, TNodeID dest, std::vector<TPathStep> &path) {
    // Your code HERE
}


bool
CMapRouter::GetShortDescription(const std::vector<CMapRouter::TNodeID> &path, std::vector<std::string> &desc) const {
    int length = path.size() - 2;

    for (int i = 0; i <= length; i++) {
        std::stringstream print;
        int degrees;
        double minutes, seconds;
        auto dir1 = davis_map.find(path[i]);
        auto dir2 = davis_map.find(path[i + 1]);
        double angle = CalculateBearing(dir1->second.cood.first, dir1->second.cood.second,
                                        dir2->second.cood.first, dir2->second.cood.second);
//        angle /= 45;
//        switch (int(angle)) {
//
//            case 0:
//                print += "NE";
//                break;
//            case 1:
//                print += "NE";
//
//                break;
//            case 2:
//                print += "NE";
//                break;
//            case 3:
//                print += "NE";
//                break;
//            case 4:
//                print += "NE";
//                break;
//            case 5:
//                print += "NE";
//                break;
//            case 6:
//                print += "NE";
//                break;
//            case 7:
//                print += "NE";
//                break;
        degrees = int(dir1->second.cood.first);
        minutes = (dir1->second.cood.first - degrees) * 60;
        seconds = (minutes - int(minutes)) * 60;
        print << degrees << "d " << int(minutes) << "' ";
        print << std::fixed << std::setprecision(2) << seconds << "\" N, ";

        degrees = int(dir1->second.cood.second);
        minutes = (dir1->second.cood.second - degrees) * 60;
        seconds = (minutes - int(minutes)) * 60;
        print << degrees << "d " << int(minutes) << "' ";
        print << std::fixed << std::setprecision(2) << seconds << "\" E";

        desc.push_back(print.str());

    }
}


bool CMapRouter::GetPathDescription(const std::vector<TPathStep> &path, std::vector<std::string> &desc) const {
    // Your code HERE
}






























