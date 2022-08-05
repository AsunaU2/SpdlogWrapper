#include <iostream>

#include "logger_manager.h"
#include "sink_manager.h"
#include "spdlog_manager.h"
#include "wrapper_api.h"

using namespace spdlogsink;

int main() {
  __LOG_INIT__("./config/config.json");

  LOGI("YOYOYO {}", 4);

  std::cout << "Hello, World2!" << std::endl;
  return 0;
}