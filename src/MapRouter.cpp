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

//Unit in mph
#define WALK_SPEED 3.0
#define BUS_SPEED 25.0
//Wait time in hours
#define BUS_WAIT_TIME (30/3600.0)

const CMapRouter::TNodeID CMapRouter::InvalidNodeID = -1;

double CMapRouter::PathToTime(std::vector<TNodeID> &path) {
    auto iter_post = ++path.begin();
    double tot_time = 0;
    for (auto const &element:path) {
        auto iter = davis_map.find(element);
        double time = -1;
        for (auto stct:iter->second.adjacent_node_vect) {
            if (stct.ID == *iter_post) {
                time = stct.time * WALK_SPEED / stct.speed;
            }
        }
        tot_time += time;
        if (++iter_post == path.end()) {
            break;
        }
    }
    path.clear();
    if (tot_time > 0)
        return tot_time + BUS_WAIT_TIME;
    else
        return tot_time;
}


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

    if (size_t(counter) != davis_map.size()) {
        std::cout << "Error: Losing values in hashtable" << std::endl;
    }


    while (osm_in.ReadEntity(entity_in, true)) {
        bool oneway = false;
        bool roundabout = false;
        int speed = -1;
        std::vector<std::unordered_map<TNodeID, SImplementation>::iterator> buffer_vect;
        std::vector<std::pair<std::string, std::string>> way_tags;

        while (entity_in.DNameData == "nd") {
            if (entity_in.DType != SXMLEntity::EType::StartElement) {
                osm_in.ReadEntity(entity_in, true);
                continue;
            }
            TNodeID target_id = std::stoul(entity_in.AttributeValue("ref"));
            auto iter = davis_map.find(target_id);
            if (iter == davis_map.end()) {
                osm_in.ReadEntity(entity_in, true);
                continue;
            } else {
                buffer_vect.emplace_back(iter);
                osm_in.ReadEntity(entity_in, true);
            }
        }
        if (buffer_vect.size() == 1) {
            continue;
        }
        while (entity_in.DNameData == "tag") {
            if (entity_in.DType != SXMLEntity::EType::StartElement) {
                osm_in.ReadEntity(entity_in, true);
                continue;
            }
            if (entity_in.AttributeValue("k") == "maxspeed") {
                speed = std::stoi(entity_in.AttributeValue("v"));
            }
            if (entity_in.AttributeValue("k") == "oneway") {
                if (entity_in.AttributeValue("v") == "yes") {
                    oneway = true;
                }
            }
            if (entity_in.AttributeValue("k") == "junction") {
                if (entity_in.AttributeValue("v") == "roundabout") {
                    roundabout = true;
                }
            }
            osm_in.ReadEntity(entity_in, true);
        }

        int length = buffer_vect.size();
        if (speed == -1)
            speed = BUS_SPEED;
        counter = 0;

        while (counter <= length - 1) {
            double lat1, lat2, lon1, lon2;
            lat1 = buffer_vect[counter]->second.cood.first;
            lon1 = buffer_vect[counter]->second.cood.second;
            if (oneway or roundabout) {
                buffer_vect[counter]->second.oneway = oneway;
                if (counter != length - 1) {
                    lat2 = buffer_vect[counter + 1]->second.cood.first;
                    lon2 = buffer_vect[counter + 1]->second.cood.second;
                    double time = HaversineDistance(lat1, lon1, lat2, lon2) / WALK_SPEED;
                    TImplementation temp;
                    temp.ID = buffer_vect[counter + 1]->first;
                    temp.time = time;
                    temp.speed = speed;
                    buffer_vect[counter]->second.adjacent_node_vect.emplace_back(temp);
                }
                counter++;
                if (roundabout and counter == length - 1) {
                    lat2 = buffer_vect[0]->second.cood.first;
                    lon2 = buffer_vect[0]->second.cood.second;
                    double time = HaversineDistance(lat1, lon1, lat2, lon2) / WALK_SPEED;
                    TImplementation temp;
                    temp.ID = buffer_vect[0]->first;
                    temp.time = time;
                    temp.speed = speed;
                    buffer_vect[counter]->second.adjacent_node_vect.emplace_back(temp);
                }
            } else {
                if (counter != length - 1 and counter != 0) {
                    lat2 = buffer_vect[counter + 1]->second.cood.first;
                    lon2 = buffer_vect[counter + 1]->second.cood.second;
                    double time = HaversineDistance(lat1, lon1, lat2, lon2) / WALK_SPEED;
                    TImplementation temp;
                    temp.ID = buffer_vect[counter + 1]->first;
                    temp.time = time;
                    temp.speed = speed;
                    buffer_vect[counter]->second.adjacent_node_vect.emplace_back(temp);

                    lat2 = buffer_vect[counter - 1]->second.cood.first;
                    lon2 = buffer_vect[counter - 1]->second.cood.second;
                    time = HaversineDistance(lat1, lon1, lat2, lon2) / WALK_SPEED;
                    temp.ID = buffer_vect[counter - 1]->first;
                    temp.time = time;
                    temp.speed = speed;
                    buffer_vect[counter]->second.adjacent_node_vect.emplace_back(temp);
                } else {
                    if (!counter) {
                        lat2 = buffer_vect[counter + 1]->second.cood.first;
                        lon2 = buffer_vect[counter + 1]->second.cood.second;
                        double time = HaversineDistance(lat1, lon1, lat2, lon2) / WALK_SPEED;
                        TImplementation temp;
                        temp.ID = buffer_vect[counter + 1]->first;
                        temp.time = time;
                        temp.speed = speed;
                        buffer_vect[counter]->second.adjacent_node_vect.emplace_back(temp);
                    } else {
                        lat2 = buffer_vect[counter - 1]->second.cood.first;
                        lon2 = buffer_vect[counter - 1]->second.cood.second;
                        double time = HaversineDistance(lat1, lon1, lat2, lon2) / WALK_SPEED;
                        TImplementation temp;
                        temp.ID = buffer_vect[counter - 1]->first;
                        temp.time = time;
                        temp.speed = speed;
                        buffer_vect[counter]->second.adjacent_node_vect.emplace_back(temp);
                    }
                }
                counter++;
            }
        }
        osm_in.ReadEntity(entity_in, true);
    }


    CCSVReader stop_in(stops);
    CCSVReader route_in(routes);
    std::vector<std::string> row_vect;

    stop_in.ReadRow(row_vect);
    while (stop_in.ReadRow(row_vect)) {
        stop_to_node_map[std::stoul(row_vect[0])] = std::stoul(row_vect[1]);
        node_to_stop_map[std::stoul(row_vect[1])] = std::stoul(row_vect[0]);
    }

    route_in.ReadRow(row_vect);
    route_in.ReadRow(row_vect);
    bool flag = true;
    while (flag) {
        std::vector<std::pair<TStopID, BImplementation>> buffer_row;
        char line = row_vect[0][0];
        while (row_vect[0][0] == line) {
            auto iter = stop_to_node_map.find(std::stoul(row_vect[1]));
            BImplementation temp;
            temp.ID = iter->second;
            temp.traverse_left_time = -1;
            temp.traverse_right_time = -1;
            davis_map.find(iter->second)->second.busline.emplace_back(std::make_pair(line, std::stoul(row_vect[1])));
            buffer_row.emplace_back(std::make_pair(std::stoul(row_vect[1]), temp));
            if (!route_in.ReadRow(row_vect)) {
                flag = false;
                break;
            }
        }

        if (buffer_row.front().first != buffer_row.back().first) {
            buffer_row.emplace_back(std::pair<TNodeID, BImplementation>(0, BImplementation()));
        }


        auto iter_post = ++buffer_row.begin();
        for (auto iter_mid = buffer_row.begin(); iter_mid != buffer_row.end(); iter_mid++) {
            std::vector<TNodeID> path;
            TNodeID id1 = stop_to_node_map.find(iter_mid->first)->second,
                    id2 = stop_to_node_map.find(iter_post->first)->second;
            if (FindShortestPath(id1, id2, path) < 1000000)
                iter_mid->second.traverse_right_time = PathToTime(path);
            if (FindShortestPath(id2, id1, path) < 1000000)
                iter_post->second.traverse_left_time = PathToTime(path);
            iter_post++;
            if (iter_post == buffer_row.end() or !iter_post->first) {
                break;
            }
        }
        cheating_busline[line];
        complete_maniac[line] = buffer_row;
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
    for (size_t i = 0; i < index; i++) {
        iter++;
    }
    return iter->first;
}

CMapRouter::TLocation CMapRouter::GetSortedNodeLocationByIndex(size_t index) const {
    auto iter = cheating_LOL.begin();
    if (index >= davis_map.size()) {
        return std::make_pair(180.0, 360.0);
    } else {
        for (size_t i = 0; i < index; i++) {
            iter++;
        }
        return iter->second;
    }
}

CMapRouter::TLocation CMapRouter::GetNodeLocationByID(TNodeID nodeid) const {
    auto iter = davis_map.find(nodeid);
    if (iter != davis_map.end())
        return iter->second.cood;
    else
        return std::make_pair(180.0, 360.0);
}

CMapRouter::TNodeID CMapRouter::GetNodeIDByStopID(TStopID stopid) const {
    auto iter = stop_to_node_map.find(stopid);
    if (iter == stop_to_node_map.end()) {
        return InvalidNodeID;
    } else {
        return iter->second;
    }
}

size_t CMapRouter::RouteCount() const {
    return cheating_busline.size();
}

std::string CMapRouter::GetSortedRouteNameByIndex(size_t index) const {
    auto iter = cheating_busline.begin();
    for (size_t i = 0; i < index; i++) {
        iter++;
    }
    return std::string(1, iter->first);
}

bool CMapRouter::GetRouteStopsByRouteName(const std::string &route, std::vector<TStopID> &stops) {
    auto bus_line = complete_maniac.find(route[0]);
    if (bus_line == complete_maniac.end()) {
        return false;
    } else {
        size_t i = 0;
        size_t length = bus_line->second.size() - 2;
        for (auto elements: bus_line->second) {
            stops.push_back(elements.first);
            if (i == length) {
                break;
            }
            i++;
        }
        stops.pop_back();
        return true;
    }
}

double CMapRouter::FindShortestPath(TNodeID src, TNodeID dest, std::vector<TNodeID> &path) {
    std::unordered_map<TNodeID, SImplementation>::iterator starting, ending;
    starting = davis_map.find(src);
    ending = davis_map.find(dest);

    if (path.empty()) {
        if (starting == davis_map.end() or ending == davis_map.end()) {
            //std::cout << "Invalid Input of src or dest for shortest" << std::endl;
            return std::numeric_limits<double>::max();
        } else if (starting->second.adjacent_node_vect.empty() or ending == davis_map.end()) {
            //std::cout << "Starting Empty for shortest" << std::endl;
            return std::numeric_limits<double>::max();
        }
    } else
        return std::numeric_limits<double>::max();


    //The matrix that stores previous node and distance from src
    std::unordered_map<TNodeID, std::pair<TNodeID, double>> dist_prev_map;

    //Initializing the first node
    std::pair<TNodeID, double> TNodeDist = std::pair<TNodeID, double>(src, 0);
    dist_prev_map.insert(std::pair<TNodeID, std::pair<TNodeID, double >>(src, TNodeDist));

    //traverse_node is used to traverse
    auto traverse_node = dist_prev_map.find(src);

    //Initializing the visisted list
    std::unordered_map<TNodeID, std::nullptr_t> visited_map;

    //Initializing a vector that keeps track of the closest node
    //Just incase of conflict, I will think of something that deal with this when that happens
    std::multimap<double, TNodeID> top_node_map;

    //start the traversing iterator with src node
    auto traverse_iter = starting;

    while (traverse_node->first != dest) {
        visited_map[traverse_node->first]; //List the node as visited

        for (auto const &neighbor: traverse_iter->second.adjacent_node_vect) {
            auto adjacent_iter = dist_prev_map.find(neighbor.ID);
            double dist = neighbor.time + traverse_node->second.second;
            if (visited_map.find(neighbor.ID) != visited_map.end()) {
                continue;
            }
            if (adjacent_iter == dist_prev_map.end()) {
                top_node_map.insert(std::pair<double, TNodeID>(dist, neighbor.ID));
                TNodeDist = std::pair<TNodeID, double>(traverse_node->first, dist);
                dist_prev_map.insert(std::pair<TNodeID, std::pair<TNodeID, double >>(neighbor.ID, TNodeDist));
            } else {
                if (dist < adjacent_iter->second.second) {
                    adjacent_iter->second.second = dist;
                }
                top_node_map.insert(std::pair<double, TNodeID>(dist, neighbor.ID));
            }
        }

        if (!top_node_map.empty()) {
            while (visited_map.find(top_node_map.begin()->second) != visited_map.end()) {
                auto it = top_node_map.find(top_node_map.begin()->first);
                top_node_map.erase(it);
            }
            if (top_node_map.empty()) {
                return std::numeric_limits<double>::max();
            }
        }


        //traverse the current node
        traverse_node = dist_prev_map.find(top_node_map.begin()->second);
        traverse_iter = davis_map.find(traverse_node->first);
    }

    //after the matrix is set, retrieve the data backwards and collect the vector
    TNodeID loopback = dest;
    auto temp = dist_prev_map.find(loopback);
    double dist = temp->second.second;
    while (loopback != src) {
        path.insert(path.begin(), temp->first);
        loopback = temp->second.first;
        temp = dist_prev_map.find(loopback);
    }
    path.insert(path.begin(), src);
    return dist * WALK_SPEED;
}

double CMapRouter::FindFastestPath(TNodeID src, TNodeID dest, std::vector<TPathStep> &path) {
    std::unordered_map<TNodeID, SImplementation>::iterator starting, ending;
    starting = davis_map.find(src);
    ending = davis_map.find(dest);

    if (path.empty()) {
        if (starting == davis_map.end() or ending == davis_map.end()) {
            //std::cout << "Invalid Input of src or dest for fastest" << std::endl;
            return std::numeric_limits<double>::max();
        } else if (starting->second.adjacent_node_vect.empty() or ending->second.adjacent_node_vect.empty()) {
            //std::cout << "This node is empty" << std::endl;
            return std::numeric_limits<double>::max();
        }
    }

    //The matrix that stores previous node and distance from src
    std::unordered_map<TNodeID, std::pair<TPathStep, double>> dist_prev_map;

    //Initializing the first node
    std::pair<TPathStep, double> TNodeDist = std::make_pair(std::make_pair("Walk", src), 0);
    dist_prev_map.insert(std::make_pair(src, TNodeDist));

    //traverse_node is used to traverse
    auto traverse_node = dist_prev_map.find(src);

    //Initializing the visisted list
    std::unordered_map<TNodeID, std::nullptr_t> visited_map;

    //Initializing a map that keeps track of the closest node
    //Just incase of conflict, I will think of something that deal with this when that happens
    std::multimap<double, TNodeID> top_node_map;

    //start the traversing iterator with src node
    auto traverse_iter = starting;

    while (traverse_node->first != dest) {
        visited_map[traverse_node->first]; //Add to the visited node
        for (auto const &neighbor:traverse_iter->second.busline) {
            auto vect = complete_maniac.find(neighbor.first)->second;
            int counter = 0;
            for (auto const &temp:vect) {
                if (temp.first == neighbor.second) {
                    break;
                }
                counter++;
            }
            const int length = vect.size() - 1;
            std::string method = "Bus ";
            method += neighbor.first;
            if (!counter) {
                if (visited_map.find(vect[1].second.ID) == visited_map.end()) {
                    auto adjacent_iter = dist_prev_map.find(vect[1].second.ID);
                    double time = vect[0].second.traverse_right_time + traverse_node->second.second;
                    TNodeDist = std::make_pair(std::make_pair(method, traverse_node->first), time);
                    if (adjacent_iter == dist_prev_map.end()) {
                        dist_prev_map.insert(std::make_pair(vect[1].second.ID, TNodeDist));
                        top_node_map.insert(std::pair<double, TNodeID>(time, vect[1].second.ID));
                    } else {
                        if (time < adjacent_iter->second.second) {
                            adjacent_iter->second = TNodeDist;
                        }
                        top_node_map.insert(std::pair<double, TNodeID>(time, vect[1].second.ID));
                    }
                }
//                if (vect[length].first and visited_map.find(vect[length - 1].second.ID) == visited_map.end()) {
//                    auto adjacent_iter = dist_prev_map.find(vect[length].second.ID);
//                    double time = vect[length].second.traverse_left_time + traverse_node->second.second;
//                    TNodeDist = std::make_pair(std::make_pair(method, traverse_node->first), time);
//                    if (adjacent_iter == dist_prev_map.end()) {
//                        dist_prev_map.insert(std::make_pair(vect[length].second.ID, TNodeDist));
//                        top_node_map.insert(std::pair<double, TNodeID>(time, vect[length].second.ID));
//                    } else {
//                        if (time < adjacent_iter->second.second) {
//                            adjacent_iter->second = TNodeDist;
//                        }
//                        top_node_map.insert(std::pair<double, TNodeID>(time, vect[length].second.ID));
//                    }
//                }
            } else if (counter == length - 1) {
                if (vect[length].second.ID and visited_map.find(vect[0].second.ID) == visited_map.end()) {
                    auto adjacent_iter = dist_prev_map.find(vect[0].second.ID);
                    double time = vect[counter].second.traverse_right_time + traverse_node->second.second;
                    TNodeDist = std::make_pair(std::make_pair(method, traverse_node->first), time);
                    if (adjacent_iter == dist_prev_map.end()) {
                        dist_prev_map.insert(std::make_pair(vect[0].second.ID, TNodeDist));
                        top_node_map.insert(std::pair<double, TNodeID>(time, vect[0].second.ID));
                    } else {
                        if (time < adjacent_iter->second.second) {
                            adjacent_iter->second = TNodeDist;
                        }
                        top_node_map.insert(std::pair<double, TNodeID>(time, vect[0].second.ID));
                    }
                }
//                if (visited_map.find(vect[length - 2].second.ID) == visited_map.end()) {
//                    auto adjacent_iter = dist_prev_map.find(vect[length - 2].second.ID);
//                    double time = vect[length - 1].second.traverse_left_time + traverse_node->second.second;
//                    TNodeDist = std::make_pair(std::make_pair(method, traverse_node->first), time);
//                    if (adjacent_iter == dist_prev_map.end()) {
//                        dist_prev_map.insert(std::make_pair(vect[length - 2].second.ID, TNodeDist));
//                        top_node_map.insert(std::pair<double, TNodeID>(time, vect[length - 1].second.ID));
//                    } else {
//                        if (time < adjacent_iter->second.second) {
//                            adjacent_iter->second = TNodeDist;
//                        }
//                        top_node_map.insert(std::pair<double, TNodeID>(time, vect[length - 1].second.ID));
//                    }
//                }
            } else {
                if (visited_map.find(vect[counter + 1].second.ID) == visited_map.end()) {
                    auto adjacent_iter = dist_prev_map.find(vect[counter + 1].second.ID);
                    double time = vect[counter].second.traverse_right_time + traverse_node->second.second;
                    TNodeDist = std::make_pair(std::make_pair(method, traverse_node->first), time);
                    if (adjacent_iter == dist_prev_map.end()) {
                        dist_prev_map.insert(std::make_pair(vect[counter + 1].second.ID, TNodeDist));
                        top_node_map.insert(std::pair<double, TNodeID>(time, vect[counter + 1].second.ID));
                    } else {
                        if (time < adjacent_iter->second.second) {
                            adjacent_iter->second = TNodeDist;
                        }
                        top_node_map.insert(std::pair<double, TNodeID>(time, vect[counter + 1].second.ID));
                    }
                }
//                if (visited_map.find(vect[counter - 1].second.ID) == visited_map.end()) {
//                    auto adjacent_iter = dist_prev_map.find(vect[counter - 1].second.ID);
//                    double time = vect[counter].second.traverse_left_time + traverse_node->second.second;
//                    TNodeDist = std::make_pair(std::make_pair(method, traverse_node->first), time);
//                    if (adjacent_iter == dist_prev_map.end()) {
//                        dist_prev_map.insert(std::make_pair(vect[counter - 1].second.ID, TNodeDist));
//                        top_node_map.insert(std::pair<double, TNodeID>(time, vect[counter - 1].second.ID));
//                    } else {
//                        if (time < adjacent_iter->second.second) {
//                            adjacent_iter->second = TNodeDist;
//                        }
//                        top_node_map.insert(std::pair<double, TNodeID>(time, vect[counter - 1].second.ID));
//                    }
//                }
            }
        }
        for (auto const &neighbor: traverse_iter->second.adjacent_node_vect) {
            auto adjacent_iter = dist_prev_map.find(neighbor.ID);
            double time = neighbor.time + traverse_node->second.second;
            TNodeDist = std::make_pair(std::make_pair("Walk", traverse_node->first), time);
            if (visited_map.find(neighbor.ID) == visited_map.end()) {
                if (adjacent_iter == dist_prev_map.end()) {
                    dist_prev_map.insert(std::make_pair(neighbor.ID, TNodeDist));
                    top_node_map.insert(std::pair<double, TNodeID>(time, neighbor.ID));
                } else {
                    if (time < adjacent_iter->second.second) {
                        adjacent_iter->second = TNodeDist;
                    }
                    top_node_map.insert(std::pair<double, TNodeID>(time, neighbor.ID));
                }
            }
        }
        if (!top_node_map.empty()) {
            while (visited_map.find(top_node_map.begin()->second) != visited_map.end()) {
                top_node_map.erase(top_node_map.find(top_node_map.begin()->first));
            }
        }
        //traverse the current node
        traverse_node = dist_prev_map.find(top_node_map.begin()->second);
        if (traverse_node == dist_prev_map.end())
            return -1;
        traverse_iter = davis_map.find(traverse_node->first);
    }

    //after the matrix is set, retrieve the data backwards and collect the vector
    TNodeID loopback = dest;
    auto temp = dist_prev_map.find(loopback);
    double time = temp->second.second;
    //std::unordered_map<TNodeID, std::pair<TPathStep, double>> dist_prev_map;
    while (loopback != src) {
        TPathStep dumb = std::pair<std::string, TNodeID>(temp->second.first.first, loopback);
        path.insert(path.begin(), dumb);
        loopback = temp->second.first.second;
        temp = dist_prev_map.find(loopback);
    }
    path.insert(path.begin(), std::make_pair("Walk", src));
    return time;
}


bool
CMapRouter::GetShortDescription(const std::vector<CMapRouter::TNodeID> &path, std::vector<std::string> &desc) const {
    if (path.empty() or !desc.empty())
        return false;
    std::stringstream print;
    int length = path.size() - 1;
    int degrees;
    int j = 0;
    double minutes, seconds;

    auto dir1 = davis_map.find(path[0]);
    auto dir2 = davis_map.find(path[j]);
    auto temp = dir1->second.cood.first;

    print << "Start at ";
    if (temp < 0)
        temp *= -1;
    degrees = int(temp);
    minutes = (temp - degrees) * 60;
    seconds = (minutes - int(minutes)) * 60;
    print << degrees << "d " << int(minutes) << "' ";
    if (seconds == 0) {
        print << std::fixed << std::setprecision(0) << seconds << "\" N, ";
    } else
        print << std::fixed << std::setprecision(2) << seconds << "\" N, ";


    temp = dir1->second.cood.second;

    if (temp < 0)
        temp *= -1;
    degrees = int(temp);
    minutes = (temp - degrees) * 60;
    seconds = (minutes - int(minutes)) * 60;
    print << degrees << "d " << int(minutes) << "' ";
    if (seconds == 0) {
        print << std::fixed << std::setprecision(0) << seconds << "\" N, ";
    } else
        print << std::fixed << std::setprecision(2) << seconds << "\" E";

    desc.push_back(print.str());


    for (int i = 0; i <= length - 1; i++) {
        print.str("");
        j = i;
        if (i != length) {
            j++;
        }
        dir1 = davis_map.find(path[i]);
        dir2 = davis_map.find(path[j]);
        double angle = CalculateBearing(dir1->second.cood.first, dir1->second.cood.second,
                                        dir2->second.cood.first, dir2->second.cood.second);

        if (angle < 0) {
            angle += 360;
        }
        angle /= 22.5;
        switch (int(angle)) {
            case 0:
            case 15:
                print << "Walk N";
                break;
            case 1:
            case 2:
                print << "Walk NE";
                break;
            case 3:
            case 4:
                print << "Walk E";
                break;
            case 5:
            case 6:
                print << "Walk SE";
                break;
            case 7:
            case 8:
                print << "Walk S";
                break;
            case 9:
            case 10:
                print << "Walk SW";
                break;
            case 11:
            case 12:
                print << "Walk W";
                break;
            case 13:
            case 14:
                print << "Walk NW";
                break;
            default:
                std::cout << "Error Angle" << std::endl;
                break;
        }
        print << " to ";
        temp = dir2->second.cood.first;
        if (temp < 0)
            temp *= -1;
        degrees = int(temp);
        minutes = (temp - degrees) * 60;
        seconds = (minutes - int(minutes)) * 60;
        print << degrees << "d " << int(minutes) << "' ";
        if (seconds == 0) {
            print << std::fixed << std::setprecision(0) << seconds << "\" N, ";
        } else
            print << std::fixed << std::setprecision(2) << seconds << "\" N, ";

        temp = dir2->second.cood.second;
        if (temp < 0)
            temp *= -1;
        degrees = int(temp);
        minutes = (temp - degrees) * 60;
        seconds = (minutes - int(minutes)) * 60;
        print << degrees << "d " << int(minutes) << "' ";
        if (seconds == 0) {
            print << std::fixed << std::setprecision(0) << seconds << "\" E";
        } else
            print << std::fixed << std::setprecision(2) << seconds << "\" E";

        desc.push_back(print.str());
    }
    print.str("");
    print << "End at ";
    temp = dir2->second.cood.first;
    if (temp < 0)
        temp *= -1;
    degrees = int(temp);
    minutes = (temp - degrees) * 60;
    seconds = (minutes - int(minutes)) * 60;
    print << degrees << "d " << int(minutes) << "' ";
    if (seconds == 0) {
        print << std::fixed << std::setprecision(0) << seconds << "\" N, ";
    } else
        print << std::fixed << std::setprecision(2) << seconds << "\" N, ";

    temp = dir2->second.cood.second;

    if (temp < 0)
        temp *= -1;
    degrees = int(temp);
    minutes = (temp - degrees) * 60;
    seconds = (minutes - int(minutes)) * 60;
    print << degrees << "d " << int(minutes) << "' ";
    if (seconds == 0) {
        print << std::fixed << std::setprecision(0) << seconds << "\" E";
    } else
        print << std::fixed << std::setprecision(2) << seconds << "\" E";

    desc.push_back(print.str());
    return true;
}


bool CMapRouter::GetPathDescription(const std::vector<TPathStep> &path, std::vector<std::string> &desc) const {
    if (path.empty() or !desc.empty())
        return false;
    std::stringstream print;
    int length = path.size() - 1;
    int degrees;
    int j = 0;
    double minutes, seconds;
    bool bus_flag = false;
    TStopID bus_stop = 0;

    auto dir1 = davis_map.find(path[0].second);
    auto dir2 = davis_map.find(path[j].second);
    auto temp = dir1->second.cood.first;

    print << "Start at ";
    if (temp < 0)
        temp *= -1;
    degrees = int(temp);
    minutes = (temp - degrees) * 60;
    seconds = (minutes - int(minutes)) * 60;
    print << degrees << "d " << int(minutes) << "' ";
    if (seconds == 0) {
        print << std::fixed << std::setprecision(0) << seconds << "\" N, ";
    } else
        print << std::fixed << std::setprecision(2) << seconds << "\" N, ";

    temp = dir1->second.cood.second;

    if (temp < 0)
        temp *= -1;
    degrees = int(temp);
    minutes = (temp - degrees) * 60;
    seconds = (minutes - int(minutes)) * 60;
    print << degrees << "d " << int(minutes) << "' ";
    if (seconds == 0) {
        print << std::fixed << std::setprecision(0) << seconds << "\" E";
    } else
        print << std::fixed << std::setprecision(2) << seconds << "\" E";
    desc.push_back(print.str());


    for (int i = 0; i <= length; i++) {
        j = i;
        if (i != length) {
            j++;
            dir1 = davis_map.find(path[i].second);
            dir2 = davis_map.find(path[j].second);
        } else {
            dir1 = davis_map.find(path[length - 1].second);
            dir2 = davis_map.find(path[length].second);
        }
        double angle = CalculateBearing(dir1->second.cood.first, dir1->second.cood.second,
                                        dir2->second.cood.first, dir2->second.cood.second);

        if (path[i].first[0] == 'W' and path[j].first[0] != 'B') {
            if (bus_flag) {
                print << " and get off at stop " << node_to_stop_map.find(bus_stop)->second;
                desc.push_back(print.str());
                bus_flag = false;
            }
            print.str("");
            print << "Walk ";
            if (angle < 0) {
                angle += 360;
            }
            angle /= 22.5;
            switch (int(angle)) {
                case 0:
                case 15:
                    print << "N";
                    break;
                case 1:
                case 2:
                    print << "NE";
                    break;
                case 3:
                case 4:
                    print << "E";
                    break;
                case 5:
                case 6:
                    print << "SE";
                    break;
                case 7:
                case 8:
                    print << "S";
                    break;
                case 9:
                case 10:
                    print << "SW";
                    break;
                case 11:
                case 12:
                    print << "W";
                    break;
                case 13:
                case 14:
                    print << "NW";
                    break;
                default:
                    std::cout << "Error Angle" << std::endl;
                    break;
            }
            print << " to ";
            temp = dir2->second.cood.first;
            if (temp < 0)
                temp *= -1;
            degrees = int(temp);
            minutes = (temp - degrees) * 60;
            seconds = (minutes - int(minutes)) * 60;
            print << degrees << "d " << int(minutes) << "' ";
            if (seconds == 0) {
                print << std::fixed << std::setprecision(0) << seconds << "\" N, ";
            } else
                print << std::fixed << std::setprecision(2) << seconds << "\" N, ";

            temp = dir2->second.cood.second;
            if (temp < 0)
                temp *= -1;
            degrees = int(temp);
            minutes = (temp - degrees) * 60;
            seconds = (minutes - int(minutes)) * 60;
            print << degrees << "d " << int(minutes) << "' ";
            if (seconds == 0) {
                print << std::fixed << std::setprecision(0) << seconds << "\" E";
            } else
                print << std::fixed << std::setprecision(2) << seconds << "\" E";

            desc.push_back(print.str());
        } else {
            if (bus_flag) {
                bus_stop = path[i].second;
            } else {
                print.str("");
                print << "Take " << path[j].first;
                bus_stop = path[i].second;
                bus_flag = true;
            }
        }
    }

    if (bus_flag) {
        print << " and get off at stop " << node_to_stop_map.find(bus_stop)->second;
        desc.push_back(print.str());
        print.str("");
    }

    print.str("");
    print << "End at ";
    temp = dir2->second.cood.first;
    if (temp < 0)
        temp *= -1;
    degrees = int(temp);
    minutes = (temp - degrees) * 60;
    seconds = (minutes - int(minutes)) * 60;
    print << degrees << "d " << int(minutes) << "' ";
    if (seconds == 0) {
        print << std::fixed << std::setprecision(0) << seconds << "\" N, ";
    } else
        print << std::fixed << std::setprecision(2) << seconds << "\" N, ";

    temp = dir2->second.cood.second;

    if (temp < 0)
        temp *= -1;
    degrees = int(temp);
    minutes = (temp - degrees) * 60;
    seconds = (minutes - int(minutes)) * 60;
    print << degrees << "d " << int(minutes) << "' ";
    if (seconds == 0) {
        print << std::fixed << std::setprecision(0) << seconds << "\" E";
    } else
        print << std::fixed << std::setprecision(2) << seconds << "\" E";
    desc.push_back(print.str());
    return true;
}






























