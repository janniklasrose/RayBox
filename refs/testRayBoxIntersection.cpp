// testRayBoxIntersection.cpp

#include <cassert>
#include <stdexcept>

using FT = float;

struct Coord {
    FT value;

    Coord() : value(0) {}
    Coord(FT value_) : value(value_) {}

    FT operator()() const { return value; }
    operator FT() const { return value; }
};

struct Point {
    Coord x;
    Coord y;
    Coord z;

    Point() : x(0), y(0), z(0) {}
    Point(FT x_, FT y_, FT z_) : x(x_), y(y_), z(z_) {}

    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y, z + other.z);
    }

    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y, z - other.z);
    }

    Point operator*(FT scale) const {
        return Point(x * scale, y * scale, z * scale);
    }
};

struct Bbox {
    Point min;
    Point max;

    Bbox(Point min_, Point max_) : min(min_), max(max_) {}

    FT xmin() const { return min.x; }
    FT ymin() const { return min.y; }
    FT zmin() const { return min.z; }
    FT xmax() const { return max.x; }
    FT ymax() const { return max.y; }
    FT zmax() const { return max.z; }
};

struct Segment {
    Point first;
    Point second;

    Segment(Point first_, Point second_) : first(first_), second(second_) {}

    Point source() const { return first; }
    Point target() const { return second; }
};

#include "RayBoxIntersection.hpp"

bool same_point(Point lhs, Point rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool segment_hits_box_face() {
    RayBoxIntersection intersection;
    Bbox box(Point(1, 1, 1), Point(2, 2, 2));
    Segment segment(Point(0, 1.5, 1.5), Point(3, 1.5, 1.5));
    Point hit;

    return intersection.CheckSegmentBox(box, segment, hit)
        && same_point(hit, Point(1, 1.5, 1.5));
}

bool segment_outside_box_misses() {
    RayBoxIntersection intersection;
    Bbox box(Point(1, 1, 1), Point(2, 2, 2));
    Segment segment(Point(0, 3, 1.5), Point(3, 3, 1.5));
    Point hit;

    return !intersection.CheckSegmentBox(box, segment, hit);
}

bool source_inside_box_is_rejected() {
    RayBoxIntersection intersection;
    Bbox box(Point(1, 1, 1), Point(2, 2, 2));
    Segment segment(Point(1.5, 1.5, 1.5), Point(3, 1.5, 1.5));
    Point hit;

    return !intersection.CheckSegmentBox(box, segment, hit);
}

bool box_contains_boundary_point() {
    RayBoxIntersection intersection;
    Bbox box(Point(1, 1, 1), Point(2, 2, 2));

    return intersection.box_contains_point(box, Point(1, 1.5, 2));
}

bool boundary_intersection_throws() {
    RayBoxIntersection intersection;
    Point hit;

    try {
        intersection.GetIntersection(0, 1, Point(1, 1, 1), Point(2, 2, 2), hit);
    } catch (const std::runtime_error&) {
        return true;
    }

    return false;
}

int main() {
    assert(segment_hits_box_face());
    assert(segment_outside_box_misses());
    assert(source_inside_box_is_rejected());
    assert(box_contains_boundary_point());
    assert(boundary_intersection_throws());
    return 0;
}
