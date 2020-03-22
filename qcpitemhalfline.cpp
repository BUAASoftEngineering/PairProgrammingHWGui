#include "qcpitemhalfline.h"
#include <algorithm>

void QCPItemHalfLine::draw(QCPPainter *painter)
{
  QCPVector2D start(point1->pixelPosition());
  QCPVector2D end(point2->pixelPosition());
  // get visible segment of straight line inside clipRect:
  double clipPad = mainPen().widthF();
  auto rect = clipRect().adjusted(-clipPad, -clipPad, clipPad, clipPad);

  int rx1,ry1,rx2,ry2;
  rect.getCoords(&rx1, &ry1, &rx2, &ry2);

  double sx = start.x();
  double sy = start.y();
  double ex = end.x();
  double ey = end.y();
  if(sx>ex){
      rx2 = sx < rx2 ? sx : rx2;
  } else {
      rx1 = sx > rx1 ? sx : rx1;
  }
  rx1 = rx1 > rx2 ? rx2 : rx1;
  if(sy>ey){
      ry2 = sy < ry2 ? sy : ry2;
  } else {
      ry1 = sy > ry1 ? sy : ry1;
  }
  ry1 = ry1 > ry2 ? ry2 : ry1;

  rect.setCoords(rx1, ry1, rx2, ry2);

  QLineF line = getRectClippedStraightLine(start, end-start, rect);
  // paint visible segment, if existent:
  if (!line.isNull())
  {
    painter->setPen(mainPen());
    painter->drawLine(line);
  }
}
