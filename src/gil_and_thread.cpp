#include "gil_and_thread.hpp"

#include <chrono>
#include <iostream>
#include <thread>

static void thread_kernel_gil_released() {
  py::gil_scoped_release release;
  std::cout << "starting: " << std::this_thread::get_id() << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  std::cout << "ended" << std::endl;
}

static void thread_kernel_with_gil() {
  std::cout << "starting: " << std::this_thread::get_id() << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  std::cout << "ended" << std::endl;
}

void init_gil_threading(py::module m) {
  m.def("thread_kernel_gil_released", thread_kernel_gil_released)
      .def("thread_kernel_with_gil", thread_kernel_with_gil);
}