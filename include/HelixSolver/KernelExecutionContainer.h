#pragma once

#include <vector>
#include <array>
#include <utility>
#include <functional>

#include "HelixSolver/Event.h"
#include "HelixSolver/Constants.h"
#include "HelixSolver/SolutionCircle.h"

namespace HelixSolver {
    class KernelExecutionContainer {
    public:
        KernelExecutionContainer(const std::string outputFilePath, const Event &m_event);

        void FillOnDevice();

        void PrintMainAcc() const;

        void DumpCalculatedSolutionToFile() const;

    private:
        void PrepareLinspaces();

        const std::string outputFilePath;
        const Event &event;

        std::vector<float> X;
        std::vector<float> Y;

        double deltaY;
        double deltaX;
        double halfOfDeltaX;

        std::array<SolutionCircle, ACC_SIZE> solution;
    };

} // namespace HelixSolver
