#pragma once
#include "pybind11/pybind11.h"

namespace py = pybind11;

void init_numpy_example(py::module m);
