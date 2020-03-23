//
// Created by xiangweilai on 2020/3/22.
//

#ifndef GEOMETRY_STDINTERFACE_H
#define GEOMETRY_STDINTERFACE_H

enum ERROR_CODE {
    SUCCESS,
    WRONG_FORMAT,
    VALUE_OUT_OF_RANGE,
    INVALID_LINE, INVALID_CIRCLE,
    LINE_OVERLAP, CIRCLE_OVERLAP,
};

struct ERROR_INFO {
    ERROR_CODE code = SUCCESS;
    int lineNoStartedWithZero = -1;
    char messages[50] = "";
};


struct gShape {
    char type;
    int x1, y1, x2, y2;
};

struct gPoint {
    double x;
    double y;
};


struct gFigure {
    unsigned int figureId;
    gShape *shapes;  // only available after updateShapes()
    gPoint *points;  // only available after updatePoints()
    gPoint upperleft;
    gPoint lowerright;
};

gFigure *addFigure();

void deleteFigure(gFigure *fig);

void cleanFigure(gFigure *fig);

ERROR_INFO addShapeToFigure(gFigure *fig, gShape obj);

ERROR_INFO addShapeToFigureString(gFigure *fig, const char *desc);

ERROR_INFO addShapesToFigureFile(gFigure *fig, const char *filename);

ERROR_INFO addShapesToFigureStdin(gFigure *fig);

void removeShapeByIndex(gFigure *fig, unsigned int index);

int getPointsCount(const gFigure *fig);

int getShapesCount(const gFigure *fig);

void updatePoints(gFigure *fig);

void updateShapes(gFigure *fig);

#endif //GEOMETRY_STDINTERFACE_H
