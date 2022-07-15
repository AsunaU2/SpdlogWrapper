//
// Created by AsunaU2 on 2022/7/12.
//

#ifndef SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_LOGGER_MANAGER_H_
#define SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_LOGGER_MANAGER_H_

#include "../spdlog/logger.h"
#include "sink_manager.h"

class CLoggerManager {
 public:
  ~CLoggerManager() = default;
  CLoggerManager(const CLoggerManager &) = delete;
  CLoggerManager &operator=(const CLoggerManager &) = delete;
  CLoggerManager(CLoggerManager &&) noexcept = delete;
  CLoggerManager &operator=(CLoggerManager &&) noexcept = delete;

  static CLoggerManager &GetInstance() {
    static CLoggerManager inst{};
    return inst;
  }

  bool CreateLogger(const std::vector<spdlog::sink_ptr> &sinks) {
    bool ret = false;

    try {
      if (!sinks.empty()) {
        logger_ = std::make_shared<spdlog::logger>("Logger", sinks.begin(), sinks.end());
        logger_->set_level(spdlog::level::trace);
        logger_->flush_on(spdlog::level::trace);
        ret = true;
      }
    } catch (const spdlog::spdlog_ex &ex) {
      std::cerr << ex.what() << std::endl;
    }
    return ret;
  }

  std::shared_ptr<spdlog::logger> Logger() { return logger_; }

 private:
  CLoggerManager() = default;

 private:
  std::shared_ptr<spdlog::logger> logger_;
};

#endif  // SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_LOGGER_MANAGER_H_
