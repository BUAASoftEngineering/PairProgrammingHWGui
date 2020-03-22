//
// Created by xiangweilai on 2020/3/7.
//

#ifndef GEOMETRY_POINT_H
#define GEOMETRY_POINT_H

#include <cmath>
#include <ostream>
#include <iomanip>
#include <sstream>

typedef long long ll;
const int PRECISION = 6;
const int ROUND_PRECISION = 1e5;
const double BASE_EPS = 1e-15;
const double DYNAMIC_EPS_START = 1e5;

class Coordinate {
public:
    double value;

    explicit Coordinate(double v_double);

    Coordinate() = default;

    bool operator<(const Coordinate &other) const;

    bool operator==(const Coordinate &other) const;

    bool operator>(const Coordinate &other) const;

    friend std::ostream &operator<<(std::ostream &os, const Coordinate &coordinate);

    std::size_t hashCode() const {
        // BUGFIX: round()! otherwise 2.999999999999911 -> 29999! (should be 30000)
        // ROUND_PRECISION set to 1e5 for safety (collision is Okay, but 1->2 is a tragedy.)
        return std::hash<ll>{}((ll) std::round(value * ROUND_PRECISION));
    }
};


class Point {
public:
    Coordinate x, y;

    Point(Coordinate xx, Coordinate yy);

    Point() = default;

    friend std::ostream &operator<<(std::ostream &os, const Point &point);
};

extern int GLOBAL_COLLISION_COUNT;
extern int GLOBAL_HASH_COUNT;

// Hash Function For Point objects in support of unordered_map<Point>
struct hashCode_Point {
    std::size_t operator()(const Point &point) const {
        GLOBAL_HASH_COUNT++;
        return point.x.hashCode() ^ (point.y.hashCode() << 1u);
    }
};

struct equals_Point {
    bool operator()(const Point &lhs, const Point &rhs) const {
        GLOBAL_COLLISION_COUNT++;
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
};

struct gPoint {
    double x;
    double y;
};

#endif //GEOMETRY_POINT_H
