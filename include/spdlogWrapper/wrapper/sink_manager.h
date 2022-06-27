//
// Created by dell on 2022/6/27.
//

#ifndef SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_SINK_MANAGER_H_
#define SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_SINK_MANAGER_H_

#include "../spdlog/spdlog.h"
#include "../spdlog/sinks/basic_file_sink.h"
#include "../spdlog/sinks/rotating_file_sink.h"
#include "../spdlog/sinks/daily_file_sink.h"
#include "../spdlog/sinks/stdout_color_sinks.h"
#include "../spdlog/common.h"
#include "../spdlog/async.h"
#include "../nlohmannJson/json.hpp"

#include <iostream>

enum class SinkType {
  SINK_TYPE_BASIC,
  SINK_TYPE_ROTATING,
  SINK_TYPE_DAILY,
  SINK_TYPE_STDOUT,
  SINK_TYPE_ONCE_FILE
};

class ISinkManager {
 public:
  ISinkManager()  = default;
  virtual ~ISinkManager() = default;

  ISinkManager(const ISinkManager&) = delete;
  ISinkManager& operator=(const ISinkManager&) = delete;
  ISinkManager(ISinkManager&&) = delete;
  ISinkManager& operator=(ISinkManager&&) = delete;

  spdlog::sink_ptr Sink() {
	return sink_;
  }

 protected:
  spdlog::sink_ptr sink_;
  std::string pattern_ = "%Y-%m-%d %H:%M:%S.%e %l [PID:%t] %v";
  spdlog::level::level_enum level_ = spdlog::level::info;
};

class CStdoutColorSinkManager : public ISinkManager {
 public:
  CStdoutColorSinkManager() {
	try {
	  sink_ = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	} catch (const spdlog::spdlog_ex& ex) {
	  std::cerr << ex.what() << std::endl;
	}

	sink_->set_pattern(pattern_);
	sink_->set_level(level_);
  }
  explicit CStdoutColorSinkManager(spdlog::level::level_enum level, const std::string& pattern = "") : CStdoutColorSinkManager() {
	sink_->set_level(level);
	if (!pattern.empty()) {
		sink_->set_pattern(pattern);
	}
  }
};

class AbstractSinksManager {
	AbstractSinksManager() {

	}

	virtual ~AbstractSinksManager(){}

 protected:
  std::vector<spdlog::sink_ptr> sinks_;
};

#endif //SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_WRAPPER_SINK_MANAGER_H_
