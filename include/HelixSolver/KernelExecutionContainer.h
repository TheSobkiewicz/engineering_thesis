#pragma once

#include <vector>
#include <array>
#include <utility>
#include <functional>

#include "json.hpp"
#include "HelixSolver/Event.h"
#include "HelixSolver/Constants.h"
#include "HelixSolver/SolutionCircle.h"

namespace HelixSolver {

    using VectorIdxPair = std::vector<std::pair<uint32_t, uint32_t>>;

    class KernelExecutionContainer {
    public:
        KernelExecutionContainer(nlohmann::json &config, const Event &m_event);

        void Fill();

        void FillOnDevice();

        VectorIdxPair GetCellsAboveThreshold(uint8_t threshold) const;

        void PrintMainAcc() const;

        std::pair<double, double> GetValuesOfIndexes(uint32_t x, uint32_t y) const;

        const std::array<SolutionCircle, ACC_SIZE> &GetSolution() const;

    private:
        void PrepareLinspaces();

        nlohmann::json &config;
        const Event &event;

        std::vector<float> X;
        std::vector<float> Y;

        double deltaY;
        double deltaX;
        double halfOfDeltaX;

        std::array<SolutionCircle, ACC_SIZE> solution;
    };

} // namespace HelixSolver
