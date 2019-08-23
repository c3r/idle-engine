#pragma once
#ifdef __APPLE__
  #include <mach/error.h>
#else
  #include <error.h>
#endif
#include <cstring>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>

inline void throw_exception(std::string msg) {
  std::stringstream ss;
  ss << msg << ": " << std::strerror(errno);
  throw std::runtime_error(ss.str());
}