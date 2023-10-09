#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QPainter>
#include <cmath>
#include <ctime>

float PL(float f, float d){
    return 32.4 + 21 * log10(d) + 20 * log10(f);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGraphicsScene* scene = new QGraphicsScene();
    int maxX = 1000;
    int maxY = 1000;
    int onePixDistance = 100;
    int TxPower = 23;
    int antGain = 12;
    float freq = 2.5;

    //std::srand(std::time(nullptr));
    int cellPosX = 400;
    int cellPosY = 350;
    QPixmap map(maxX, maxY);
    QPainter p(&map);
    for(int i = 0; i < maxX; i++){
        for(int j = 0; j < maxY; j++){
            float distance = 0;
            distance = sqrt(pow(abs(cellPosX - i),2) + pow(abs(cellPosY - j),2)); // [pix]
            distance *= onePixDistance;
            float sigPower = TxPower + antGain - PL(freq, distance);
            if (sigPower >= -40) {
                p.setPen(QColor(255, 0, 0, 255));
            } else if (sigPower >= -50) {
                p.setPen(QColor(220, 50, 0, 255));
            } else if (sigPower >= -60) {
                p.setPen(QColor(255, 160, 0, 255));
            } else if (sigPower >= -70) {
                p.setPen(QColor(255, 255, 0, 255));
            } else if (sigPower >= -80) {
                p.setPen(QColor(173, 255, 47, 255));
            } else if (sigPower >= -90) {
                p.setPen(QColor(0, 255, 0, 255));
            } else if (sigPower >= -100) {
                p.setPen(QColor(0, 255, 255, 255));
            } else if (sigPower >= -110) {
                p.setPen(QColor(173, 216, 230, 255));
            } else if (sigPower >= -120) {
                p.setPen(QColor(0, 0, 255));
            }else if (sigPower >= 134){
                p.setPen(QColor(0, 0, 255));
            }
            p.drawPoint(i, j);
        }
    }
    p.end();
    scene->addPixmap(map);
    QGraphicsView* view = new QGraphicsView(scene);
    setCentralWidget(view);
}

MainWindow::~MainWindow()
{
    delete ui;
}

