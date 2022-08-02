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
  IConfigTransfer() = default;

  virtual ~IConfigTransfer() = default;
  IConfigTransfer(const IConfigTransfer &) = delete;
  IConfigTransfer &operator=(const IConfigTransfer &) = delete;
  IConfigTransfer(IConfigTransfer &&) = delete;
  IConfigTransfer &operator=(IConfigTransfer &&) = delete;

  virtual std::pair<bool, std::vector<spdlogsink::SinkInfo>> TransferConfig() = 0;
};

class CJsonConfigTransfer : public IConfigTransfer {
 public:
  explicit CJsonConfigTransfer(std::string configFilePath)
      : configFilePath_(std::move(configFilePath)) {}

 public:
  std::pair<bool, std::vector<spdlogsink::SinkInfo>> TransferConfig() override {
    bool ret = false;
    std::vector<spdlogsink::SinkInfo> sinkInfos;

    try {
      std::ifstream ifs{configFilePath_};
      auto data = nlohmann::json::parse(ifs);
      if (data.contains("config")) {
        spdlogsink::SinkInfo skInfo;

        for (const auto &config : data["config"]) {
          if (!config.contains("enable") || !config["enable"]) {
            continue;
          }

          // Parse Sink type
          if (config.contains("sinkType")) {
            const std::string sinkType = config["sinkType"];
            if (sinkType == "basic") {
              skInfo.sink_type = spdlogsink::SinkType::SINK_TYPE_BASIC;
            } else if (sinkType == "once_file") {
              skInfo.sink_type = spdlogsink::SinkType::SINK_TYPE_ONCE_FILE;
            } else if (sinkType == "daily") {
              skInfo.sink_type = spdlogsink::SinkType::SINK_TYPE_DAILY;
            } else if (sinkType == "rotating") {
              skInfo.sink_type = spdlogsink::SinkType::SINK_TYPE_ROTATING;
            } else if (sinkType == "stdout") {
              skInfo.sink_type = spdlogsink::SinkType::SINK_TYPE_STDOUT;
            } else {
              ret = false;
            }
          }

          // Parse Log path
          if (config.contains("logPath")) {
            skInfo.file_path = config["logPath"];
          }

          // Parse sinkLevel
          if (config.contains("sinkLevel")) {
            const std::string sinkLevel = config["sinkLevel"];
            if (sinkLevel == "trace") {
              skInfo.sink_level = spdlogsink::spd_level::trace;
            } else if (sinkLevel == "debug") {
              skInfo.sink_level = spdlogsink::spd_level::debug;
            } else if (sinkLevel == "info") {
              skInfo.sink_level = spdlogsink::spd_level::info;
            } else if (sinkLevel == "warning") {
              skInfo.sink_level = spdlogsink::spd_level::warn;
            } else if (sinkLevel == "error") {
              skInfo.sink_level = spdlogsink::spd_level::err;
            } else if (sinkLevel == "critical") {
              skInfo.sink_level = spdlogsink::spd_level::critical;
            } else {
              ret = false;
            }
          }

          // Parse rotateCount
          if (config.contains("rotateCount")) {
            skInfo.rotate_count = config["rotateCount"];
          }

          // Parse rotateSize
          if (config.contains("rotateSize")) {
            skInfo.rotate_size = config["rotateSize"];
          }

          // Parse Pattern
          if (config.contains("pattern")) {
            skInfo.sink_pattern = config["pattern"];
          }
        }

        sinkInfos.emplace_back(skInfo);
      }

    } catch (...) {
      ret = false;
    }

    return {ret, sinkInfos};
  }

 private:
  std::string configFilePath_;
};

class CSpdlogManager {
 public:
  explicit CSpdlogManager(std::string configFilePath = "")
      : configFilePath_(std::move(configFilePath)) {}

 public:
  bool Initialize() {
    static std::once_flag once;
    bool ret = false;
    std::call_once(once, [this] { return false; });

    return ret;
  }

 private:
  bool initialize() {}

  bool initializeWithConfig() {}

 private:
  std::string configFilePath_;
  std::unique_ptr<IConfigTransfer> configTransfer_;
  std::unique_ptr<spdlogsink::CSinksManager> sinkManager_;
  std::unique_ptr<CLoggerManager> loggerManager_;
};

#endif  // SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_SPDLOG_MANAGER_H_
