#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iostream>

#include "polyline.hpp"

polyline::polyline(std::vector<std::pair<float, float>> vec, std::string alg, std::string edge_sel, std::string init, std::string out_file): points(vec), out_file(out_file) {
    this->edge_sel = std::stoi(edge_sel);
    if (this->edge_sel != 1 && this->edge_sel != 2 && this->edge_sel != 3) throw std::invalid_argument("\'Edge selection\' must be \'1\', \'2\' or \'3\'");
    try {
        // run the correct algorithm
        if (!alg.compare("incremental")) {
            int initialization;
            if (!init.compare("1a")) initialization = 1;
            else if (!init.compare("1b")) initialization = 2;
            else if (!init.compare("2a")) initialization = 3;
            else if (!init.compare("2b")) initialization = 4;
            else throw std::invalid_argument("\'Initialization\' must be \'1a\', \'1b\', \'2a\' or \'2b\'");
            this->incremental(initialization);
        } else if (!alg.compare("convex_hull")) {
            this->convex_hull();
        } else throw std::invalid_argument("\'Algorithm\' must be \'incremental\' or \'convex_hull\'");
    } catch (...) {
        throw;
    }
    return;
}

void polyline::incremental(int init) {
    try {
        // sort this->points
        switch (init) {
            case 1:
                // x descending
                std::sort(this->points.begin(),this->points.end(),[] (const std::pair<float,float> &a, const std::pair<float,float> &b) {
                    return (a.first > b.first);
                });
                break;
            case 2:
                // x ascending
                std::sort(this->points.begin(),this->points.end());
                break;
            case 3:
                // y descending
                std::sort(this->points.begin(),this->points.end(),[] (const std::pair<float,float> &a, const std::pair<float,float> &b) {
                    return (a.second  > b.second);
                });
                break;
            case 4:
                // y ascending
                std::sort(this->points.begin(),this->points.end(),[] (const std::pair<float,float> &a, const std::pair<float,float> &b) {
                    return (a.second  < b.second);
                });
                break;
            default:
                throw "Error: Couldn't sort vector";
        }
        // todo the rest
    } catch (...) {
        throw;
    }
    return;
}

void polyline::convex_hull(void) {
    // todo
    return;
}

void polyline::print_points(void) const {
    for(std::pair<float,float> i : this->points)
        std::cout << i.first << " " << i.second << std::endl;
}
