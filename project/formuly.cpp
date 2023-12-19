#include "formuly.h"


double formula(double f, double d) {
    return 28 + 22 * log10(f) + 20 * log10(d);
}

float calculate(int x1,int x2,int y1,int y2){
    return sqrt(pow(abs(x1 - x2),2) + pow(abs(y1 - y2),2));
}

float pix_translate(float x, int pix_distance){
    return x*pix_distance;
}
