#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QPainter>
#include <cmath>
#include <ctime>
#include <QRandomGenerator>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsLineItem>
int cellPosX = 400;
int cellPosY = 350;
float freq = 2.5;
float glass = 2.0 + 0.2 * freq;

class RectangleItem : public QGraphicsRectItem{
public:
    RectangleItem(qreal x, qreal y, qreal width, qreal heigth) : QGraphicsRectItem(x, y, width, heigth){
        setBrush((QBrush(QColor(0, 0, 0))));
    }
};

class TriangleItem : public QGraphicsPolygonItem {
public:
    TriangleItem(QPointF p1, QPointF p2, QPointF p3) : QGraphicsPolygonItem() {
        QPolygonF polygon;
        polygon << p1 << p2 << p3;
        setPolygon(polygon);
        setBrush(QBrush(QColor(0, 0, 0)));
    }
};

class CircleItem : public QGraphicsEllipseItem{
public:
    CircleItem(qreal x, qreal y, qreal width, qreal height) : QGraphicsEllipseItem(x, y, width, height){
        setBrush(QBrush(QColor(0, 0, 0)));
    }
};

class LineItem : public QGraphicsLineItem{
public:
    LineItem(qreal x1, qreal y1, qreal x2, qreal y2) : QGraphicsLineItem(x1, y1, x2, y2){
        setPen(QPen(QColor(0, 0, 0)));
    }
};

float PL(float f, float d){
    return 32.4 + 21 * log10(d) + 20 * log10(f);
}

float distanceToRectangle(int centerX, int centerY, RectangleItem* rectangle){
    qreal rectX = rectangle->x() + rectangle->rect().width() / 2;
    qreal rectY = rectangle->y() + rectangle->rect().height() / 2;

    int x0 = centerX;
    int y0 = centerY;
    int x1 = rectX;
    int y1 = rectY;

    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    int distance = 0;

    while (true) {
        if (x0 == x1 && y0 == y1) {
            break;
        }

        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }

        distance++;
    }

    return static_cast<float>(distance);
}

float distanceToCircle(int centerX, int centerY, CircleItem* circle) {
    qreal circleX = circle->x() + circle->rect().center().x();
    qreal circleY = circle->y() + circle->rect().center().y();

    int x0 = centerX;
    int y0 = centerY;
    int x1 = circleX;
    int y1 = circleY;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    float distance = sqrt(dx * dx + dy * dy);
    return distance;
}

float distanceToTriangle(int centerX, int centerY, TriangleItem* triangle) {
    QPolygonF polygon = triangle->polygon();
    QPointF p1 = polygon[0];
    QPointF p2 = polygon[1];
    QPointF p3 = polygon[2];

    float distanceAB = sqrt((p1.x() - centerX) * (p1.x() - centerX) + (p1.y() - centerY) * (p1.y() - centerY));
    float distanceBC = sqrt((p2.x() - centerX) * (p2.x() - centerX) + (p2.y() - centerY) * (p2.y() - centerY));
    float distanceCA = sqrt((p3.x() - centerX) * (p3.x() - centerX) + (p3.y() - centerY) * (p3.y() - centerY));

    float minDistance = std::min(std::min(distanceAB, distanceBC), distanceCA);

    return minDistance;
}

float distanceToLine(int centerX, int centerY, LineItem* line) {
    qreal x1 = line->line().x1();
    qreal y1 = line->line().y1();
    qreal x2 = line->line().x2();
    qreal y2 = line->line().y2();

    // Параметры уравнения прямой Ax + By + C = 0
    qreal A = y1 - y2;
    qreal B = x2 - x1;
    qreal C = x1 * y2 - x2 * y1;

    // Расстояние от центра до прямой
    float distance = fabs(A * centerX + B * centerY + C) / sqrt(A * A + B * B);

    return distance;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGraphicsScene* scene = new QGraphicsScene();
    int maxX = 1000;
    int maxY = 1000;
    int onePixDistance = 10;
    int TxPower = 23;
    int antGain = 12;

    //std::srand(std::time(nullptr));
    QPixmap map(maxX, maxY);
    QPainter p(&map);
    for(int i = 0; i < maxX; i++){
        for(int j = 0; j < maxY; j++){
            float distance = 0;
            distance = sqrt(pow(abs(cellPosX - i),2) + pow(abs(cellPosY - j),2)); // [pix]
            distance *= onePixDistance;
            float sigPower = TxPower + antGain - PL(freq, distance);
            if(sigPower >= -40){
                p.setPen(QColor(255, 0, 0, 255));
            } else if (sigPower < -40 && sigPower >= -45) {
                p.setPen(QColor(255, 100, 0, 255));
            } else if (sigPower < -45 && sigPower >= -55) {
                p.setPen(QColor(255, 150, 0, 255));
            } else if (sigPower < -55 && sigPower >= -60) {
                p.setPen(QColor(255, 200, 0, 255));
            } else if (sigPower < -60 && sigPower >= -65) {
                p.setPen(QColor(200, 255, 0, 255));
            } else if (sigPower < -65 && sigPower >= -70) {
                p.setPen(QColor(150, 255, 0, 255));
            } else if (sigPower < -70 && sigPower >= -75) {
                p.setPen(QColor(100, 255, 0, 255));
            } else if (sigPower < -75 && sigPower >= -80) {
                p.setPen(QColor(50, 255, 0, 255));
            } else if (sigPower < -80 && sigPower >= -85) {
                p.setPen(QColor(0, 255, 100, 255));
            } else if (sigPower < -85 && sigPower >= -90) {
                p.setPen(QColor(0, 255, 150, 255));
            } else if (sigPower < -90 && sigPower >= -95) {
                p.setPen(QColor(0, 255, 200, 255));
            } else if (sigPower < -95 && sigPower >= -100) {
                p.setPen(QColor(0, 200, 255, 255));
            } else if (sigPower < -100 && sigPower >= -105) {
                p.setPen(QColor(0, 150, 255, 255));
            } else if (sigPower < -105 && sigPower >= -110) {
                p.setPen(QColor(139, 24, 255, 255));
            } else if (sigPower < -115 && sigPower >= -120) {
                p.setPen(QColor(0, 50, 255, 255));
            } else if (sigPower < -120 && sigPower >= -125) {
                p.setPen(QColor(116, 24, 255, 255));
            }
            p.drawPoint(i, j);
        }
    }
    p.end();
    RectangleItem* rectangle = new RectangleItem(456, 456, 50, 50);
    float distToRect = distanceToRectangle(cellPosX, cellPosY, rectangle);
    qDebug() << "Distance to Rectingle: " << distToRect;
    TriangleItem* triangle = new TriangleItem(QPointF(300, 300), QPointF(350, 300), QPointF(325, 250));
    float distToTriangle = distanceToTriangle(cellPosX, cellPosY, triangle);
    qDebug() << "Distance to Triangle: " << distToTriangle;
    CircleItem* circle = new CircleItem(625, 225, 50, 50);
    float distToCircle = distanceToCircle(cellPosX, cellPosY, circle);
    qDebug() << "Distance to Circle" << distToCircle;
    LineItem* line = new LineItem(125, 425, 300, 600);
    float distToLine = distanceToLine(cellPosX, cellPosY, line);
    qDebug() << "Distance to Line: " << distToLine;
    scene->addPixmap(map);
    scene->addItem(rectangle);
    scene->addItem(circle);
    scene->addItem(triangle);
    scene->addItem(line);
    QGraphicsView* view = new QGraphicsView(scene);
    setCentralWidget(view);
}

MainWindow::~MainWindow()
{
    delete ui;
}
