#include "brez.h"
#include "mainwindow.h"
#include "materials.h"

double drawBresenhamLine(QPainter *p, int **map_obj, int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    double f = 0;

    while (true) {
        if (x1 == x2 && y1 == y2) {
            break;
        }

        int err2 = 2 * err;

        if (err2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if (err2 < dx) {
            err += dx;
            y1 += sy;
        }
        switch(map_obj[y1][x1]){
        case (int)type_material::NO_MATERIAL:

            break;
        default:
            f += materials[map_obj[y1][x1] - 1].absorption;
            p->setPen(materials[map_obj[y1][x1] - 1].color);
            p->drawPoint(x1, y1);
            break;
        }


    }

    return f;
}
