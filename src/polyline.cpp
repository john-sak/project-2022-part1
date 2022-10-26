#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iostream>

#include <polyline.hpp>

void polyline::incremental(int init) {
    try {
        // sort this->points
        this->sort_points(init);

        // initialize polygon
        int i = this->init_polygon();

        // expand polygn
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
            // x descending
            std::sort(this->points.begin(), this->points.end(), [] (const std::pair<float,float> &a, const std::pair<float,float> &b) {
                return (a.first > b.first);
            });
            break;
        case 2:
            // x ascending
            std::sort(this->points.begin(), this->points.end());
            break;
        case 3:
            // y descending
            std::sort(this->points.begin(), this->points.end(), [] (const std::pair<float,float> &a, const std::pair<float,float> &b) {
                return (a.second > b.second);
            });
            break;
        case 4:
            // y ascending
            std::sort(this->points.begin(), this->points.end(), [] (const std::pair<float,float> &a, const std::pair<float,float> &b) {
                return (a.second < b.second);
            });
            break;
        default:
            throw "Error: Couldn't sort vector";
    }
}

int polyline::init_polygon(void) {
    try {
        Point p1(this->points[0].first, this->points[0].second);
        Point p2(this->points[1].first, this->points[1].second);
        Point p3(this->points[2].first, this->points[2].second);
        this->PL.push_back(p1);
        this->PL.push_back(p2);
        this->PL.push_back(p3);
        this->CH.push_back(p1);
        this->CH.push_back(p2);
        this->CH.push_back(p3);
        if (!CGAL::collinear(p1, p2, p3)) return 3;
        int i = 3;
        int flag = 1;
        do {
            Point p(this->points[i].first, this->points[i].second);
            this->PL.push_back(p);
            this->CH.push_back(p);
            i++;
            if (!CGAL::collinear(p1, p2, p)) flag = 0;
        } while (flag);
        return i;
    } catch (...) {
        throw;
    }
}

void polyline::expand(int i) {
    try {
        while (i < this->points.size()) {
            Point p(this->points[i].first, this->points[i].second);
            i++;
            // find red lines of CH
            // for every red find visible
            // choose visible (edge_sel)
        }
        return;
    } catch (...) {
        throw;
    }
}

void polyline::write_to_file(void) const {
    // todo
    return;
}

polyline::polyline(std::vector<std::pair<float, float>> vec, std::string alg, std::string edge_sel, std::string init, std::string out_file): points(vec), PL(), CH(), out_file(out_file) {
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
    for(std::pair<float,float> i : this->points) std::cout << i.first << " " << i.second << std::endl;
    return;
}
