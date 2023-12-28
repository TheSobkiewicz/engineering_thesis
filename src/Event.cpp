#include <fstream>
#include <iostream>

#include "HelixSolver/AccumulatorHelper.h"
#include "HelixSolver/Event.h"

namespace HelixSolver {

    Event::Event(std::string filePath) {
        LoadFromFile(filePath);
    }

    void Event::LoadFromFile(std::string filePath) {
        try {
            std::ifstream l_pointsFile(filePath);
            float l_x, l_y, l_z;
            uint32_t l_layer;
            while (l_pointsFile >> l_x >> l_y >> l_z >> l_layer) {
                stubs.push_back(Stub{l_x, l_y, l_z, static_cast<uint8_t>(l_layer)});
            }
        }
        catch (std::exception &l_exc) {
            std::cerr << l_exc.what() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void Event::Print() const {
        std::cout.precision(64);
        for (auto &stub : stubs) {
            std::cout << stub.x << " " << stub.y << " " << stub.z << std::endl;
        }
    }

    const std::vector<Stub> &Event::GetStubs() const {
        return stubs;
    }

    const std::vector<std::function<float(float)>> &Event::GetStubsFuncs() const {
        return stubsFunctions;
    }

    void Event::BuildStubsFunctions() {
        for (const auto& stub : stubs) {
            const auto[rad, ang] = cart2pol(stub.x, stub.y);
            const float r = rad / 1000.0;

            radiousVector.push_back(r);
            phiVector.push_back(ang);
            layers.push_back(stub.layer);

            auto fun = [r, ang](float x) { return -r * x + ang; };
            stubsFunctions.push_back(fun);
        }
    }

    std::vector<float> Event::GetR() const {
        return radiousVector;
    }
    
    std::vector<float> Event::GetPhi() const {
        return phiVector;
    }

    std::vector<uint8_t> Event::GetLayers() const {
        return layers;
    }

} // namespace HelixSolver
