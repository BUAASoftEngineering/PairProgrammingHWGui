#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <cmath>
#include "interface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *);

private:
    gManager * gmgr = createManager();
    QMap<QString, gShape> gShapes;

    int nextGraphId = 0;
    double range = 5.0;
    double centerX = 0.;
    double centerY = 0.;
    double mouseOriX = 0.;
    double mouseOriY = 0.;

    int getAndIncrementNextGraphId() {
       return nextGraphId ++;
    }

    QString drawCircle(int x, int y, int r);
    QString drawHalfLine(int x1, int y1, int x2, int y2);
    QString drawSegmentLine(int x1, int y1, int x2, int y2);
    QString drawLine(int x1, int y1, int x2, int y2);
    QString drawShape(char type, int x1, int y1, int x2, int y2);
    void drawPoint(double x, double y);
    void replotPoints();

private slots:
    void on_actionOpen_triggered();

    void on_shapeTypeComboBox_currentIndexChanged(int);

    void on_addShapeButton_clicked();

    void on_plot_mouseWheel(QWheelEvent*);

    void on_plot_mouseMove(QMouseEvent*);

    void on_plot_mousePress(QMouseEvent*);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
