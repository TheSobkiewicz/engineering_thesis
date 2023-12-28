#include <iostream>
#include <limits>
#include <cmath>

#include "HelixSolver/HoughTransformKernel.h"

#include <sycl/ext/intel/fpga_extensions.hpp>


#include "HelixSolver/KernelExecutionContainer.h"
#include "HelixSolver/AccumulatorHelper.h"


namespace HelixSolver {

    KernelExecutionContainer::KernelExecutionContainer(nlohmann::json &config, const Event &event)
            : config(config), event(event) {
        PrepareLinspaces();

        deltaX = X[1] - X[0];
        halfOfDeltaX = deltaX / 2;

        deltaY = Y[1] - Y[0];

        solution.fill(SolutionCircle{0,0,0});
    }

    void KernelExecutionContainer::FillOnDevice() {

        #if defined(FPGA_EMULATOR)
             auto selector = sycl::ext::intel::fpga_emulator_selector_v;
        #else
            auto selector = sycl::ext::intel::fpga_selector_v;
        #endif

        auto propertyList = sycl::property_list{sycl::property::queue::enable_profiling()};
        sycl::queue fpgaQueue(selector, NULL, propertyList);

        sycl::platform platform = fpgaQueue.get_context().get_platform();
        sycl::device device = fpgaQueue.get_device();
        std::cout << "Platform: " <<  platform.get_info<sycl::info::platform::name>().c_str() << std::endl;
        std::cout << "Device: " <<  device.get_info<sycl::info::device::name>().c_str() << std::endl;

        sycl::buffer<SolutionCircle, 1> mapBuffer(ACC_SIZE);

        std::vector<float> rVec = event.GetR();
        std::vector<float> phiVec = event.GetPhi();
        std::vector<uint8_t> layers = event.GetLayers();

        sycl::buffer<float, 1> rBuffer(rVec.begin(), rVec.end());
        sycl::buffer<float, 1> phiBuffer(phiVec.begin(), phiVec.end());
        sycl::buffer<uint8_t, 1> layersBuffer(layers.begin(), layers.end());

        sycl::buffer<float, 1> XLinspaceBuf(X.begin(), X.end());
        sycl::buffer<float, 1> YLinspaceBuf(Y.begin(), Y.end());


        sycl::event qEvent = fpgaQueue.submit([&](sycl::handler &h) {

            HoughTransformKernel kernel(h, mapBuffer, rBuffer, phiBuffer, layersBuffer, XLinspaceBuf, YLinspaceBuf);

            h.single_task<HoughTransformKernel>(kernel);
        });

        qEvent.wait();

        sycl::cl_ulong kernelStartTime = qEvent.get_profiling_info<sycl::info::event_profiling::command_start>();
        sycl::cl_ulong kernelEndTime = qEvent.get_profiling_info<sycl::info::event_profiling::command_end>();
        double kernelTime = (kernelEndTime - kernelStartTime) / NS;

        std::cout << "Execution time: " << kernelTime << " seconds" << std::endl;

        sycl::host_accessor hostMapAccessor(mapBuffer, sycl::read_only);
        for (uint32_t i = 0; i < ACC_SIZE; ++i) solution[i] = hostMapAccessor[i];
    }

    void KernelExecutionContainer::Fill() {
        for (const auto& stubFunc : event.GetStubsFuncs()) {
            for (uint32_t i = 0; i < X.size(); ++i) {
                float x = X[i];
                float xLeft = x - halfOfDeltaX;
                float xRight = x + halfOfDeltaX;
                
                float yLeft = stubFunc(xLeft);
                float yRight = stubFunc(xRight);

                float yLeftIdx = FindClosest(Y, yLeft);
                float yRightIdx = FindClosest(Y, yRight);

                for (uint32_t j = yRightIdx; j <= yLeftIdx; ++j) {
                    solution[j * ACC_WIDTH + i].isValid = true;
                }
            }
        }
    }

    const std::array<SolutionCircle, ACC_SIZE> &KernelExecutionContainer::GetSolution() const {
        return solution;
    }

    void KernelExecutionContainer::PrepareLinspaces() {
        linspace(X,
                 Q_OVER_P_BEGIN,
                 Q_OVER_P_END,
                 ACC_WIDTH);

        linspace(Y,
                 PHI_BEGIN,
                 PHI_END,
                 ACC_HEIGHT);
    }

    void KernelExecutionContainer::PrintMainAcc() const {
        for (uint32_t i = 0; i < ACC_HEIGHT; ++i) {
            for (uint32_t j = 0; j < ACC_WIDTH; ++j) {
                std::cout << int(solution[i * ACC_WIDTH + j].isValid) << " ";
            }
            std::cout << std::endl;
        }
    }

    std::pair<double, double> KernelExecutionContainer::GetValuesOfIndexes(uint32_t x, uint32_t y) const {
        return std::pair<double, double>(X[x], Y[y]);
    }

} // namespace HelixSolver
