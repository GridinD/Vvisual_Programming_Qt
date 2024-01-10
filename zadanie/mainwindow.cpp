#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QPainter>
#include <cmath>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>
float PL(float f, float d){
    return 32.4 + 21 * log10(d) + 20 * log10(f);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGraphicsScene* scene = new QGraphicsScene();
    int maxX = 1200;
    int maxY = 1000;
    int onePixDistance = 3;
    int TxPower = 13;
    int antGain = 12;
    float freq = 7.5;

    QPixmap map(maxX, maxY);
    QPainter p(&map);

    int cellPosX = 100;
    int cellPosY = 200;
    double *data[maxX];
    for(int i = 0; i < maxX; ++i){
        data[i] = new double[maxY];
        memset(data[i], 0.0, maxY);
    }
    //memset(&data[0], 0, maxY * maxX);
    double sigPower;
    for(int t = 0; t < 2; ++t){
        cellPosX = 100;
        for(int t2 = 0; t2 < 6; ++t2){

            for(int i = 0; i < maxX; i++){
                for(int j = 0; j < maxY; j++){
                    float distance = 0;
                    distance = sqrt(pow(abs(cellPosX - i),2) + pow(abs(cellPosY - j),2)); // [pix]
                    distance *= onePixDistance;
                    sigPower = TxPower + antGain - PL(freq, distance);
                    //data[i][j] -= std::min(sigPower, data[i][j]);
                    //if(data[i][j] > )
                    if(data[i][j] == 0.0)
                        data[i][j] = sigPower;
                    else
                        data[i][j] = (double)std::max(sigPower, data[i][j]);
                    //data[i][j] = (sigPower);

                }
            }
            cellPosX += 120;


        }
        cellPosY += 400;

    }

    for(int i = 0; i < maxX; i++){
        for(int j = 0; j < maxY; j++){
            //std::cerr<<i<<"  "<<j<<"\n";
            sigPower = data[i][j];
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
            }else{
                //}else if (sigPower >= -134){
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
