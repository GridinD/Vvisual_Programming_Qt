#include "mainwindow.h"
#include "materials.h"
//#include "color.h"
#include "brez.h"
#include "formuly.h"
#include <QtWidgets>
#include <QGroupBox>
#include <QWidget>
#include <cmath>

#define Pix_to_Meters 10
#define Tx_Power 23
#define Antena_Gain -12
#define frequency 2.5
#define Bs_PosX 500
#define Bs_PosY 500

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(1200, 1000);
    QGraphicsScene* scene = new QGraphicsScene();
    QPixmap map(SIZE_MAP_X, SIZE_MAP_Y);

    map_obj = new int*[SIZE_MAP_Y];
    for(int i = 0; i < SIZE_MAP_Y; ++i){
        map_obj[i] = new int[SIZE_MAP_X]{};
    }

    setupMap();
    drawSignalStrength(&map);


    scene->addPixmap(map);
    QGraphicsView* view = new QGraphicsView(scene);
    setCentralWidget(view);
    view -> setGeometry(300, 0, SIZE_MAP_X, SIZE_MAP_Y);
    this->layout()->addWidget(view);

    gradientGroupBox_ = new QGroupBox(QStringLiteral("Signal Strength"));
    gradientGroupBox_->setGeometry(50, 100, 200, 700);
    this->layout()->addWidget(gradientGroupBox_);
    createColorMap();
}

void MainWindow::setupMap() {
    add_material_to_map(map_obj, 500, 350, 20, 30, (int)type_material::GLASS);
    add_material_to_map(map_obj, 500, 700, 30, 40, (int)type_material::CONCRETE);
    add_material_to_map(map_obj, 700, 500, 50, 80, (int)type_material::WOOD);
}

void MainWindow::drawSignalStrength(QPixmap *map) {
    QPainter p(map);
    for (int i = 0; i < SIZE_MAP_Y; i++) {
        for(int j = 0; j < SIZE_MAP_X; j++) {
            float distance = 0;
            distance = calculate(Bs_PosX, i, Bs_PosY, j);
            distance = pix_translate(distance, Pix_to_Meters);
            float Signal_Power = Tx_Power + Antena_Gain - formula(frequency, distance);

            int df = drawBresenhamLine(&p, map_obj, Bs_PosX, Bs_PosY, i, j);
            Signal_Power -= df;

            QColor signalColor;
            if(Signal_Power < -44 && Signal_Power >= -54){
                signalColor = QColor(255, 0, 0, 255);
            } else if (Signal_Power < -54 && Signal_Power >= -64) {
                signalColor = QColor(220, 50, 0, 255);
            } else if (Signal_Power < -64 && Signal_Power >= -74){
                signalColor = QColor(255,165,0,255);
            } else if(Signal_Power < -74 && Signal_Power >= -84){
                signalColor = QColor(255,255,0,255);
            } else if(Signal_Power < -84 && Signal_Power >= -94){
                signalColor = QColor(173,255,47,255);
            } else if(Signal_Power < -94 && Signal_Power >= -104){
                signalColor = QColor(0,255,0,255);
            } else if(Signal_Power < -104 && Signal_Power >= -114){
                signalColor = QColor(0,250,154,255);
            } else if(Signal_Power < -114 && Signal_Power >= -124){
                signalColor = QColor(102,205,170,255);
            } else if(Signal_Power < -124 && Signal_Power >= -134){
                signalColor = QColor(65,105,225,255);
            } else if(Signal_Power < -134 && Signal_Power >= -144){
                signalColor = QColor(0,0,255,255);
            }

            p.setPen(signalColor);
            p.drawPoint(i, j);
        }
    }
    p.end();
}

void MainWindow::createColorMap() {
    int height = 400;
    int width = 150;
    int border = 10;
    QLinearGradient gr(0, 0, 1, height - 2 * border);
    gr.setColorAt(1.0f, Qt::blue);
    gr.setColorAt(0.8f, Qt::cyan);
    gr.setColorAt(0.6f, Qt::green);
    gr.setColorAt(0.4f, Qt::yellow);
    gr.setColorAt(0.0f, Qt::red);

    QPixmap pm(width, height);
    pm.fill(Qt::transparent);
    QPainter pmp(&pm);
    pmp.setBrush(QBrush(gr));
    pmp.setPen(Qt::NoPen);
    pmp.drawRect(border, border, 35, height - 2 * border);
    pmp.setPen(Qt::black);
    int step = (height - 2 * border) / 10;
    for (int i = 0; i < 11; i++) {
        int yPos = i * step + border;
        pmp.drawLine(border, yPos, 55, yPos);
        pmp.drawText(60, yPos + 2, QString("%1 [dBm]").arg(-44 - (i * 10)));
    }

    label_ = new QLabel();
    label_->setPixmap(pm);

    QVBoxLayout *colorMapVBox = new QVBoxLayout;
    colorMapVBox->addWidget(label_);
    gradientGroupBox_->setLayout(colorMapVBox);
}

MainWindow::~MainWindow()
{


}
