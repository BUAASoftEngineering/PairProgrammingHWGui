#ifndef QCPITEMHALFLINE_H
#define QCPITEMHALFLINE_H

#include <qcustomplot.h>

class QCPItemHalfLine : public QCPItemStraightLine
{
public:
    explicit QCPItemHalfLine(QCustomPlot *parentPlot):QCPItemStraightLine(parentPlot){}
    void draw(QCPPainter *painter);
};

#endif // QCPITEMHALFLINE_H
