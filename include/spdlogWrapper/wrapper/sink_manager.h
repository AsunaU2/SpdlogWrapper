//
// Created by dell on 2022/6/27.
//

#ifndef SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_SINK_MANAGER_H_
#define SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_SINK_MANAGER_H_

#include <iostream>

#include "../nlohmannJson/json.hpp"
#include "../spdlog/async.h"
#include "../spdlog/common.h"
#include "../spdlog/sinks/basic_file_sink.h"
#include "../spdlog/sinks/daily_file_sink.h"
#include "../spdlog/sinks/rotating_file_sink.h"
#include "../spdlog/sinks/stdout_color_sinks.h"
#include "../spdlog/spdlog.h"

namespace spdlogsink {

using spd_level = spdlog::level::level_enum;

enum class SinkType {
  SINK_TYPE_BASIC,
  SINK_TYPE_ROTATING,
  SINK_TYPE_DAILY,
  SINK_TYPE_STDOUT,
  SINK_TYPE_ONCE_FILE
};

struct SinkInfo {
  SinkType    type         = SinkType::SINK_TYPE_ONCE_FILE;
  spd_level   level        = spdlog::level::info;
  std::string path         = "./logs";
  int         rotate_count = 1;
  uint64_t    rotate_size  = 1024 * 1024 * 1;
  std::string pattern      = "%Y-%m-%d %H:%M:%S.%e %l [PID:%t] %v";
};

class ISinkManager {
 public:
  ISinkManager()          = default;
  virtual ~ISinkManager() = default;

  ISinkManager(const ISinkManager &) = delete;
  ISinkManager &operator=(const ISinkManager &) = delete;
  ISinkManager(ISinkManager &&)                 = delete;
  ISinkManager &operator=(ISinkManager &&) = delete;

  virtual spdlog::sink_ptr CreateSink() = 0;
};

// sink of stdout type
class CStdoutColorSinkManager : public ISinkManager {
 public:
  CStdoutColorSinkManager() = default;

  spdlog::sink_ptr CreateSink() override {
    try {
      auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      return sink;
    } catch (const spdlog::spdlog_ex &ex) {
      std::cerr << ex.what() << std::endl;
      throw;
    }
  }
};

class CSinksManager {
  using sink_weak_ptr = std::weak_ptr<spdlog::sinks::sink>;

 public:
  CSinksManager(const CSinksManager &) = delete;
  CSinksManager &operator=(const CSinksManager &) = delete;

  static CSinksManager &GetInstance() {
    static CSinksManager inst;
    return inst;
  }

  void CreateSink(const SinkInfo &info) {
    spdlog::sink_ptr tmpSink;
    switch (info.type) {
      case SinkType::SINK_TYPE_BASIC: {
      } break;
      case SinkType::SINK_TYPE_ROTATING: {
      } break;
      case SinkType::SINK_TYPE_DAILY: {
      } break;
      case SinkType::SINK_TYPE_STDOUT: {
        tmpSink = CStdoutColorSinkManager();
      } break;
      case SinkType::SINK_TYPE_ONCE_FILE: {
      } break;
      default:
        break;
    }
  }

 private:
  CSinksManager() = default;

 private:
  std::vector<sink_weak_ptr> sinks_;
};
}  // namespace spdlogsink
#endif  // SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_SINK_MANAGER_H_