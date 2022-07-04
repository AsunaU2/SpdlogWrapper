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

enum class SinkType {
  SINK_TYPE_BASIC,
  SINK_TYPE_ROTATING,
  SINK_TYPE_DAILY,
  SINK_TYPE_STDOUT,
  SINK_TYPE_ONCE_FILE
};

struct SinkInfo {
  SinkType    type;
  std::string level;
  std::string path;
  int         rotate_count;
  uint64_t    rotate_size;
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

  spdlog::sink_ptr Sink() { return sink_; }

 protected:
  spdlog::sink_ptr          sink_;
  std::string               pattern_ = "%Y-%m-%d %H:%M:%S.%e %l [PID:%t] %v";
  spdlog::level::level_enum level_   = spdlog::level::info;
};

// sink of stdout type
class CStdoutColorSinkManager : public ISinkManager {
 public:
  CStdoutColorSinkManager() {
    sink_->set_pattern(pattern_);
    sink_->set_level(level_);
  }
  explicit CStdoutColorSinkManager(spdlog::level::level_enum level, const std::string &pattern = "") : CStdoutColorSinkManager() {
    sink_->set_level(level);
    if (!pattern.empty()) {
      sink_->set_pattern(pattern);
    }
  }

  spdlog::sink_ptr CreateSink() override {
    try {
      sink_ = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      return sink_;
    } catch (const spdlog::spdlog_ex &ex) {
      std::cerr << ex.what() << std::endl;
    }
  }
};

class CSinksManager {
 public:
  CSinksManager(const CSinksManager &) = delete;
  CSinksManager &operator=(const CSinksManager &) = delete;

  static CSinksManager &GetInstance() {
    static CSinksManager inst;
    return inst;
  }

 private:
  CSinksManager() = default;

 private:
  std::vector<spdlog::sink_ptr> sinks_;
};

#endif  // SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_SINK_MANAGER_H_