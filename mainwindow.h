#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <cmath>

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
    int nextGraphId = 0;

    int getAndIncrementNextGraphId() {
       return nextGraphId ++;
    }

    void drawPlotWithXY(const QVector<double> &x, const QVector<double> &y);

    void drawCircle(int x, int y, int r);
    void drawHalfLine(int x1, int y1, int x2, int y2);
    void drawSegmentLine(int x1, int y1, int x2, int y2);
    void drawLine(int x1, int y1, int x2, int y2);

private slots:
    void on_actionOpen_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
