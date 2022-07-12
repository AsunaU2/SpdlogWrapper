#include <iostream>

#include "sink_manager.h"

using namespace spdlogsink;

int main() {
  SinkInfo sf;
  std::string pattern = sf.sink_pattern;
  sf.sink_type = SinkType::SINK_TYPE_STDOUT;
  CSinksManager::GetInstance().CreateSinks(
      {SinkInfo{SinkType::SINK_TYPE_STDOUT, "", spd_level::info, 1, 1024, pattern}, SinkInfo{SinkType::SINK_TYPE_BASIC, "./logs/basic_log.log", spd_level::info, 1, 1024, pattern},
       SinkInfo{SinkType::SINK_TYPE_ROTATING, "./logs/rotate_log.log", spd_level::info, 1, 1024, pattern}, SinkInfo{SinkType::SINK_TYPE_DAILY, "./logs/day_log.log", spd_level::info, 1, 1024, pattern},
       SinkInfo{SinkType::SINK_TYPE_ONCE_FILE, "./logs/day_log.<log", spd_level::info, 1, 1024, pattern}

      });

  std::shared_ptr<spdlog::logger> logger1 = std::make_shared<spdlog::logger>("multiple log", begin(CSinksManager::GetInstance().Sinks()), end(CSinksManager::GetInstance().Sinks()));

  logger1->set_level(spdlog::level::trace);
  logger1->debug("xixixixi");
  logger1->info("xixixixi");
  std::cout << "Hello, World2!" << std::endl;
  return 0;
}