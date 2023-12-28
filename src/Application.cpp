#include <iostream>
#include <fstream>

#include "HelixSolver/Application.h"
#include "HelixSolver/TrackFindingAlgorithm.h"

namespace HelixSolver {

    Application::Application(std::vector<std::string> &argv) {
        if (argv.size() < 2) {
            std::cerr << "You must pass configuration file location as program arg!" << std::endl;
            exit(EXIT_FAILURE);
        }
        std::ifstream l_configFile(argv[1]);
        l_configFile >> config;
    }

    int Application::Run() {
        event.LoadFromFile(config["inputFile"]);
        event.BuildStubsFunctions();
        TrackFindingAlgorithm l_algorithm(config, event);
        l_algorithm.Run();
        return 0;
    }

    Application::~Application() {
    }

} // HelixSolver
