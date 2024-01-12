#pragma once

#include <vector>
#include <functional>


namespace HelixSolver
{

    class Event
    {
    public:
        void LoadAndProcessDataFromFile(std::string filePath);

        const std::vector<std::function<float(float)>> &getFunctionVector() const;
        std::vector<float> GetR() const;
        std::vector<float> GetPhi() const;
        std::vector<uint8_t> GetLayers() const;

    private:
        void ProcessPoint(float x, float y, uint8_t layer);
        std::vector<std::function<float(float)>> functionVector;
        std::vector<float> radiousVector;
        std::vector<float> phiVector;
        std::vector<uint8_t> layers;
    };

} // HelixSolver
