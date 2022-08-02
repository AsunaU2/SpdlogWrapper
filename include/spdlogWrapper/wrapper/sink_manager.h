//
// Created by AsunaU2 on 2022/6/27.
//

#ifndef SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_SINK_MANAGER_H_
#define SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_SINK_MANAGER_H_

#include <initializer_list>
#include <iostream>
#include <utility>

#include "../spdlog/common.h"
#include "../spdlog/sinks/basic_file_sink.h"
#include "../spdlog/sinks/daily_file_sink.h"
#include "../spdlog/sinks/rotating_file_sink.h"
#include "../spdlog/sinks/stdout_color_sinks.h"

namespace spdlogsink {

using spd_level = spdlog::level::level_enum;

enum class SinkType {
  SINK_TYPE_BASIC = 0,
  SINK_TYPE_ONCE_FILE,
  SINK_TYPE_ROTATING,
  SINK_TYPE_DAILY,
  SINK_TYPE_STDOUT
};

struct SinkInfo {
  explicit SinkInfo(SinkType type = SinkType::SINK_TYPE_ONCE_FILE, std::string path = "./logs/log.log", spd_level level = spdlog::level::info, std::size_t rotateCount = 1,
                    std::size_t rotateSize = 1024 * 1024 * 1, std::string pattern = "%Y-%m-%d %H:%M:%S.%e %l [PID:%t] %v")
      : sink_type(type)
      , file_path(std::move(path))
      , sink_level(level)
      , rotate_count(rotateCount)
      , rotate_size(rotateSize)
      , sink_pattern(std::move(pattern)) {}

  SinkType sink_type;
  spd_level sink_level;
  std::string file_path;
  std::size_t rotate_count;
  std::size_t rotate_size;
  std::string sink_pattern;
};

class ISinkFactory {
 public:
  ISinkFactory() = default;

  virtual ~ISinkFactory() = default;
  ISinkFactory(const ISinkFactory &) = delete;
  ISinkFactory &operator=(const ISinkFactory &) = delete;
  ISinkFactory(ISinkFactory &&) = delete;
  ISinkFactory &operator=(ISinkFactory &&) = delete;

  virtual bool CreateSink() = 0;

  spdlog::sink_ptr Sink() { return sink_; }

  void SetOutputInfo(const SinkInfo &info) {
    if (sink_) {
      sink_->set_level(info.sink_level);
      sink_->set_pattern(info.sink_pattern);
    }
  }

 protected:
  spdlog::sink_ptr sink_;
};

class CBasicSinkFactory : public ISinkFactory {
 public:
  explicit CBasicSinkFactory(std::string filePath)
      : filePath_(std::move(filePath)) {}

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

class COnceFileSinkFactory : public ISinkFactory {
 public:
  explicit COnceFileSinkFactory(std::string preFilePath)
      : preFilePath_(std::move(preFilePath)) {}

  bool CreateSink() override {
    bool ret = false;

    do {
      basicSinkFactory_ = std::make_shared<CBasicSinkFactory>(preFilePath_.append("/").append(generateFilePath()).append(".log"));
      if (!basicSinkFactory_) {
        break;
      }

      ret = basicSinkFactory_->CreateSink();
    } while (false);

    if (ret) {
      sink_ = basicSinkFactory_->Sink();
    }
    return ret;
  }

 private:
  static std::string generateFilePath() { return GetCurrentTime(); }

  static std::string GetCurrentTime() {
    std::string strTime;
    std::time_t now = std::time(nullptr);
    std::tm p{};
#ifdef _WIN32
    localtime_s(&p, &now);
#else
    localtime_r(&now, &p);
#endif  // _WIN32

    std::string year = std::to_string(1900 + p.tm_year);
    std::string month = std::to_string(1 + p.tm_mon);
    std::string day = std::to_string(p.tm_mday);
    std::string hour = std::to_string(p.tm_hour);
    std::string minute = std::to_string(p.tm_min);
    std::string second = std::to_string(p.tm_sec);

    strTime.append(year);
    strTime.append("-");
    strTime.append(month);
    strTime.append("-");
    strTime.append(day);
    strTime.append("-");
    strTime.append(hour);
    strTime.append("-");
    strTime.append(minute);
    strTime.append("-");
    strTime.append(second);

    return strTime;
  }

 private:
  std::shared_ptr<CBasicSinkFactory> basicSinkFactory_;
  std::string preFilePath_;
};

class CRotatingSinkFactory : public ISinkFactory {
 public:
  CRotatingSinkFactory(std::string filePath, std::size_t singleFileSize, std::size_t rotatingCounts)
      : filePath_(std::move(filePath))
      , singleFileSize_(singleFileSize)
      , rotatingCounts_(rotatingCounts) {}

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

class CDailySinkFactory : public ISinkFactory {
 public:
  CDailySinkFactory(std::string filePath, int hours, int minutes)
      : filePath_(std::move(filePath))
      , hours_(hours)
      , minutes_(minutes) {}

  bool CreateSink() override {
    try {
      sink_ = std::make_shared<spdlog::sinks::daily_file_sink_mt>(filePath_, hours_, minutes_);
      return true;
    } catch (const spdlog::spdlog_ex &ex) {
      std::cerr << ex.what() << std::endl;
      return false;
    }
  }

 private:
  std::string filePath_;
  int hours_ = 0;
  int minutes_ = 0;
};

class CStdoutColorSinkFactory : public ISinkFactory {
 public:
  CStdoutColorSinkFactory() = default;

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
 public:
  CSinksManager() = default;
  ~CSinksManager() = default;
  CSinksManager(const CSinksManager &) = delete;
  CSinksManager &operator=(const CSinksManager &) = delete;
  CSinksManager(CSinksManager &&) noexcept = delete;
  CSinksManager &operator=(CSinksManager &&) noexcept = delete;

  void CreateSinks(std::initializer_list<SinkInfo> infoList) {
    for (const auto &it : infoList) {
      createSink(it);
    }
  }

  const std::vector<spdlog::sink_ptr> &Sinks() const { return sinks_; }

 private:
  void createSink(const SinkInfo &info) {
    std::shared_ptr<ISinkFactory> sinkFactory;

    switch (info.sink_type) {
      case SinkType::SINK_TYPE_BASIC: {
        sinkFactory = std::make_shared<CBasicSinkFactory>(info.file_path);
      } break;
      case SinkType::SINK_TYPE_ROTATING: {
        sinkFactory = std::make_shared<CRotatingSinkFactory>(info.file_path, info.rotate_size, info.rotate_count);
      } break;
      case SinkType::SINK_TYPE_DAILY: {
        sinkFactory = std::make_shared<CDailySinkFactory>(info.file_path, 8, 0);
      } break;
      case SinkType::SINK_TYPE_STDOUT: {
        sinkFactory = std::make_shared<CStdoutColorSinkFactory>();
      } break;
      case SinkType::SINK_TYPE_ONCE_FILE: {
        sinkFactory = std::make_shared<COnceFileSinkFactory>(info.file_path);
      } break;
      default:
        break;
    }

    if (sinkFactory && sinkFactory->CreateSink()) {
      sinkFactory->SetOutputInfo(info);
      sinks_.emplace_back(sinkFactory->Sink());
    }
  }

 private:
  std::vector<spdlog::sink_ptr> sinks_;
};
}  // namespace spdlogsink
#endif  // SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_SINK_MANAGER_H_