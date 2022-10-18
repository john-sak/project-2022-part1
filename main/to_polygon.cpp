// to_polygon.cpp
#include <iostream>
#include <list>
#include <string>

#include "../include/arguments.hpp"
#include "../include/data.hpp"

int main(int argc, char *argv[]) {
    try {
        arguments arg(argc, argv);
    } catch (std::exception &e) {
        std::cerr << "Usage: ./to_polygon -i <input_file> -o <output_file> -algorithm <\"incremental\" | \"convex_hull\"> -edge_selection <\"1\" | \"2\" | \"3\"> -initialization <\"1a\" | \"1b\" | \"2a\" | \"2b\">" << std::endl;
        return -1;
    }
    // todo
    return 0;
}