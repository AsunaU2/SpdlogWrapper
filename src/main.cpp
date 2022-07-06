#include <iostream>

#include "sink_manager.h"

using namespace spdlogsink;

int main() {
  SinkInfo sf;
  sf.type = SinkType::SINK_TYPE_STDOUT;
  CSinksManager::GetInstance().CreateSink(sf);

  std::shared_ptr<spdlog::logger> logger1 = std::make_shared<spdlog::logger>("multiple log", begin(CSinksManager::GetInstance().Sinks()), end(CSinksManager::GetInstance().Sinks()));

  logger1->set_level(spdlog::level::trace);
  logger1->debug("xixixixi");
  logger1->info("xixixixi");
  std::cout << "Hello, World!" << std::endl;
  return 0;
}
