#include "numpy_example.hpp"
#include "numpy_opencv.hpp"
#include "pybind11/pybind11.h"

namespace py = pybind11;

int cpp_add(int a, int b) { return a + b; }

PYBIND11_MODULE(pybind11_numpy, m) {
  m.doc() = R"(Simple Pybind11/Numpy example)";
  m.def("add", &cpp_add, R"(adding two integers in C++)", py::arg("a"),
        py::arg("b") = 0);
  // initialize modules
  init_numpy_example(m);
  init_numpy_opencv(m);
}