#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qcpitemhalfline.h"
#include "QFileDialog"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    auto customPlot = ui->plot;
    customPlot->setInteraction(QCP::iSelectItems, true);
    customPlot->setInteraction(QCP::iMultiSelect, true);


    drawCircle(0,1,2);
    drawLine(0,1,1,2);
    drawHalfLine(0,0,1,1);
    drawHalfLine(1,1,2,0);
    drawHalfLine(2,2,1,-2);
    drawHalfLine(3,3,0,-1);
    drawHalfLine(2,2,-1,-2);
    drawHalfLine(1,1,-3,0);
    drawHalfLine(0,0,-3,7);
    drawHalfLine(1,2,0,1);
    drawSegmentLine(1,0,2,1);

    drawPoint(0,0);
    drawPoint(-1,-1.4);

    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:

    customPlot->xAxis->setRange(-5,5);
    customPlot->yAxis->setRange(-5,5);
    customPlot->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(gmgr) {
        qDebug() << "closing gManager:" << gmgr;
//        closeManager(gmgr);
    }

}

void MainWindow::keyPressEvent(QKeyEvent * event) {
    if(event->key() == Qt::Key_Delete){
        qDebug()<<"key delete pressed";
        auto items = ui->plot->selectedItems();
        qDebug()<<items;
        for(auto& item: items){
            auto name = item->objectName();
            qDebug()<<name;
            gShapes.remove(name);
            ui->plot->removeItem(item);
            cleanManager(gmgr);
            for(const auto &shape: gShapes.values()) {
                addShape(gmgr, shape.type, shape.x1, shape.y1, shape.x2, shape.y2, nullptr, nullptr);
            }
        }
        replotPoints();
        ui->plot->replot();
    }
}


void MainWindow::on_actionOpen_triggered()
{
    // slot handling file open
    auto fname = QFileDialog::getOpenFileName(this, "Select a file to open", ".", "*.txt;*.in");
    qDebug()<<fname;
    FILE * file;
    if (!fname.isEmpty()) {
        if(!fopen_s(&file, fname.toStdString().c_str(), "r")) {
            cleanManager(gmgr);
            gShapes.clear();
            ui->plot->clearItems();

            ERROR_INFO err = addShapesBatch(gmgr, file, nullptr, nullptr);
            int nShape = gmgr->shapes->size(); // TODO: fix here
            gShape *shapes = new gShape[nShape];
            getGeometricShapes(gmgr, shapes);
            for(int i = 0; i<nShape; ++i) {
                auto &shape = shapes[i];
                qDebug() << shape.type;
                QString id;
                switch(shape.type) {
                case 'L':
                    id = drawLine(shape.x1, shape.y1, shape.x2, shape.y2);
                    break;
                case 'R':
                    id = drawHalfLine(shape.x1, shape.y1, shape.x2, shape.y2);
                    break;
                case 'S':
                    id = drawSegmentLine(shape.x1, shape.y1, shape.x2, shape.y2);
                    break;
                case 'C':
                    id = drawCircle(shape.x1, shape.y1, shape.x2);
                    break;
                }
                gShapes.insert(id, shape);
            }
            delete [] shapes;

            replotPoints();
            ui->plot->replot();
            fclose(file);
        }
    }
}

QString MainWindow::drawCircle(int x, int y, int r) {
    auto circle = new QCPItemEllipse(ui->plot);
    QPoint topLeft(x-r, y+r);
    QPoint bottomRight(x+r, y-r);
    circle->topLeft->setCoords(topLeft);
    circle->bottomRight->setCoords(bottomRight);
    QString id = QString::number(getAndIncrementNextGraphId());
    circle->setObjectName(id);
    return id;
}

QString MainWindow::drawLine(int x1, int y1, int x2, int y2) {
    auto line = new QCPItemStraightLine(ui->plot);
    QPoint start(x1, y1);
    QPoint end(x2, y2);
    line->point1->setCoords(start);
    line->point2->setCoords(end);
    QString id = QString::number(getAndIncrementNextGraphId());
    line->setObjectName(id);
    return id;
}

QString MainWindow::drawHalfLine(int x1, int y1, int x2, int y2) {
    auto line = new QCPItemHalfLine(ui->plot);
    QPoint start(x1, y1);
    QPoint end(x2, y2);
    line->point1->setCoords(start);
    line->point2->setCoords(end);
    QString id = QString::number(getAndIncrementNextGraphId());
    line->setObjectName(id);
    return id;
}

QString MainWindow::drawSegmentLine(int x1, int y1, int x2, int y2) {
    auto line = new QCPItemLine(ui->plot);
    QPoint start(x1, y1);
    QPoint end(x2, y2);
    line->start->setCoords(start);
    line->end->setCoords(end);
    QString id = QString::number(getAndIncrementNextGraphId());
    line->setObjectName(id);
    return id;
}

void MainWindow::replotPoints() {
    ui->plot->clearGraphs();
    int nPoint = getIntersectionsCount(gmgr);
    gPoint *points = new gPoint[nPoint];
    getIntersections(gmgr, points);
    for(int i = 0; i<nPoint; ++i){
        auto& point = points[i];
        drawPoint(point.x, point.y);
    }
    delete [] points;
}

void MainWindow::drawPoint(double x, double y) {
    QPen pen;
    pen.setWidth(5);
    auto graph = ui->plot->addGraph();
    graph->setData({x}, {y});
    graph->setPen(pen);
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDot, 2));
}
