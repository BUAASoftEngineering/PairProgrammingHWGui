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

    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:

    customPlot->xAxis->setRange(-range,range);
    customPlot->yAxis->setRange(-range,range);
    customPlot->replot();

    ui->shapeListView->setModel(&shapeListModel);

    connect(ui->plot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(on_plot_mouseWheel(QWheelEvent*)));
    connect(ui->plot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(on_plot_mouseMove(QMouseEvent*)));
    connect(ui->plot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(on_plot_mousePress(QMouseEvent*)));
    // QT connects the below signal-slot pairs automaticly
    // so DO NOT repeat it
    // connect(ui->addShapeButton, &QPushButton::clicked, this, &MainWindow::on_addShapeButton_clicked);
    // connect(ui->shapeTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_shapeTypeComboBox_currentIndexChanged(int)));
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
            ui->plot->removeItem(item);
            auto shape = shapes.find(name);
            if(shape!=shapes.end()){
                shapeListModel.removeRow(shape->index.row());
            }
            shapes.remove(name);
        }
        cleanManager(gmgr);
        for(const auto &shapeItem: shapes.values()) {
            auto & shape = shapeItem.gshape;
            addShape(gmgr, shape.type, shape.x1, shape.y1, shape.x2, shape.y2, nullptr, nullptr);
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
            ui->plot->clearItems();
            shapes.clear();
            shapeListModel.removeRows(0,shapeListModel.rowCount());

            ERROR_INFO err = addShapesBatch(gmgr, file, nullptr, nullptr);
            int nShape = gmgr->shapes->size(); // TODO: fix here
            gShape *shapes = new gShape[nShape];
            getGeometricShapes(gmgr, shapes);
            for(int i = 0; i<nShape; ++i) {
                auto &shape = shapes[i];
                qDebug() << shape.type;
                QString id = plotShape(shape.type, shape.x1, shape.y1, shape.x2, shape.y2);
            }
            delete [] shapes;

            replotPoints();
            ui->plot->replot();
            fclose(file);
        }
    }
}

QCPAbstractItem * MainWindow::drawCircle(const QString &id, int x, int y, int r) {
    auto circle = new QCPItemEllipse(ui->plot);
    QPoint topLeft(x-r, y+r);
    QPoint bottomRight(x+r, y-r);
    QPen pen;
    pen.setColor(QColor(0xff0000));
    pen.setWidth(2);
    circle->setPen(pen);
    circle->topLeft->setCoords(topLeft);
    circle->bottomRight->setCoords(bottomRight);
    circle->setObjectName(id);
    return circle;
}

QCPAbstractItem * MainWindow::drawLine(const QString &id, int x1, int y1, int x2, int y2) {
    auto line = new QCPItemStraightLine(ui->plot);
    QPoint start(x1, y1);
    QPoint end(x2, y2);
    QPen pen;
    pen.setColor(QColor(0x00ff00));
    pen.setWidth(2);
    line->setPen(pen);
    line->point1->setCoords(start);
    line->point2->setCoords(end);
    line->setObjectName(id);
    return line;
}

QString MainWindow::plotShape(char type, int x1, int y1, int x2, int y2)
{
    QString id = QString::number(getAndIncrementNextGraphId());
    QCPAbstractItem * item;
    switch(type) {
    case 'L':
        item = drawLine(id, x1, y1, x2, y2);
        break;
    case 'R':
        item = drawHalfLine(id, x1, y1, x2, y2);
        break;
    case 'S':
        item = drawSegmentLine(id, x1, y1, x2, y2);
        break;
    case 'C':
        item = drawCircle(id, x1, y1, x2);
        break;
    }

    QString visName = id+"_"+type+"_"+QString::number(x1)+"_"+QString::number(y1)+"_"+QString::number(x2);
    if(type!='C'){
        visName += "_" + QString::number(y2);
    }
    shapeListModel.insertRow(shapeListModel.rowCount());
    auto lastRowind=shapeListModel.index(shapeListModel.rowCount()-1,0);
    shapeListModel.setData(lastRowind,visName,Qt::DisplayRole);

    shapes.insert(id, {{type, x1, y2, x2, y2}, item, lastRowind});
    return id;
}

QCPAbstractItem * MainWindow::drawHalfLine(const QString &id, int x1, int y1, int x2, int y2) {
    auto line = new QCPItemHalfLine(ui->plot);
    QPoint start(x1, y1);
    QPoint end(x2, y2);
    QPen pen;
    pen.setColor(QColor(0x003333));
    pen.setWidth(2);
    line->setPen(pen);
    line->point1->setCoords(start);
    line->point2->setCoords(end);
    line->setObjectName(id);
    return line;
}

QCPAbstractItem * MainWindow::drawSegmentLine(const QString &id, int x1, int y1, int x2, int y2) {
    auto line = new QCPItemLine(ui->plot);
    QPoint start(x1, y1);
    QPoint end(x2, y2);
    QPen pen;
    pen.setColor(QColor(0x666666));
    pen.setWidth(2);
    line->setPen(pen);
    line->start->setCoords(start);
    line->end->setCoords(end);
    line->setObjectName(id);
    return line;
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
    pen.setWidth(6);
    auto graph = ui->plot->addGraph();
    graph->setData({x}, {y});
    graph->setPen(pen);
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDot, 2));
}

void MainWindow::on_shapeTypeComboBox_currentIndexChanged(int index)
{
//    int index = ui->shapeTypeComboBox->currentIndex();
    if (index == 3) {
        // index 3: circle
        ui->spinBoxY2->setEnabled(false);
    } else {
        ui->spinBoxY2->setEnabled(true);
    }
}

void MainWindow::on_addShapeButton_clicked()
{
    qDebug() << "clicked";
    int shapeTypeInd = ui->shapeTypeComboBox->currentIndex();
    char shapeType = "LRSC"[shapeTypeInd];
    int x1 = ui->spinBoxX1->value();
    int y1 = ui->spinBoxY1->value();
    int x2 = ui->spinBoxX2->value();
    int y2 = ui->spinBoxY2->value();
    int nShape = gmgr->shapes->size(); // TODO upd it
    gPoint * pointBuffer = new gPoint[nShape * 2];
    int posBuf = 0;
    auto err = addShape(gmgr, shapeType, x1, y1, x2, y2, pointBuffer, &posBuf);
    if(err == ERROR_CODE::SUCCESS){
        auto id = plotShape(shapeType, x1, y1, x2, y2);
        for(int i =0; i<posBuf; ++i){
            drawPoint(pointBuffer[i].x, pointBuffer[i].y);
        }
        delete [] pointBuffer;
        ui->plot->replot();

    } else {
        // TODO
    }
}

void MainWindow::on_plot_mouseWheel(QWheelEvent* event)
{
    double delta = (double)event->delta();
    double factor = delta>0 ? 3.3 : (3./1.1);
    range = range * (factor * abs(delta) / 360);
    qDebug()<<range;
    ui->plot->xAxis->setRange(-range,range);
    ui->plot->yAxis->setRange(-range,range);
    ui->plot->replot();
}

void MainWindow::on_plot_mousePress(QMouseEvent* event) {
    mouseOriX = ui->plot->xAxis->pixelToCoord(event->x());
    mouseOriY = ui->plot->xAxis->pixelToCoord(event->y());
    qDebug()<<"mouse down"<<mouseOriX<<mouseOriY;
}

void MainWindow::on_plot_mouseMove(QMouseEvent* event) {
    if(event->buttons() & Qt::LeftButton) {
        // TODO: fix it
//        double xCoord = ui->plot->xAxis->pixelToCoord(event->x());
//        double yCoord = ui->plot->yAxis->pixelToCoord(event->y());
//        double deltaX = xCoord - mouseOriX;
//        double deltaY = yCoord - mouseOriY;
//        centerX-=deltaX;
//        centerY-=deltaY;

//        qDebug()<<mouseOriX<<mouseOriY<<centerX<<centerY;
//        ui->plot->xAxis->setRange(centerX-range, centerX+range);
//        ui->plot->yAxis->setRange(centerY-range, centerY+range);
//        mouseOriX = ui->plot->xAxis->pixelToCoord(event->x());;
//        mouseOriY = ui->plot->xAxis->pixelToCoord(event->y());;
//        ui->plot->replot();
    }
}
