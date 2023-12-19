#ifndef MATERIALS_H
#define MATERIALS_H

#include <QtWidgets>
#include <QGroupBox>
#include <QWidget>
#include <cmath>

void add_material_to_map(int **map, int px, int py, int sx, int sy, int type);
struct Material {
    QColor color;
    double absorption;
};
extern Material materials[];

#endif // MATERIALS_H
