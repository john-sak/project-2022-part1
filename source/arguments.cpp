#include "../include/arguments.hpp"

arguments::arguments(int argc, char *argv[]) {
    // todo
    return;
}

std::string arguments::get_in_file(void) const {
    return this->in_file;
}

std::string arguments::get_out_file(void) const {
    return this->out_file;
}

std::string arguments::get_alg(void) const {
    return this->alg;
}

std::string arguments::get_edge_sel(void) const {
    return this->edge_sel;
}

std::string arguments::get_init(void) const {
    return this->init;
}

std::list<std::pair<float, float>> arguments::get_points(void) const {
    return this->points;
}