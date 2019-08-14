// raybox.hpp

/*
Amy Williams, Steve Barrus, R. Keith Morley, Peter Andshirley.
An efficient and robust ray-box intersection algorithm.
2005. Journal of Graphics Tools 10, 54.
DOI:10.1145/1198555.1198748
*/

#pragma once // include guard

#include <cassert>

class Vector3 {
    private:
        float xyz[3] = {0,0,0};
    public:
        Vector3() = default;
        Vector3 (const float x, const float y, const float z) {
            xyz[0] = x;
            xyz[1] = y;
            xyz[2] = z;
        }
        float x() const { return xyz[0]; };
        float y() const { return xyz[1]; };
        float z() const { return xyz[2]; };

        bool operator< (const Vector3& other) const {
            bool out = true;
            for (int i = 0; i < 3; i++)
                out = out && (xyz[i] < other.xyz[i]);
            return out;
        }
};

class Ray {
    public:
        Ray (const Vector3 &o, const Vector3 &d) {
            origin = o;
            direction = d;
            inv_direction = Vector3(1/d.x(), 1/d.y(), 1/d.z());
            sign[0] = (inv_direction.x() < 0);
            sign[1] = (inv_direction.y() < 0);
            sign[2] = (inv_direction.z() < 0);
        }
        Vector3 origin;
        Vector3 direction;
        Vector3 inv_direction;
        int sign[3];
};

class Box {
    public:
        Box (const Vector3 &min, const Vector3 &max) {
            assert(min < max);
            bounds[0] = min;
            bounds[1] = max;
        }
        bool intersect (const Ray &r, float t0, float t1) const;
        Vector3 bounds[2];
};

bool Box::intersect (const Ray &r, float t0, float t1) const {
    float txmin, txmax, tymin, tymax, tzmin, tzmax; // ray intersections

    // x
    txmin = (bounds[  r.sign[0]].x() - r.origin.x()) * r.inv_direction.x();
    txmax = (bounds[1-r.sign[0]].x() - r.origin.x()) * r.inv_direction.x();
    float tmin = txmin;
    float tmax = txmax;

    // y
    tymin = (bounds[  r.sign[1]].y() - r.origin.y()) * r.inv_direction.y();
    tymax = (bounds[1-r.sign[1]].y() - r.origin.y()) * r.inv_direction.y();
    if ( (tmin > tymax) || (tymin > tmax) )
        return false;
    if (tymin > tmin) {
        tmin = tymin;
    }
    if (tymax < tmax) {
        tmax = tymax;
    }

    // z
    tzmin = (bounds[  r.sign[2]].z() - r.origin.z()) * r.inv_direction.z();
    tzmax = (bounds[1-r.sign[2]].z() - r.origin.z()) * r.inv_direction.z();
    if ( (tmin > tzmax) || (tzmin > tmax) )
        return false;
    if (tzmin > tmin) {
        tmin = tzmin;
    }
    if (tzmax < tmax) {
        tmax = tzmax;
    }

    // interval
    return ( (tmin < t1) && (tmax > t0) );
}

