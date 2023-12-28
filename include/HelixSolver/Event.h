#pragma once

#include <vector>
#include <functional>
#include "json.hpp"

#include "HelixSolver/Stub.h"

namespace HelixSolver {

    class Event {
    public:
        Event() = default;

        Event(std::string filePath);

        void LoadFromFile(std::string filePath);
        // TODO: loadFromRootFile

        const std::vector<Stub> &GetStubs() const;
        const std::vector<std::function<float(float)>> &GetStubsFuncs() const;
        
        std::vector<float> GetR() const;
        std::vector<float> GetPhi() const;
        std::vector<uint8_t> GetLayers() const;

        void BuildStubsFunctions();

        void Print() const;

    private:
        std::vector<Stub> stubs;
        std::vector<std::function<float(float)>> stubsFunctions;
        std::vector<float> radiousVector;
        std::vector<float> phiVector;
        std::vector<uint8_t> layers;
    };

} // HelixSolver
