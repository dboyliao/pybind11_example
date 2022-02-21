#include "numpy_opencv.hpp"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "pybind11/numpy.h"

void thread_kernel_gil_released() {
  py::gil_scoped_release release;
  std::cout << "starting: " << std::this_thread::get_id() << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  std::cout << "ended" << std::endl;
}

void thread_kernel_with_gil() {
  std::cout << "starting: " << std::this_thread::get_id() << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  std::cout << "ended" << std::endl;
}

std::string get_format_descriptor(int cv_dtype) {
  // https://docs.opencv.org/4.5.4/d1/d1b/group__core__hal__interface.html
  std::string ret_str;
  switch (cv_dtype) {
    case CV_8UC1:
    case CV_8UC2:
    case CV_8UC3:
    case CV_8UC4:
      ret_str = py::format_descriptor<uchar>::format();
      break;
    case CV_8SC1:
    case CV_8SC2:
    case CV_8SC3:
    case CV_8SC4:
      ret_str = py::format_descriptor<char>::format();
      break;
    case CV_16UC1:
    case CV_16UC2:
    case CV_16UC3:
    case CV_16UC4:
      ret_str = py::format_descriptor<ushort>::format();
      break;
    case CV_16SC1:
    case CV_16SC2:
    case CV_16SC3:
    case CV_16SC4:
    case CV_16FC1:
    case CV_16FC2:
    case CV_16FC3:
    case CV_16FC4:
      ret_str = py::format_descriptor<short>::format();
      break;
    case CV_32SC1:
    case CV_32SC2:
    case CV_32SC3:
    case CV_32SC4:
      ret_str = py::format_descriptor<int>::format();
      break;
    case CV_32FC1:
    case CV_32FC2:
    case CV_32FC3:
    case CV_32FC4:
      ret_str = py::format_descriptor<float>::format();
      break;
    case CV_64FC1:
    case CV_64FC2:
    case CV_64FC3:
    case CV_64FC4:
      ret_str = py::format_descriptor<double>::format();
      break;
    default:
      break;
  }
  return ret_str;
}

py::array convert_to_array(const cv::Mat &cv_mat) {
  // 1. copy the data from given Mat
  size_t data_size = cv_mat.total() * cv_mat.elemSize();
  uchar *data = new uchar[data_size];
  memcpy(data, cv_mat.data, data_size);
  // 2. build buffer info
  auto info = py::buffer_info(
      data,                                    // data
      cv_mat.step.buf[1] / cv_mat.channels(),  // element size in bytes
      get_format_descriptor(
          cv_mat.type()),  // Python-struct format description string
      3,                   // number of dims
      {cv_mat.rows, cv_mat.cols, cv_mat.channels()},  // shape
      {cv_mat.step.buf[0], cv_mat.step.buf[1],
       cv_mat.step.buf[1] / static_cast<size_t>(cv_mat.channels())}  // strides
  );
  return py::array(info);
}

void init_numpy_opencv(py::module m) {
  m.def(
       "get_format_descriptor", &get_format_descriptor,
       R"(cv_type: integer of OpenCV Mat datatype, see https://docs.opencv.org/4.5.4/d1/d1b/group__core__hal__interface.html for full list)",
       py::arg("cv_type"))
      .def("read_img",
           [](const std::string &path) {
             cv::Mat img = cv::imread(path, cv::IMREAD_UNCHANGED);
             return convert_to_array(img);
           })
      .def("thread_kernel_gil_released", thread_kernel_gil_released)
      .def("thread_kernel_with_gil", thread_kernel_with_gil);
}