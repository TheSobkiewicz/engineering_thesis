#include <iostream>
#include <fstream>

#include "HelixSolver/Application.h"
#include "HelixSolver/KernelExecutionContainer.h"

namespace HelixSolver
{

    Application::Application(std::vector<std::string> &argv) {
        if (argv.size() < 2)
        {
            std::cerr << "You must pass configuration file location as program arg!" << std::endl;
            exit(EXIT_FAILURE);
        }
        std::ifstream configFile(argv[1]);
        configFile >> config;
        event.LoadAndProcessDataFromFile(config["inputFile"].get<std::string>());
    }

    void Application::Run() {
        KernelExecutionContainer kernelExecContainer(config["outputFile"].get<std::string>(), event);
        kernelExecContainer.FillOnDevice();
        kernelExecContainer.PrintMainAcc();
        kernelExecContainer.DumpCalculatedSolutionToFile();
    }

} // HelixSolver
