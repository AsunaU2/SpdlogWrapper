//
// Created by AsunaU2 on 2022/6/27.
//

#ifndef SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_SPDLOG_API_H_
#define SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_SPDLOG_API_H_

#include "spdlog_manager.h"

// For get code file name
#if (defined(_WIN32) || defined(_WIN64))
#define FILENAME_ (strrchr(__FILE__, '\\') ? (strrchr(__FILE__, '\\') + 1) : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : __FILE__)
#endif

#define LOG_INIT_(x) CSpdlogManager::GetInstance().Initialize(x)

#define SPDLOGMANAGER_LOG_PRINT CSpdlogManager::GetInstance().LogPrint
#if (defined(_WIN32) || defined(_WIN64))
#define LOGT(fmt, ...) SPDLOGMANAGER_LOG_PRINT(spdlogsink::spd_level::trace, "{}:{} {} " fmt, FILENAME_, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOGD(fmt, ...) SPDLOGMANAGER_LOG_PRINT(spdlogsink::spd_level::debug, "{}:{} {} " fmt, FILENAME_, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOGI(fmt, ...) SPDLOGMANAGER_LOG_PRINT(spdlogsink::spd_level::info, "{}:{} {} " fmt, FILENAME_, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOGW(fmt, ...) SPDLOGMANAGER_LOG_PRINT(spdlogsink::spd_level::warn, "{}:{} {} " fmt, FILENAME_, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOGE(fmt, ...) SPDLOGMANAGER_LOG_PRINT(spdlogsink::spd_level::err, "{}:{} {} " fmt, FILENAME_, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOGC(fmt, ...) SPDLOGMANAGER_LOG_PRINT(spdlogsink::spd_level::critical, "{}:{} {} " fmt, FILENAME_, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define LOGT(fmt, args...) SPDLOGMANAGER_LOG_PRINT(spdlogsink::spd_level::trace, "{}:{} {} " fmt, FILENAME_, __LINE__, __FUNCTION__, ##args)
#define LOGD(fmt, args...) SPDLOGMANAGER_LOG_PRINT(spdlogsink::spd_level::debug, "{}:{} {} " fmt, FILENAME_, __LINE__, __FUNCTION__, ##args)
#define LOGI(fmt, args...) SPDLOGMANAGER_LOG_PRINT(spdlogsink::spd_level::info, "{}:{} {} " fmt, FILENAME_, __LINE__, __FUNCTION__, ##args)
#define LOGW(fmt, args...) SPDLOGMANAGER_LOG_PRINT(spdlogsink::spd_level::warn, "{}:{} {} " fmt, FILENAME_, __LINE__, __FUNCTION__, ##args)
#define LOGE(fmt, args...) SPDLOGMANAGER_LOG_PRINT(spdlogsink::spd_level::err, "{}:{} {} " fmt, FILENAME_, __LINE__, __FUNCTION__, ##args)
#define LOGC(fmt, args...) SPDLOGMANAGER_LOG_PRINT(spdlogsink::spd_level::critical, "{}:{} {} " fmt, FILENAME_, __LINE__, __FUNCTION__, ##args)
#endif

#endif  // SPDLOGWRAPPER_INCLUDE_SPDLOGWRAPPER_SPDLOG_API_H_
