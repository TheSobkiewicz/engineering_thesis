#include "Point.h"

Point::Point(double x, double y, double z, uint8_t layer) :
        x(x), y(y), z(z), layer(layer) {}

double Point::getX() const {
    return x;
}

double Point::getY() const {
    return y;
}

double Point::getZ() const {
    return z;
}

uint8_t Point::getLayer() const {
    return layer;
}