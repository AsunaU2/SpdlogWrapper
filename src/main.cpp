#include <iostream>

#include "logger_manager.h"
#include "sink_manager.h"
#include "spdlog_manager.h"
#include "wrapper_api.h"

using namespace spdlogsink;

int main() {
  SinkInfo sf;
  std::string pattern = sf.sink_pattern;
  sf.sink_type = SinkType::SINK_TYPE_STDOUT;
  auto sinksManager = std::make_unique<CSinksManager>();
  sinksManager->CreateSinks(
      {SinkInfo{SinkType::SINK_TYPE_STDOUT, "", spd_level::info, 1, 1024, pattern}, SinkInfo{SinkType::SINK_TYPE_BASIC, "./logs/basic_log.log", spd_level::info, 1, 1024, pattern},
       SinkInfo{SinkType::SINK_TYPE_ROTATING, "./logs/rotate_log.log", spd_level::info, 1, 1024, pattern}, SinkInfo{SinkType::SINK_TYPE_DAILY, "./logs/day_log.log", spd_level::info, 1, 1024, pattern},
       SinkInfo{SinkType::SINK_TYPE_ONCE_FILE, "./logs/once.log", spd_level::info, 1, 1024, pattern}});

  auto loggerManager = std::make_unique<CLoggerManager>();

  if (loggerManager->CreateLogger(sinksManager->Sinks())) {
    loggerManager->Logger()->debug("debug");
    loggerManager->Logger()->info("info");
    loggerManager->Logger()->error("error");
    loggerManager->Logger()->log(spdlogsink::spd_level::info, "jjjjjj", "yoyoyo");
  }

  __LOG_INIT__("./config/config.json");

  CSpdlogManager::GetInstance().LogPrint(spdlogsink::spd_level::info, "xxixixi", "yoyoyo");
  LOGI("YOYOYO");

  std::cout << "Hello, World2!" << std::endl;
  return 0;
}