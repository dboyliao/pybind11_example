#include "numpy_example.hpp"

#include <iostream>

#include "pybind11/numpy.h"
#include "utils.hpp"

namespace {
template <typename T>
class Matrix {
 public:
  using elemType = T;
  Matrix(size_t rows, size_t cols) : m_rows(rows), m_cols(cols) {
    m_data = new elemType[rows * cols];
    memset(m_data, 0, static_cast<size_t>(rows * cols) * sizeof(elemType));
  }
  T *data() const { return m_data; }
  size_t rows() const { return m_rows; }
  size_t cols() const { return m_cols; }
  ~Matrix() { delete[] m_data; }
  py::buffer_info get_buff_info() const {
    return py::buffer_info(
        this->data(),                              /* Pointer to buffer */
        sizeof(T),                                 /* Size of one scalar */
        py::format_descriptor<elemType>::format(), /* Python struct-style
                                                   format descriptor */
        2,                                         /* Number of dimensions */
        {this->rows(), this->cols()},              /* Buffer dimensions */
        {sizeof(elemType) *
             this->cols(), /* Strides (in bytes) for each index */
         sizeof(elemType)});
  }

 private:
  size_t m_rows, m_cols;
  elemType *m_data;
};
}  // namespace

static void print_buffer_info(py::buffer b) {
  auto info = b.request();
  std::cout << "ptr: " << info.ptr << std::endl;
  std::cout << "item size: " << info.itemsize << std::endl;
  std::cout << "size: " << info.size << std::endl;
  std::cout << "format: " << info.format << std::endl;
  std::cout << "ndim: " << info.ndim << std::endl;
  std::cout << "shape: ";
  print_vector(info.shape);
  std::cout << "strides (in bytes): ";
  print_vector(info.strides);
}

static double vectorize_kernel(double x, double y, double z) {
  return 3.0 * x + y + 1.24 * z;
}

static py::array_t<double> arr_add_value(const py::array_t<double> &in_arr,
                                         double v = 1.0) {
  auto info = in_arr.request();
  double *out_buff = new double[info.size];
  auto in_ptr = (double *)info.ptr;
  for (py::ssize_t idx = 0; idx < info.size; ++idx) {
    *(out_buff + idx) = *(in_ptr + idx) + v;
  }
  auto out_info = py::buffer_info(out_buff, info.itemsize, info.format,
                                  info.ndim, info.shape, info.strides);
  return py::array_t<double>(out_info);
}

void init_numpy_example(py::module m) {
  m.def("print_buffer_info", &print_buffer_info, R"(print buffer info)");
  m.def("vectorize_example", py::vectorize(vectorize_kernel),
        R"(vectorized function)");
  m.def("vectorize_kernel", &vectorize_kernel, R"(vectorize kernel function)");
  m.def("arr_add_value", &arr_add_value, R"(add by given value)",
        py::arg("input_array"), py::arg("value") = 0.0);
  py::class_<Matrix<float>>(m, "Matrix_f", py::buffer_protocol())
      .def(py::init<size_t, size_t>())
      .def_buffer([](const Matrix<float> &m) -> py::buffer_info {
        // this makes Matrix can be converted to numpy array via np.array
        return m.get_buff_info();
      });
  // double
  py::class_<Matrix<double>>(m, "Matrix_d", py::buffer_protocol())
      .def(py::init<size_t, size_t>())
      .def_buffer([](const Matrix<double> &m) -> py::buffer_info {
        return m.get_buff_info();
      });
  // int
  py::class_<Matrix<int>>(m, "Matrix_i", py::buffer_protocol())
      .def(py::init<size_t, size_t>())
      .def_buffer([](const Matrix<int> &m) -> py::buffer_info {
        return m.get_buff_info();
      });
  // long
  py::class_<Matrix<long>>(m, "Matrix_l", py::buffer_protocol())
      .def(py::init<size_t, size_t>())
      .def_buffer([](const Matrix<long> &m) -> py::buffer_info {
        return m.get_buff_info();
      });
}