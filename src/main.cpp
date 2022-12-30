#include <iostream>

#include "logger_manager.h"
#include "sink_manager.h"
#include "spdlog_manager.h"
#include "spdlog_wrapper_api.h"

using namespace spdlogsink;

int main() {
  LOG_INIT_("");

  for (int i = 0; i < 1024; ++i) LOGE("YOYOY2O {}", 4);

  std::cout << "Hello, World2!" << std::endl;
  return 0;
}