#include <fstream>
#include <iostream>

#include "HelixSolver/AccumulatorHelper.h"
#include "HelixSolver/Event.h"

namespace HelixSolver
{

    void Event::LoadAndProcessDataFromFile(std::string filePath)
    {
        try
        {
            std::ifstream pointsFile(filePath);
            float x, y, z;
            uint32_t layer;
            while (pointsFile >> x >> y >> z >> layer)
            {
                ProcessPoint(x, y, static_cast<uint8_t>(layer));
            }
        }
        catch (std::exception &exc)
        {
            std::cerr << exc.what() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    const std::vector<std::function<float(float)>> &Event::getFunctionVector() const
    {
        return functionVector;
    }

    void Event::ProcessPoint(float x, float y, uint8_t layer)
    {
        const auto [rad, ang] = cart2pol(x, y);
        const float r = rad / 1000.0;
        const float phi = ang;

        radiousVector.push_back(r);
        phiVector.push_back(ang);
        layers.push_back(layer);

        auto fun = [r, phi](float x)
        { return -r * x + phi; };
        functionVector.push_back(fun);
    }

    std::vector<float> Event::GetR() const
    {
        return radiousVector;
    }

    std::vector<float> Event::GetPhi() const
    {
        return phiVector;
    }

    std::vector<uint8_t> Event::GetLayers() const
    {
        return layers;
    }

} // namespace HelixSolver
