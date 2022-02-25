import pybind11_example
import numpy as np

pybind11_example.print_buffer_info(np.random.rand(3, 5))
print(pybind11_example.arr_add_value(np.ones((5, 3), dtype=np.double), np.pi))
print(np.array(pybind11_example.Matrix_f(5, 2)))
