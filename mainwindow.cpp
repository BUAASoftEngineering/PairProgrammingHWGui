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
}

void MainWindow::keyPressEvent(QKeyEvent * event) {
    if(event->key() == Qt::Key_Delete){
        qDebug()<<"key delete pressed";
        auto items = ui->plot->selectedItems();
        qDebug()<<items;
        for(auto& item: items){
            qDebug()<<item->objectName();
            ui->plot->removeItem(item);
        }
        ui->plot->replot();
    }
}


void MainWindow::on_actionOpen_triggered()
{
    auto fname = QFileDialog::getOpenFileName(this, "Select a file to open", ".", "*.txt;*.in");
    qDebug()<<fname;

}

void MainWindow::drawPlotWithXY(const QVector<double> &x, const QVector<double> &y){
    auto *graph = new QCPCurve(ui->plot->xAxis, ui->plot->yAxis);
    graph->setData(x, y);
    int id = getAndIncrementNextGraphId();
    graph->setObjectName(QString::number(id));
}

void MainWindow::drawCircle(int x, int y, int r) {
    auto circle = new QCPItemEllipse(ui->plot);
    QPoint topLeft(x-r, y+r);
    QPoint bottomRight(x+r, y-r);
    circle->topLeft->setCoords(topLeft);
    circle->bottomRight->setCoords(bottomRight);
}

void MainWindow::drawLine(int x1, int y1, int x2, int y2) {
    auto line = new QCPItemStraightLine(ui->plot);
    QPoint start(x1, y1);
    QPoint end(x2, y2);
    line->point1->setCoords(start);
    line->point2->setCoords(end);
}

void MainWindow::drawHalfLine(int x1, int y1, int x2, int y2) {
    auto line = new QCPItemHalfLine(ui->plot);
    QPoint start(x1, y1);
    QPoint end(x2, y2);
    line->point1->setCoords(start);
    line->point2->setCoords(end);
}

void MainWindow::drawSegmentLine(int x1, int y1, int x2, int y2) {
    auto line = new QCPItemLine(ui->plot);
    QPoint start(x1, y1);
    QPoint end(x2, y2);
    line->start->setCoords(start);
    line->end->setCoords(end);
}
