//
// Created by xiangweilai on 2020/3/9.
//

#ifndef GEOMETRY_SHAPES_H
#define GEOMETRY_SHAPES_H

#include <variant>
#include <vector>
#include <iostream>
#include <tuple>
#include "Point.h"

enum LineType {
    LINE,
    HALF_LINE,
    SEGMENT_LINE,
};

class Line {
public:
    Line() : Line(0, 0, 0, 0) {}

    Line(int x1, int y1, int x2, int y2);

    Line(int x1, int y1, int x2, int y2, LineType type);

    int p1_x, p1_y;
    int p2_x, p2_y;
    LineType type;

    virtual bool checkPoint(const Coordinate &x, const Coordinate &y) const;
};

bool checkPointHalf(const Coordinate &x, const Coordinate &y,
                    const Line &line);

bool checkPointSegment(const Coordinate &x, const Coordinate &y,
                       const Line &line);


class Circle {
public:
    Circle(int x, int y, int r);

    int center_x, center_y;
    int radius;
};

typedef std::tuple<bool, int, Point, Point> point_container_t;

point_container_t intersection(const Line &a, const Circle &b);

point_container_t intersection(const Circle &a, const Line &b);

point_container_t intersection(const Line &a, const Line &b);

point_container_t intersection(const Circle &a, const Circle &b);

ll square(ll x);

// variant for auto combination
using Geometry = std::variant<Line, Circle>;


struct interset_visitor {
    template<typename Shape1, typename Shape2>
    point_container_t operator()(const Shape1 &lhs, const Shape2 &rhs) const {
        return intersection(lhs, rhs);
    }
};


// Line/Circle ---> gShape for C-style users
struct gShape {
    char type;
    int x1, y1, x2, y2;
};

gShape gShapeConverter(const Line &s);

gShape gShapeConverter(const Circle &s);

struct gShape_visitor {
    template<typename Shape1>
    gShape operator()(const Shape1 &lhs) const {
        return gShapeConverter(lhs);
    }
};

#endif //GEOMETRY_SHAPES_H
