#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iostream>

#include <CGAL/convex_hull_2.h>
#include <CGAL/Convex_hull_traits_adapter_2.h>
#include <CGAL/property_map.h>

#include <polyline.hpp>

typedef CGAL::Convex_hull_traits_adapter_2<K,
          CGAL::Pointer_property_map<Point>::type > Convex_hull_traits_2;


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

        // for(auto it = points.begin(); it != points.end(); ++it) p.push_back(Point(it->first, it->second));
        // for (auto vi = p.vertices_begin(); vi != p.vertices_end(); ++vi) std::cout << *vi << std::endl;
        // if(p.is_simple())
        //     std::cout << "P is simple" << std::endl;
        // else
        //     std::cout << "P isn't simple" << std::endl;
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
        // stop while when the first non collinear point is found
        do {
            //create netx segment
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
        // get convex hull of current polygon points plus the next point to add
        curr_ch = get_ch(i);

        while (i < this->points.size()) {
            // insert next point to polygon line points
            this->pl_points.push_back(this->points[i]);
            
            prev_ch = curr_ch;

            // get convex hull of current polygon points plus the next point to add
            curr_ch = get_ch(i);

            // red lines of current convex hull are the ones removed from previous convex hull
            // probably need to create segments of both hulls to find the difference
            

            i++; 
            // find red lines of CH
            // for every red find visible
            // choose visible (edge_sel)
        }
        for( auto it = curr_ch.begin(); it != curr_ch.end(); ++it) std::cout << it->x() << " " << it->y() << std::endl;
        std::cout << "PREV" << std::endl;
        for( auto it = prev_ch.begin(); it != prev_ch.end(); ++it) std::cout << it->x() << " " << it->y() << std::endl;
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
