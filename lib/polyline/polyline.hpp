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
        std::vector<Point> points;
        std::vector<Point> pl_points;
        std::vector<Segment> poly_line;
        int edge_sel;
        std::string out_file;
        int pl_area;
        int ch_area;
        void incremental(int);
        void convex_hull(void);
        void sort_points(int);
        int init_polygon(void);
        void expand(int);
        Segment min_area(std::vector<Segment>, int);
        Segment max_area(std::vector<Segment>, int);
        void insert_point(Segment, int);
        std::vector<Point> get_ch(int);
        std::vector<Segment> get_segment(std::vector<Point>);
        std::vector<Segment> get_red_edges(std::vector<Segment>, std::vector<Segment>);
        std::vector<Segment> get_vis_edges(int, std::vector<Segment>);
        bool is_vis(Segment, Segment); 
        void write_to_file(std::string, int) const;
    public:
        polyline(std::vector<std::pair<float, float>>, std::string, std::string, std::string, std::string);
        void print_points(void) const;
};

#endif
