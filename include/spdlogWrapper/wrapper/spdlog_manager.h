//
// Created by Kirito on 2022/7/20.
//

#ifndef SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_SPDLOG_MANAGER_H_
#define SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_SPDLOG_MANAGER_H_

#include <fstream>

#include "../nlohmannJson/json.hpp"
#include "../spdlog/spdlog.h"
#include "sink_manager.h"
#include "logger_manager.h"

namespace kiritou2 {
// make_unique support for pre c++14

#if __cplusplus >= 201402L // C++14 and beyond
using std::enable_if_t;
using std::make_unique;
#else
template<bool B, class T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args &&... args)
{
  static_assert(!std::is_array<T>::value, "arrays not supported");
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif
}

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
    bool ret = true;
    std::vector<spdlogsink::SinkInfo> sinkInfos;

    try {
      std::ifstream ifs{configFilePath_};
      auto data = nlohmann::json::parse(ifs);

      if (!data.contains("config")) {
        throw;
      }

      spdlogsink::SinkInfo skInfo;

      for (const auto &config : data["config"]) {
        if (!config.contains("enabled") || !config["enabled"]) {
          continue;
        }

        // Parse Sink type
        if (config.contains("sinkType")) {
          const std::string sinkType = config["sinkType"];
          if (sinkType == "basic") {
            skInfo.sink_type = spdlogsink::SinkType::SINK_TYPE_BASIC;
          } else if (sinkType == "once_file") {
            skInfo.sink_type = spdlogsink::SinkType::SINK_TYPE_ONCE_FILE;
          } else if (sinkType == "rotating") {
            skInfo.sink_type = spdlogsink::SinkType::SINK_TYPE_ROTATING;
          } else if (sinkType == "daily") {
            skInfo.sink_type = spdlogsink::SinkType::SINK_TYPE_DAILY;
          } else if (sinkType == "stdout") {
            skInfo.sink_type = spdlogsink::SinkType::SINK_TYPE_STDOUT;
          } else {
            ret = false;
            continue;
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
            continue;
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
        sinkInfos.emplace_back(skInfo);
      }
    } catch (...) {
      throw;
    }

    return {ret, sinkInfos};
  }

 private:
  std::string configFilePath_;
};

class CSpdlogManager {
 public:
  static CSpdlogManager &GetInstance() {
    static CSpdlogManager inst;
    return inst;
  }

  ~CSpdlogManager() {
    spdlog::drop_all();
  }

  CSpdlogManager(const CSpdlogManager &) = delete;
  CSpdlogManager &operator=(const CSpdlogManager &) = delete;
  CSpdlogManager(CSpdlogManager &&) = delete;
  CSpdlogManager &operator=(CSpdlogManager &&) = delete;

 private:
  CSpdlogManager() = default;

 public:
  void Initialize(const std::string &configFilePath = "") {
    try {
      std::call_once(once_, &CSpdlogManager::initialize, this, configFilePath);
    } catch (...) {
      std::cerr << "Spdlog manager initialize failed! \n";
      throw;
    }
  }

  template <typename... Args>
  void LogPrint(spdlogsink::spd_level level, const std::string &fmt, const Args &...args) {
    try {
      if (loggerManager_) {
        loggerManager_->Logger()->log(level, fmt, args...);
      }
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
    }
  }

 private:
  void initialize(const std::string &configFilePath) {
    std::vector<spdlogsink::SinkInfo> sinkInfos;

    if (!configFilePath.empty()) {
      configTransfer_ = kiritou2::make_unique<CJsonConfigTransfer>(configFilePath);
      auto transferResult = configTransfer_->TransferConfig();
      if (transferResult.first) {
        sinkInfos.swap(transferResult.second);
      } else {
        std::cerr << "Profile conversion failed \n";
        throw;
      }
    } else {
      spdlogsink::SinkInfo info;
      sinkInfos.emplace_back(info);
    }

    try {
      sinkManager_ = kiritou2::make_unique<spdlogsink::CSinksManager>();
      sinkManager_->CreateSinks(sinkInfos);
    } catch (...) {
      throw;
    }

    try {
      loggerManager_ = kiritou2::make_unique<CLoggerManager>();
      loggerManager_->CreateLogger(sinkManager_->Sinks());
    } catch (...) {
      throw;
    }
  }

 private:
  std::once_flag once_;

  std::unique_ptr<IConfigTransfer> configTransfer_;
  std::unique_ptr<spdlogsink::CSinksManager> sinkManager_;
  std::unique_ptr<CLoggerManager> loggerManager_;
};

#endif  // SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_SPDLOG_MANAGER_H_
