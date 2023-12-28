#pragma once

#include <vector>
#include "json.hpp"

#include "HelixSolver/Stub.h"
#include "HelixSolver/KernelExecutionContainer.h"

namespace HelixSolver {

    class TrackFindingAlgorithm {
    public:
        TrackFindingAlgorithm(nlohmann::json &config, const Event &event);

        void Run();

    private:
        nlohmann::json &config;
        const double bConfig{0};
        KernelExecutionContainer kernelExecContainer;
    };

} // HelixSolver
