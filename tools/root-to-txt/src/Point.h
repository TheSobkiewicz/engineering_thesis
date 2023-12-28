#pragma once

#include <cstdint>

class Point {
public:
    Point(double x, double y, double z, uint8_t layer);

    double getX() const;

    double getY() const;

    double getZ() const;

    uint8_t getLayer() const;

private:
    const double x;
    const double y;
    const double z;
    const uint8_t layer;
};
