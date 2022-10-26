#ifndef POLYLINE_HPP
#define POLYLINE_HPP

#include <string>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef CGAL::Polygon_2<K> Polygon;
typedef K::Segment_2 Segment;

class polyline {
    private:
        std::vector<std::pair<float, float>> points;
        int edge_sel;
        Polygon PL, CH;
        std::string out_file;
        void incremental(int);
        void convex_hull(void);
        void sort_points(int);
        int init_polygon(void);
        void expand(int);
        void write_to_file(void) const;
    public:
        polyline(std::vector<std::pair<float, float>>, std::string, std::string, std::string, std::string);
        void print_points(void) const;
};

#endif
