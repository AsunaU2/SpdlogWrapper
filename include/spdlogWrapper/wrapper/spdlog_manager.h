//
// Created by Kirito on 2022/7/20.
//

#ifndef SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_SPDLOG_MANAGER_H_
#define SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_SPDLOG_MANAGER_H_

#include <fstream>

#include "../nlohmannJson/json.hpp"
#include "sink_manager.h"

class IConfigTransfer {
 public:
  virtual ~IConfigTransfer() = default;
  IConfigTransfer(const IConfigTransfer &) = delete;
  IConfigTransfer &operator=(const IConfigTransfer &) = delete;
  IConfigTransfer(IConfigTransfer &&) = delete;
  IConfigTransfer &operator=(IConfigTransfer &&) = delete;

  virtual std::pair<bool, std::vector<spdlogsink::SinkInfo>> TransferConfig() = 0;
};

class CJsonConfigTransfer : public IConfigTransfer {
 public:
  std::pair<bool, std::vector<spdlogsink::SinkInfo>> TransferConfig() override {
    bool ret = false;
    std::vector<spdlogsink::SinkInfo> sinkInfos;

    try {
      std::ifstream ifs{configFilePath_};
      auto data = nlohmann::json::parse(ifs);
      
    } catch (...) {
      ret = false;
    }
  }

 private:
  std::string configFilePath_;
};

class CSpdlogManager {
 private:
};

#endif  // SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_SPDLOG_MANAGER_H_
