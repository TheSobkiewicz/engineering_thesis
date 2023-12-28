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
            std::ifstream pointsFile(filePath);
            float x, y, z;
            uint32_t layer;
            while (pointsFile >> x >> y >> z >> layer) {
                stubs.push_back(Stub{x, y, z, static_cast<uint8_t>(layer)});
            }
        }
        catch (std::exception &exc) {
            std::cerr << exc.what() << std::endl;
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
            const float phi = ang;

            radiousVector.push_back(r);
            phiVector.push_back(ang);
            layers.push_back(stub.layer);

            auto fun = [r, phi](float x) { return -r * x + phi; };
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
