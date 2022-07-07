//
// Created by dell on 2022/6/27.
//

#ifndef SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_SINK_MANAGER_H_
#define SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_SINK_MANAGER_H_

#include <initializer_list>
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
  SinkType type            = SinkType::SINK_TYPE_ONCE_FILE;
  spd_level level          = spdlog::level::info;
  std::string path         = "./logs";
  std::size_t rotate_count = 1;
  std::size_t rotate_size  = 1024 * 1024 * 1;
  std::string pattern      = "%Y-%m-%d %H:%M:%S.%e %l [PID:%t] %v";
};

class ISinkManager {
 public:
  ISinkManager() = default;

  virtual ~ISinkManager()                       = default;
  ISinkManager(const ISinkManager &)            = delete;
  ISinkManager &operator=(const ISinkManager &) = delete;
  ISinkManager(ISinkManager &&)                 = delete;
  ISinkManager &operator=(ISinkManager &&)      = delete;

  virtual bool CreateSink() = 0;

  spdlog::sink_ptr Sink() { return sink_; }

  void SetOutputInfo(const SinkInfo &info) {
    if (sink_) {
      sink_->set_level(info.level);
      sink_->set_pattern(info.pattern);
    }
  }

 protected:
  spdlog::sink_ptr sink_;
};

class CBasicSinkManager : public ISinkManager {
 public:
  CBasicSinkManager(const std::string &filePath) : filePath_{filePath} {}

  bool CreateSink() override {
    try {
      sink_ = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filePath_);
      return true;
    } catch (const spdlog::spdlog_ex &ex) {
      std::cerr << ex.what() << std::endl;
      return false;
    }
  }

 private:
  std::string filePath_;
};

class CRotatingSinkManager : public ISinkManager {
 public:
  CRotatingSinkManager(const std::string &filePath, std::size_t singleFileSize, std::size_t rotatingCounts) : filePath_{filePath}, singleFileSize_{singleFileSize}, rotatingCounts_{rotatingCounts} {}

  bool CreateSink() override {
    try {
      sink_ = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(filePath_, singleFileSize_, rotatingCounts_);
      return true;
    } catch (const spdlog::spdlog_ex &ex) {
      std::cerr << ex.what() << std::endl;
      return false;
    }
  }

 private:
  std::string filePath_;
  std::size_t singleFileSize_ = 0;
  std::size_t rotatingCounts_ = 1;
};

class CDailySinkManager : public ISinkManager {
 public:
  CDailySinkManager() = default;

  bool CreateSink() override {
    try {
      sink_ = std::make_shared<spdlog::sinks::daily_file_sink_mt>();
    } catch (const spdlog::spdlog_ex &ex) {
      std::cerr << ex.what() << std::endl;
      return false;
    }
  }
};

class CStdoutColorSinkManager : public ISinkManager {
 public:
  CStdoutColorSinkManager() = default;

  bool CreateSink() override {
    try {
      sink_ = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      return true;
    } catch (const spdlog::spdlog_ex &ex) {
      std::cerr << ex.what() << std::endl;
      return false;
    }
  }
};

class CSinksManager {
  using sink_weak_ptr = std::weak_ptr<spdlog::sinks::sink>;

 public:
  CSinksManager(const CSinksManager &)            = delete;
  CSinksManager &operator=(const CSinksManager &) = delete;

  static CSinksManager &GetInstance() {
    static CSinksManager inst;
    return inst;
  }

  void CreateSinks(std::initializer_list<SinkInfo> infoList) {
    for (const auto &it : infoList) {
      createSink(it);
    }
  }

  const std::vector<spdlog::sink_ptr> &Sinks() const { return sinks_; }

 private:
  void createSink(const SinkInfo &info) {
    std::shared_ptr<ISinkManager> sinkManager;
    bool isSinkCreateSucceed = false;

    switch (info.type) {
      case SinkType::SINK_TYPE_BASIC: {
      } break;
      case SinkType::SINK_TYPE_ROTATING: {
        sinkManager = std::make_shared<CRotatingSinkManager>(info.path, info.rotate_size, info.rotate_count);
      } break;
      case SinkType::SINK_TYPE_DAILY: {
      } break;
      case SinkType::SINK_TYPE_STDOUT: {
        sinkManager = std::make_shared<CStdoutColorSinkManager>();
      } break;
      case SinkType::SINK_TYPE_ONCE_FILE: {
      } break;
      default:
        break;
    }

    if (sinkManager && sinkManager->CreateSink()) {
      sinkManager->SetOutputInfo(info);
      sinks_.emplace_back(sinkManager->Sink());
    }
  }

 private:
  CSinksManager() = default;

 private:
  std::vector<spdlog::sink_ptr> sinks_;
};
}  // namespace spdlogsink
#endif  // SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_SINK_MANAGER_H_