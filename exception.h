#pragma once
#include <error.h>
#include <cstring>
#include <sstream>
#include <string>

inline void throw_exception(std::string msg) {
  std::stringstream ss;
  ss << msg << ": " << std::strerror(errno);
  throw std::runtime_error(ss.str());
}