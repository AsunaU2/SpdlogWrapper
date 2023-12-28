#include <iostream>

#include "wrapper/spdlog_wrapper_api.h"

int main() {
  LOG_INIT_("");

  for (int i = 0; i < 1024; ++i) LOGE("YOYOY2O {}", 4);

  std::cout << "Hello, World2!" << std::endl;
  return 0;
}
