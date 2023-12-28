#include <fstream>
#include <boost/math/constants/constants.hpp>

#include "HelixSolver/TrackFindingAlgorithm.h"

namespace HelixSolver {

    TrackFindingAlgorithm::TrackFindingAlgorithm(nlohmann::json &config, const Event& event)
            : config(config), bConfig(config["B"].get<double>()), event(event),
              kernelExecContainer(config["main_accumulator_config"], event) {
    }

    void TrackFindingAlgorithm::Run() {
        kernelExecContainer.FillOnDevice();
        kernelExecContainer.PrintMainAcc();
        std::ofstream out(config["outputFile"].get<std::string>());
        out << std::setprecision(64);
        for (const auto &solution : kernelExecContainer.GetSolution()) {
            if (solution.isValid) {
                out << solution.r << " " << solution.phi << std::endl;
            } 
        }
    }

} // namespace HelixSolver
