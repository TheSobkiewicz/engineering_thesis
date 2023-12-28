#include "SingleRunMap.h"
#include <fstream>
#include <algorithm>

SingleRunMap::SingleRunMap(TTree *tree, int entryNumber) {
    std::vector<double> *l_spX = new std::vector<double>();
    std::vector<double> *l_spY = new std::vector<double>();
    std::vector<double> *l_spZ = new std::vector<double>();
    std::vector<uint8_t> *l_spLayer = new std::vector<uint8_t>();

    tree->SetBranchAddress("spX", &l_spX);
    tree->SetBranchAddress("spY", &l_spY);
    tree->SetBranchAddress("spZ", &l_spZ);
    tree->SetBranchAddress("spLayer", &l_spLayer);
    tree->GetEntry(entryNumber);

    int l_entrySize = l_spX->size();

    for (int i = 0; i < l_entrySize; ++i) {
        this->addPoint(Point(l_spX->at(i), l_spY->at(i), l_spZ->at(i), l_spLayer->at(i)));
    }

    delete l_spX;
    delete l_spY;
    delete l_spZ;
    delete l_spLayer;
}

int SingleRunMap::getSize() const {
    return m_points.size();
}

void SingleRunMap::addPoint(const Point &point) {
    m_points.push_back(point);
}

void SingleRunMap::dumpToFile(std::string_view fileName) const {
    std::ofstream file;
    file.open(fileName);
    file.precision(64);
    std::for_each(m_points.begin(), m_points.end(),
                  [&file](const Point &l_point) {
                      file << l_point.getX() << " " << l_point.getY() << " " << l_point.getZ() << " " << static_cast<int>(l_point.getLayer()) << std::endl;
                  });

    file.close();
}
