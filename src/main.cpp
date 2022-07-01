#include <iostream>
#include "sink_manager.h"

int main() {

  CStdoutColorSinkManager st {spdlog::level::debug, ""};
  std::vector<spdlog::sink_ptr> m_sinks;
  m_sinks.push_back(st.Sink());
  std::shared_ptr<spdlog::logger> logger1 = std::make_shared<spdlog::logger>("multiple log", begin( m_sinks ), end( m_sinks ));

  logger1->set_level(spdlog::level::trace);
  logger1->debug("xixixixi");
  logger1->info("xixixixi");
  std::cout << "Hello, World!" << std::endl;
  return 0;
}
