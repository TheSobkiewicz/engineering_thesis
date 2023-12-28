#include <iostream>
#include <fstream>

#include "HelixSolver/Application.h"
#include "HelixSolver/TrackFindingAlgorithm.h"

namespace HelixSolver
{

    Application::Application(std::vector<std::string> &argv)
    {
        if (argv.size() < 2)
        {
            std::cerr << "You must pass configuration file location as program arg!" << std::endl;
            exit(EXIT_FAILURE);
        }
        std::ifstream configFile(argv[1]);
        configFile >> config;
    }

    void Application::Run()
    {
        event.LoadFromFile(config["inputFile"]);
        event.BuildStubsFunctions();
        TrackFindingAlgorithm algorithm(config, event);
        algorithm.Run();
    }

} // HelixSolver