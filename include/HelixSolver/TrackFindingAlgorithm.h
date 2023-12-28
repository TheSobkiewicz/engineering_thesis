#pragma once

#include <vector>
#include "json.hpp"

#include "HelixSolver/Stub.h"
#include "HelixSolver/KernelExecutionContainer.h"

namespace HelixSolver {

    class TrackFindingAlgorithm {
    public:
        TrackFindingAlgorithm(nlohmann::json &p_config, const Event &p_event);

        void Run();

    private:
        nlohmann::json &config;
        const double bConfig{0};
        const Event &event;
        KernelExecutionContainer kernelExecContainer;
    };

} // HelixSolver
