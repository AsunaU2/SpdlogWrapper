#include <iostream>

#include "logger_manager.h"
#include "sink_manager.h"
#include "spdlog_manager.h"

using namespace spdlogsink;

int main() {
  try {
    std::ifstream ifs{"./config/config.json"};
    nlohmann::json js = nlohmann::json::parse(ifs);
    if (js.contains("config")) {
      auto configArray = js["config"];
      for (const auto &config : configArray) {
        if (config.contains("sinkType")) {
          std::cout << js["sinkType"] << std::endl;
        }
        if (js.contains("logPath")) {
          std::cout << js["logPath"] << std::endl;
        }
      }
    }
    for (const auto &i : js) {
      //      if (i["enable"]) {
      //        std::cout << i["sinkType"] << std::endl;
      //        std::cout << i["logPath"] << std::endl;
      //        std::cout << i["rotateSize"] << std::endl;
      //        std::cout << i["rotateCount"] << std::endl;
      //        std::cout << i["sinkLevel"] << std::endl;
      //      }
    }
  } catch (...) {
    std::cerr << "二姐" << std::endl;
  }

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