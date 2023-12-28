#pragma once

#include <vector>
#include <string_view>
#include <TTree.h>

#include "Point.h"

class SingleRunMap {
public:
    SingleRunMap(TTree *tree, int entryNo);

    int getSize() const;

    void addPoint(const Point &point);

    void dumpToFile(std::string_view fileName) const;

private:
    std::vector<Point> m_points;
};
