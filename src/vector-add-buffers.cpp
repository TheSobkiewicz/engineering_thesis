
#include <sycl/sycl.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <chrono>
#include <cmath>
#if FPGA_HARDWARE || FPGA_EMULATOR || FPGA_SIMULATOR
#include <sycl/ext/intel/fpga_extensions.hpp>
#endif

using namespace sycl;

size_t num_repetitions = 1;
typedef std::vector<int> IntVector;
const int number_of_tries = 4;
const int base = 10;

    // Create an exception handler for asynchronous SYCL exceptions
    static auto exception_handler = [](sycl::exception_list e_list)
{
  for (std::exception_ptr const &e : e_list)
  {
    try
    {
      std::rethrow_exception(e);
    }
    catch (std::exception const &e)
    {
#if _DEBUG
      std::cout << "Failure" << std::endl;
#endif
      std::terminate();
    }
  }
};

//************************************
// Vector add in SYCL on device: returns sum in 4th parameter "sum_parallel".
//************************************
void VectorAdd(queue &q, const IntVector &a_vector, const IntVector &b_vector, const IntVector &c_vector,
               IntVector &sum_parallel)
{
  range<1> num_items{a_vector.size()};

  buffer a_buf(a_vector);
  buffer b_buf(b_vector);
  buffer c_buf(c_vector);
  buffer sum_buf(sum_parallel.data(), num_items);

  for (size_t i = 0; i <= num_repetitions; i++)
  {

    q.submit([&](handler &h)
             {
      accessor a(a_buf, h, read_only);
      accessor b(b_buf, h, read_only);
      accessor c(c_buf, h, read_only);

      accessor sum(sum_buf, h, write_only, no_init);
  
      h.parallel_for(num_items, [=](auto i) { sum[i] = a[i] + b[i] + c[i]; }); });
  };
  q.wait();
}

void InitializeVector(IntVector &a)
{
  for (size_t i = 0; i < a.size(); i++)
    a.at(i) = i;
}

int main(int argc, char *argv[])
{
#if FPGA_EMULATOR
  auto selector = sycl::ext::intel::fpga_emulator_selector_v;
#elif FPGA_SIMULATOR
  auto selector = sycl::ext::intel::fpga_simulator_selector_v;
#elif FPGA_HARDWARE
  auto selector = sycl::ext::intel::fpga_selector_v;
#else
  auto selector = default_selector_v;
#endif
  queue q(selector, exception_handler);
  std::cout << "Running on device: "
            << q.get_device().get_info<info::device::name>() << "\n";
  size_t sizes[number_of_tries];

  for (int i = 0; i <= number_of_tries; i++)
  {
    sizes[i] = pow(base, i);
  }


  for (int i = 0; i < number_of_tries; i++) 
  {
    IntVector a, b, c, validation_sum, sum_parallel;
    const size_t vector_size = sizes[i];
    a.resize(vector_size);
    b.resize(vector_size);
    c.resize(vector_size);
    validation_sum.resize(vector_size);
    sum_parallel.resize(vector_size);

    InitializeVector(a);
    InitializeVector(b);
    InitializeVector(c);

    auto start = std::chrono::system_clock::now();
    VectorAdd(q, a, b, c, sum_parallel);
    auto stop = std::chrono::system_clock::now();

    for (size_t i = 0; i < validation_sum.size(); i++)
      validation_sum.at(i) = a.at(i) + b.at(i) + c.at(i);

    for (size_t i = 0; i < validation_sum.size(); i++)
    {
      if (sum_parallel.at(i) != validation_sum.at(i))
      {
        std::cout << "Vector add failed on device.\n";
        return -1;
      }
    }

    a.clear();
    b.clear();
    c.clear();
    validation_sum.clear();
    sum_parallel.clear();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Vector add successfully completed on device. Vector size: " << vector_size << ". Execution time: " << duration.count() <<" microseconds. \n";
  }
  return 0;
}
