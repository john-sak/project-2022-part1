#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <limits>
#include <chrono>

#include <CGAL/convex_hull_2.h>
#include <CGAL/Convex_hull_traits_adapter_2.h>
#include <CGAL/property_map.h>
#include <CGAL/intersections.h>

#include <polyline.hpp>

typedef CGAL::Convex_hull_traits_adapter_2<K, CGAL::Pointer_property_map<Point>::type> Convex_hull_traits_2;
typedef K::Intersect_2 Intersect;

void polyline::incremental(int init) {
    try {
        auto start = std::chrono::high_resolution_clock::now();

        // sort points
        this->sort_points(init);

        // initialize polygon
        int i = this->init_triangle();

        // expand polygon
        this->expand(i);

        auto stop = std::chrono::high_resolution_clock::now();

        // write to output file
        this->write_to_file("Incremental", std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count());
    } catch (...) {
        throw;
    }
    return;
}

void polyline::convex_hull(void) {
    try {
        auto start = std::chrono::high_resolution_clock::now();

        // get convex_hull of all points
        std::vector<Point> ch;
        std::vector<std::size_t> indices(this->points.size()), out;
        std::iota(indices.begin(), indices.end(), 0);

        CGAL::convex_hull_2(indices.begin(), indices.end(), std::back_inserter(out), Convex_hull_traits_2(CGAL::make_property_map(this->points)));
        for (std::size_t j : out) ch.push_back(this->points[j]);

        // initialize polyline to be the convex_hull of all the points
        for (int i = 0; i < ch.size(); i++) {
            this->pl_points.push_back(ch[i]);
            if (i < ch.size() - 1) this->poly_line.push_back(Segment(ch[i], ch[i + 1]));
            else this->poly_line.push_back(Segment(ch[i], ch[0]));
        }

        // for every edge in poly_line, find the closest visible point p and add it to the polyline
        // when there are no more edges to go through, we are done
        std::vector<Segment>::size_type size = this->poly_line.size();
        for (std::vector<Segment>::size_type i = 0; i < size; i++) {
            // ===== TODO =====
            // find closest visible point p and let p_index be the index of point p
            // ===== TODO =====

            // insert p to polyline and remove this->poly_line[i]
            this->insert_point(this->poly_line[i], p_index);

            // increment the size of the polyline
            size += 2;
        }

        auto stop = std::chrono::high_resolution_clock::now();

        // write to output file
        this->write_to_file("Convex_Hull", std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count());
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

int polyline::init_triangle(void) {
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
        curr_ch = get_ch();

        while (i < this->points.size()) {
            Point p = this->points[i];

            // insert next point to polygon line points
            this->pl_points.push_back(this->points[i]);

            prev_ch = curr_ch;
            prev_ch_segment = this->get_segment(prev_ch);

            // get convex hull of current polygon points plus the next point to add
            curr_ch = get_ch();
            curr_ch_segment = this->get_segment(curr_ch);

            // red lines of current convex hull are the ones removed from previous convex hull
            // compare the two segments to get red lines 
            red_edges = this->get_red_edges(prev_ch_segment, curr_ch_segment);

            // point has the same x or y coordinate (depends of sorting) as a prev one
            if (red_edges.size() == 0) {

                if (!this->init.compare("1a") || this->init.compare("1b")) {

                    // find red edes
                    for (auto it = curr_ch_segment.begin(); it != curr_ch_segment.end(); it++) {
                        if(it->source().x() == p.x() || it->target().x() == p.x())
                            red_edges.push_back(*it);
                    }
                } else if (!this->init.compare("2a") || this->init.compare("2b")) {

                    // find red edes
                    for (auto it = curr_ch_segment.begin(); it != curr_ch_segment.end(); it++) {
                        if(it->source().y() == p.y() || it->target().y() == p.y())
                            red_edges.push_back(*it);
                    }
                }
            }

            // for every red find visible
            vis_edges = this->get_vis_edges(i, red_edges);

            Segment replaceable_edge;
            switch(edge_sel) {
                case 1:
                    // choose visible replaceable edge randomly
                    replaceable_edge = vis_edges[std::rand() % vis_edges.size()];
                    break;

                case 2:
                    // choose visible repleceable edge that adds min area
                    replaceable_edge = min_area(vis_edges, i);
                    break;

                case 3:
                    // choose visible repleceable edge that adds max area
                    replaceable_edge = max_area(vis_edges, i);
                    break;

                default:
                    throw "Error: Wrong edge_selection value!";
            }
            std::cout << "POLYGON" << std::endl;
            for (auto it = poly_line.begin(); it != poly_line.end(); it++) std::cout << it->source() << " " << it->target() << std::endl;
            insert_point(replaceable_edge, i);
            i++; 
        }

        // TESTING
        Polygon p;
        for(auto it = this->poly_line.begin(); it != this->poly_line.end(); ++it) p.push_back(it->target());
        if(p.is_simple()) std::cout << "SIMPLE" << std::endl;
        else std::cout << "CONVEX" << std::endl;
        std::cout << "POLYGON" << std::endl;
        for(auto it = this->poly_line.begin(); it != this->poly_line.end(); ++it) std::cout << it->source() << " " << it->target() << std::endl;
        // TESTING

        return;
    } catch (...) {
        throw;
    }
}

std::vector<Point> polyline::get_ch(void) {
    std::vector<Point> curr_ch;

    std::vector<std::size_t> indices(this->pl_points.size()), out;
    std::iota(indices.begin(), indices.end(), 0);

    CGAL::convex_hull_2(indices.begin(), indices.end(), std::back_inserter(out), Convex_hull_traits_2(CGAL::make_property_map(this->pl_points)));

    // push back points to current convex hull variable
    for(std::size_t j : out) curr_ch.push_back(this->pl_points[j]);

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
    std::cout << "RED " << std::endl;
    for (auto it = seg.begin(); it != seg.end(); it++) std::cout << it->source() << " " << it->target() << std::endl;
    return seg;
}

// for every red edge, checks if it is visible from point i
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
            if (red.source() == it->source() ) {

                while(red.target() != it->target() && it != this->poly_line.end()) {
                    // create edges connecting the start and end point 
                    // of the possible visible edge with the point
                    Segment red1(p, it->source()), red2(p, it->target());

                    // for each polyline edge check if it intersects with the edges created
                    // if at least one polyline intersects the possible visible edge is not visible
                    if (is_vis(red1, red2)) seg.push_back(Segment(it->source(), it->target()));
                    std::cout << "VISIBLE " << std::endl;
                    for (auto i = seg.begin(); i != seg.end(); i++) std::cout << i->source() << " " << i->target() << std::endl;

                    it++;
                    // stop when the end of a polyline edge is the same as of the red edge    
                }

                Segment red1(p, it->source()), red2(p, it->target());
                if (is_vis(red1, red2)) seg.push_back(*it);
                // found all visible edges for this red edge
                break;
            }
        }
    }
    if (seg.size() == 0){
        int flag = 1;
        for (Segment red : red_edges) {

            for (Segment line : this->poly_line) {
                if (red.source() == line.target() && red.target() == line.source() ) {
                    flag = 0;
                    std::cout << "IM HERE " << std::endl;
                    seg.push_back(line);
                    break;
                }
            }

            // if red edge is part of the polyline there no more visible edges
            if(flag == 0) continue;
            for (auto it = this->poly_line.begin(); it != this->poly_line.end(); ++it) {

                if (red.target() == it->source()) {
                    while(red.source() != it->target()) {
                        // create edges connecting the start and end point 
                        // of the possible visible edge with the point
                        Segment red1(p, it->source()), red2(p, it->target());

                        // for each polyline edge check if it intersects with the edges created
                        // if at least one polyline intersects the possible visible edge is not visible
                        if (is_vis(red1, red2)) seg.push_back(*it);
                        it++;
                        // stop when the end of a polyline edge is the same as of the red edge    
                    }

                    Segment red1(p, it->source()), red2(p, it->target());
                    if (is_vis(red1, red2)) seg.push_back(*it);
                    // found all visible edges for this red edge
                    break;

                }
            }
        }   
    }
    std::cout << "VISIBLE " << std::endl;
    for (auto it = seg.begin(); it != seg.end(); it++) std::cout << it->source() << " " << it->target() << std::endl;

    return seg;
}

bool polyline::is_vis(Segment red1, Segment red2) const {
        for(Segment line : this->poly_line) {
            if ((line.source() == red1.target()) && (line.target() == red2.target())) continue;
            if (!intersection(red1, line) && !intersection(red2, line)) continue;
            if (intersection(red1, line) && intersection(red2, line)) return false;
            CGAL::Object result1 = intersection(red1, line);
            CGAL::Object result2 = intersection(red2, line);
            Point ipoint;
            Point ipoint1;
            Segment iseg;

            // check if the only point that intersects is the common point of the egdes
            if (CGAL::assign(ipoint, result1) && !intersection(red2, line) && ipoint != red1.target())  return false;          
            if (CGAL::assign(ipoint, result2) && !intersection(red1, line) && ipoint != red2.target()) return false;
            if (CGAL::assign(iseg, result1) || CGAL::assign(iseg, result2)) return false;
        }
        return true;
}

Segment polyline::min_area(std::vector<Segment> vis_edges, int i) const {
    Segment repleceable;
    double min_area = std::numeric_limits<double>::max();
    for(auto it = vis_edges.begin(); it != vis_edges.end(); ++it) {
        double curr_area = std::abs(CGAL::area(it->source(), this->points[i], it->target()));
        if (curr_area < min_area) {
            min_area = curr_area;
            repleceable = *it;
        }
    }
    return repleceable;
}

Segment polyline::max_area(std::vector<Segment> vis_edges, int i) const {
    Segment repleceable;
    double max_area = std::numeric_limits<double>::min();
    for(auto it = vis_edges.begin(); it != vis_edges.end(); ++it) {
        double curr_area = std::abs(CGAL::area(it->source(), this->points[i], it->target()));
        if (curr_area > max_area) {
            max_area = curr_area;
            repleceable = *it;
        }
    }
    return repleceable;
}

void polyline::insert_point(Segment repleceable_edge, int i) {
    // remove point to add it to the right place
    this->pl_points.pop_back();
    std::cout <<  "REPLACEABLE " << repleceable_edge.source() << " " << repleceable_edge.target() << std::endl;
    //insert segment
    auto index = std::find(poly_line.begin(), poly_line.end(), repleceable_edge);
    this->poly_line.insert(index, Segment(repleceable_edge.source(), this->points[i]));
    index = std::find(poly_line.begin(), poly_line.end(), repleceable_edge);
    std::cout <<  "FOUND " << index->source() << " " << index->target() << std::endl;
    this->poly_line.insert(index, Segment(this->points[i], repleceable_edge.target()));
    index = std::find(poly_line.begin(), poly_line.end(), repleceable_edge);
    this->poly_line.erase(index);

    //insert point
    auto pindex = std::find(pl_points.begin(), pl_points.end(), repleceable_edge.target());
    if (pindex == this->pl_points.begin()) this->pl_points.push_back(this->points[i]);
    else this->pl_points.insert(pindex, this->points[i]);
}

void polyline::write_to_file(std::string alg, int time) const {
    try {
        std::ofstream file(this->out_file);
        file << "Polygonization" << std::endl;
        for (Point p : this->points) file << p.x() << " " << p.y() << std::endl;
        for (Segment s : this->poly_line) file << s.source() << " " << s.target() << std::endl;
        file << "Algorithm: " << alg << "_" << this->edge_sel << std::endl;
        // TODO: ======= calculate polyline and convex_hull areas =======
        file << "Area: " << this->pl_area << std::endl;
        file << "Ratio: " << (this->pl_area / this->ch_area) << std::endl;
        file << "Construction time: " << time << " msec" << std::endl;
        file.close();
    } catch (...) {
        throw;
    }
    return;
}

polyline::polyline(std::vector<std::pair<float, float>> vec, std::string alg, std::string edge_sel, std::string init, std::string out_file): out_file(out_file), init(init) {
    //initialize points
    for(auto it = vec.begin(); it != vec.end(); ++it) this->points.push_back(Point(it->first, it->second));
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
