#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iostream>

#include <CGAL/convex_hull_2.h>
#include <CGAL/Convex_hull_traits_adapter_2.h>
#include <CGAL/property_map.h>
#include <CGAL/intersections.h>

#include <polyline.hpp>

typedef CGAL::Convex_hull_traits_adapter_2<K, CGAL::Pointer_property_map<Point>::type> Convex_hull_traits_2;
typedef K::Intersect_2 Intersect;

void polyline::incremental(int init) {
    try {
        // sort this->points
        this->sort_points(init);

        // initialize polygon
        int i = this->init_polygon();

        // expand polygon
        this->expand(i);

        // write to output file
        this->write_to_file();
    } catch (...) {
        throw;
    }
    return;
}

void polyline::convex_hull(void) {
    try {
        // convex hull
    } catch (...) {
        throw;
    }
    return;
}

void polyline::sort_points(int type) {
    switch (type) {
        case 1:
            // sort x descending
            std::sort(this->points.begin(), this->points.end(), [] (const Point &a, const Point &b) {
                return (a.x() > b.x());
            });
            break;
        case 2:
            // sort x ascending
            std::sort(this->points.begin(), this->points.end(), [] (const Point &a, const Point &b) {
                return (a.x() < b.x());
            });
            break;
        case 3:
            // sort y descending
            std::sort(this->points.begin(), this->points.end(), [] (const Point &a, const Point &b) {
                return (a.y() > b.y());
            });
            break;
        case 4:
            // sort y ascending
            std::sort(this->points.begin(), this->points.end(), [] (const Point &a, const Point &b) {
                return (a.y() < b.y());
            });
            break;
        default:
            throw "Error: Couldn't sort vector";
    }
}

int polyline::init_polygon(void) {
    try {
        this->pl_points.push_back(this->points[0]);
        this->pl_points.push_back(this->points[1]);
        this->pl_points.push_back(this->points[2]);

        this->poly_line.push_back(Segment(this->points[0], this->points[1]));
        this->poly_line.push_back(Segment(this->points[1], this->points[2]));
        //if three initial points are not collinear triangle can be created
        if (!CGAL::collinear(this->points[0], this->points[1], this->points[2])) {
            this->poly_line.push_back(Segment(this->points[2], this->points[0]));
            return 3;
        }
        //if initial points are collinear 
        int i = 3;
        //collinearity flag
        int flag = 1;
        // stop when the first non collinear point is found
        do {
            //create next segment
            this->pl_points.push_back(this->points[i]);
            this->poly_line.push_back(Segment(this->points[i - 1], this->points[i]));
            if (!CGAL::collinear(this->points[0], this->points[1], this->points[i])) {
                flag = 0;
                //point i is not collinear so triangle can be closed
                this->poly_line.push_back(Segment(this->points[i], this->points[0]));
            }
            i++;
        } while (flag);
        // return number of points needed for triangle creation
        return i;
    } catch (...) {
        throw;
    }
}

void polyline::expand(int i) {
    try {
        // current convex hull variable
        std::vector<Point> curr_ch;
        // previous convex hull variable
        std::vector<Point> prev_ch;

        std::vector<Segment> curr_ch_segment;
        std::vector<Segment> prev_ch_segment;

        std::vector<Segment> red_edges;
        std::vector<Segment> vis_edges;
        // get convex hull of current polygon points
        curr_ch = get_ch(i - 1);
        while (i < this->points.size()) {
            // insert next point to polygon line points
            this->pl_points.push_back(this->points[i]);
            
            prev_ch = curr_ch;
            prev_ch_segment = get_segment(prev_ch);
            // get convex hull of current polygon points plus the next point to add
            curr_ch = get_ch(i);
            curr_ch_segment = get_segment(curr_ch);
            // red lines of current convex hull are the ones removed from previous convex hull

            // compare the two segments to get red lines 
            red_edges = get_red_edges(prev_ch_segment, curr_ch_segment);

            // for every red find visible
            vis_edges = get_vis_edges(i, red_edges);

            // choose visible
            // todo choose visible line (edge_sel)
            i++; 
        }
        return;
    } catch (...) {
        throw;
    }
}

std::vector<Point> polyline::get_ch(int i) {
    std::vector<Point> curr_ch;
            
    std::vector<std::size_t> indices(this->pl_points.size()), out;
    std::iota(indices.begin(), indices.end(),0);

    CGAL::convex_hull_2(indices.begin(), indices.end(), std::back_inserter(out), Convex_hull_traits_2(CGAL::make_property_map(this->pl_points)));
    // push back points to current convex hull variable
    for( std::size_t j : out) curr_ch.push_back(this->pl_points[j]);

    return curr_ch;
}

std::vector<Segment> polyline::get_segment(std::vector<Point> points) {
    std::vector<Segment> seg;
    int i = 0;

    while(i != (points.size() - 1)) {
        seg.push_back(Segment(points[i], points[i+1]));
        i++;
    }
    seg.push_back(Segment(points[points.size() - 1], points[0]));
    return seg;
}

std::vector<Segment> polyline::get_red_edges(std::vector<Segment> prev, std::vector<Segment> curr) {
    std::vector<Segment> seg;

    // red edges are the edges that are on the previous convex hull, but not the current one
    for (Segment line : prev) if ((*(std::find(curr.begin(), curr.end(), line)) == *curr.end()) && (line != *curr.end())) seg.push_back(line);
    // test
    std::cout << "RED EDGES" << std::endl;
    for( auto it = seg.begin(); it != seg.end(); ++it) std::cout << it->source() << " " << it->target() << std::endl;

    return seg;
}
// for every red edge, checks whether it is visible from point i
std::vector<Segment> polyline::get_vis_edges(int i, std::vector<Segment> red_edges) {
    std::vector<Segment> seg;
    Point p = this->points[i];

    for (Segment red : red_edges) {
        // check if convex hull red edge is also polyline edge
        // if the condition is true the line is visible
        int flag = 1;
        for (Segment line : this->poly_line) {
            if (red == line) {
                flag = 0;
                seg.push_back(line);
                break;
            }
        }
        // if red edge is part of the polyline there no more visible edges
        if(flag == 0) continue;
        // if red edge is not part of polyline
        for (auto it = this->poly_line.begin(); it != this->poly_line.end(); ++it) {
            // when the starting point of polyline is the same as the one of the red edge
            // possible visible edges exist
            if (red.source() == it->source()) {
                do {
                    int flag = 1;
                    // create edges connecting the start and end point 
                    // of the possible visible edge with the point
                    Segment red1(p, it->source()), red2(p, it->target());
                    // for each polyline edge check if it intersects with the edges created
                    // if at least one polyline intersects the possible visible edge is not visible
                    for(Segment line : this->poly_line)
                        if (intersection(red1, line) || intersection(red2, line)) {
                            flag = 0;
                            break;
                        }
                    if (flag == 1) seg.push_back(*it);
                    it++;
                    // stop when the end of a polyline edge is the same as of the red edge    
                }while(red.target() != it->target());
                // found all visible edges for this red edge
                break;
            }
        }
    }
    return seg;
}

void polyline::write_to_file(void) const {
    // todo
    return;
}

polyline::polyline(std::vector<std::pair<float, float>> vec, std::string alg, std::string edge_sel, std::string init, std::string out_file): out_file(out_file) {
    //initialize points
    for(auto it = vec.begin(); it != vec.end(); ++it)
        points.push_back(Point(it->first,it->second));
    this->edge_sel = std::stoi(edge_sel);
    if (this->edge_sel != 1 && this->edge_sel != 2 && this->edge_sel != 3) throw std::invalid_argument("\'Edge selection\' must be \'1\', \'2\' or \'3\'");
    try {
        // run the correct algorithm
        if (!alg.compare("incremental")) {
            if (!init.compare("1a")) this->incremental(1);
            else if (!init.compare("1b")) this->incremental(2);
            else if (!init.compare("2a")) this->incremental(3);
            else if (!init.compare("2b")) this->incremental(4);
            else throw std::invalid_argument("\'Initialization\' must be \'1a\', \'1b\', \'2a\' or \'2b\'");
        } else if (!alg.compare("convex_hull")) {
            this->convex_hull();
        } else throw std::invalid_argument("\'Algorithm\' must be \'incremental\' or \'convex_hull\'");
    } catch (...) {
        throw;
    }
    return;
}

void polyline::print_points(void) const {
    for(auto it = points.begin(); it != points.end(); ++it) std::cout << it->x() << " " << it->y() << std::endl;
    return;
}
