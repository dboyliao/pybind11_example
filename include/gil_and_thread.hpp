#pragma once
#include "pybind11/pybind11.h"

namespace py = pybind11;

void init_gil_threading(py::module m);