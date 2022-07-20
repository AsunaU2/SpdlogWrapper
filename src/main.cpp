#include <iostream>

#include "logger_manager.h"
#include "sink_manager.h"

using namespace spdlogsink;

int main() {
  SinkInfo sf;
  std::string pattern = sf.sink_pattern;
  sf.sink_type = SinkType::SINK_TYPE_STDOUT;
  CSinksManager::GetInstance().CreateSinks(
      {SinkInfo{SinkType::SINK_TYPE_STDOUT, "", spd_level::info, 1, 1024, pattern}, SinkInfo{SinkType::SINK_TYPE_BASIC, "./logs/basic_log.log", spd_level::info, 1, 1024, pattern},
       SinkInfo{SinkType::SINK_TYPE_ROTATING, "./logs/rotate_log.log", spd_level::info, 1, 1024, pattern}, SinkInfo{SinkType::SINK_TYPE_DAILY, "./logs/day_log.log", spd_level::info, 1, 1024, pattern},
       SinkInfo{SinkType::SINK_TYPE_ONCE_FILE, "./logs/once.log", spd_level::info, 1, 1024, pattern}

      });

  if (CLoggerManager::GetInstance().CreateLogger(CSinksManager::GetInstance().Sinks())) {
    CLoggerManager::GetInstance().Logger()->debug("debug");
    CLoggerManager::GetInstance().Logger()->info("info");
    CLoggerManager::GetInstance().Logger()->error("error");
  }

  std::cout << "Hello, World2!" << std::endl;
  return 0;
}