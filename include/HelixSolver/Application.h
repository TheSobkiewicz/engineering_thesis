#pragma once

#include "json.hpp"

#include "HelixSolver/Event.h"

namespace HelixSolver {

    class Application {
        public:
            explicit Application(std::vector<std::string> &argv);
            ~Application() {}
            void Run();

        private:
            Event event;
            nlohmann::json config;
    };

} // HelixSolver
