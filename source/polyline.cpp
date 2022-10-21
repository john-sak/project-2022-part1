#include <vector>
#include <stdexcept>

#include "../include/polyline.hpp"

polyline::polyline(std::vector<std::pair<float, float>> vec, std::string alg, std::string edge_sel, std::string init, std::string out_file): points(vec), out_file(out_file) {
    this->edge_sel = std::stoi(edge_sel);
    if (this->edge_sel != 1 && this->edge_sel != 2 && this->edge_sel != 3) throw std::invalid_argument("Wrong arguments");
    try {
        // run the correct algorithm
        if (!alg.compare("incremental")) {
            int initialization;
            if (!init.compare("1a")) initialization = 1;
            else if (!init.compare("1b")) initialization = 2;
            else if (!init.compare("2a")) initialization = 3;
            else if (!init.compare("2b")) initialization = 4;
            else throw std::invalid_argument("Wrong arguments");
            this->incremental(initialization);
        } else if (!alg.compare("convex_hull")) {
            this->convex_hull();
        } else throw std::invalid_argument("Wrong arguments");
    } catch (...) {
        throw;
    }
    return;
}

void polyline::incremental(int init) {
    // todo
    return;
}

void polyline::convex_hull(void) {
    // todo
    return;
}