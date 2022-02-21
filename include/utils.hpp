#pragma once
#include <iostream>
#include <vector>

template <typename T>
void print_vector(const std::vector<T> &vec, std::ostream &os = std::cout) {
  os << "{";
  for (size_t i = 0; i < vec.size() - 1; ++i) {
    os << vec[i] << ", ";
  }
  if (vec.size() >= 1) {
    os << vec.back();
  }
  os << "}" << std::endl;
}