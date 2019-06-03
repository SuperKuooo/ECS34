#ifndef MAPROUTER_H
#define MAPROUTER_H

#include <vector>
#include <map>
#include <unordered_map>
#include <istream>



class CMapRouter{
    public:
        using TNodeID = unsigned long;
        using TStopID = unsigned long;
        using TLocation = std::pair<double, double>; //lat then lon
        using TPathStep = std::pair<std::string, TNodeID>;
        using BImplementation = struct{
            TNodeID ID = -1;
            //from 1 to 2, 2 to 3, 3 to 4 etc..
            double traverse_right_time = -1;

            //from 4 to 3, 3 to 2, 2 to 1 etc..
            double traverse_left_time = -1;
        };
        using TImplementation = struct{
            TNodeID ID;
            double time;
            int speed;
        };
        using SImplementation = struct{
            TLocation cood;
            bool oneway = false;
            std::vector<std::pair<char, TStopID>> busline;
            std::vector<TImplementation> adjacent_node_vect;
        };

        static const TNodeID InvalidNodeID;

    private:
        std::map<TNodeID, TLocation> cheating_LOL;
        // Can add another map so you don't have to traverse the vector to find the StopID
        std::unordered_map<char, std::vector<std::pair<TStopID, BImplementation>>> complete_maniac;
        std::unordered_map<TStopID, TNodeID> stop_to_node_map;
        std::unordered_map<TNodeID, SImplementation> davis_map;
    public:
        CMapRouter();
        ~CMapRouter();

        static double HaversineDistance(double lat1, double lon1, double lat2, double lon2);
        static double CalculateBearing(double lat1, double lon1,double lat2, double lon2);
        double PathToTime(std::vector<TNodeID> &path);

        bool LoadMapAndRoutes(std::istream &osm, std::istream &stops, std::istream &routes);
        size_t NodeCount() const;
        TNodeID GetSortedNodeIDByIndex(size_t index) const;
        TLocation GetSortedNodeLocationByIndex(size_t index) const;
        TLocation GetNodeLocationByID(TNodeID nodeid) const;
        TNodeID GetNodeIDByStopID(TStopID stopid) const;
        size_t RouteCount() const;
        std::string GetSortedRouteNameByIndex(size_t index) const;
        bool GetRouteStopsByRouteName(const std::string &route, std::vector< TStopID > &stops);

        double FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path);
        double FindFastestPath(TNodeID src, TNodeID dest, std::vector< TPathStep > &path);
        bool GetPathDescription(const std::vector< TPathStep > &path, std::vector< std::string > &desc) const;
        bool GetShortDescription(const std::vector<TNodeID> &path, std::vector<std::string> &desc) const;
};

#endif
