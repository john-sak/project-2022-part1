#ifndef POLYLINE_HPP
#define POLYLINE_HPP

#include <string>
#include <vector>

class polyline {
    private:
        std::vector<std::pair<float, float>> points;
        int edge_sel;
        std::string out_file;
        void incremental(int);
        void convex_hull();
    public:
        polyline(std::vector<std::pair<float, float>>, std::string, std::string, std::string, std::string);
};

#endif