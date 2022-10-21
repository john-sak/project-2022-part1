// to_polygon.cpp
#include <iostream>
#include <string>

#include "../include/arguments.hpp"
#include "../include/polyline.hpp"

int main(int argc, char *argv[]) {
    arguments arg;
    try {
        // get arguments from command line
        arg.initialize(argc, argv);
    } catch (std::invalid_argument const &ex) {
        std::cerr << ex.what() << std::endl;
        std::cerr << "Usage: ./to_polygon -i <input_file> -o <output_file> -algorithm <\"incremental\" | \"convex_hull\"> -edge_selection <\"1\" | \"2\" | \"3\"> -initialization <\"1a\" | \"1b\" | \"2a\" | \"2b\">" << std::endl;
        return -1;
    } catch (std::exception const &ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    }
    try {
        // run the given algorithm, write results in given out_file
        polyline S(arg.get_points(), arg.get_alg(), arg.get_edge_sel(), arg.get_init(), arg.get_out_file());
    } catch (std::exception const &ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    }
    return 0;
}