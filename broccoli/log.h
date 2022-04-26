/*
 * @Descripttion: 
 * @version: 
 * @Author: cheng
 * @Date: 2022-04-26 21:42:29
 * @LastEditors: cheng
 * @LastEditTime: 2022-04-26 21:51:19
 */
#pragma once

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdarg.h>
#include <map>
#include "singleton.h"
#include "util.h"

#define BROCCOLI_LOG_LEVEL(logger, level) \
	if(logger->getLevel() <= level) \
		broccoli::LogEventWrap(broccoli::LogEvent::ptr(new broccoli::LogEvent(logger, level, \
							__FILE__, __LINE__, 0, broccoli::GetThreadId(), \
							broccoli::GetFiberId(), time(0)))).getSS()

#define BROCCOLI_LOG_DEBUG(logger) BROCCOLI_LOG_LEVEL(logger, broccoli::LogLevel::DEBUG)
#define BROCCOLI_LOG_INFO(logger) BROCCOLI_LOG_LEVEL(logger, broccoli::LogLevel::INFO)
#define BROCCOLI_LOG_WARN(logger) BROCCOLI_LOG_LEVEL(logger, broccoli::LogLevel::WARN)
#define BROCCOLI_LOG_ERROR(logger) BROCCOLI_LOG_LEVEL(logger, broccoli::LogLevel::ERROR)
#define BROCCOLI_LOG_FATAL(logger) BROCCOLI_LOG_LEVEL(logger, broccoli::LogLevel::FATAL)

#define BROCCOLI_LOG_FMT_LEVEL(logger, level, fmt, ...) \
	if(logger->getLevel() <= level) \
		broccoli::LogEventWrap(broccoli::LogEvent::ptr(new broccoli::LogEvent(logger, level, \
							__FILE__, __LINE__, 0, broccoli::GetThreadId(), \
							broccoli::GetFiberId(), time(0)))).getEvent()->format(fmt, __VA_ARGS__)

#define BROCCOLI_LOG_FMT_DEBUG(logger, fmt, ...) BROCCOLI_LOG_FMT_LEVEL(logger, broccoli::LogLevel::DEBUG, fmt, __VA_ARGS__)
#define BROCCOLI_LOG_FMT_INFO(logger, fmt, ...) BROCCOLI_LOG_FMT_LEVEL(logger, broccoli::LogLevel::INFO, fmt, __VA_ARGS__)
#define BROCCOLI_LOG_FMT_WARN(logger, fmt, ...) BROCCOLI_LOG_FMT_LEVEL(logger, broccoli::LogLevel::WARN, fmt, __VA_ARGS__)
#define BROCCOLI_LOG_FMT_ERROR(logger, fmt, ...) BROCCOLI_LOG_FMT_LEVEL(logger, broccoli::LogLevel::ERROR, fmt, __VA_ARGS__)
#define BROCCOLI_LOG_FMT_FATAL(logger, fmt, ...) BROCCOLI_LOG_FMT_LEVEL(logger, broccoli::LogLevel::FATAL, fmt, __VA_ARGS__)

#define BROCCOLI_LOG_ROOT() broccoli::loggerMgr::GetInstance()->getRoot()
#define BROCCOLI_LOG_NAME(name) broccoli::loggerMgr::GetInstance()->getLogger(name);

namespace broccoli {

class Logger;
class LoggerManager;

// 日志级别
class LogLevel {
public:
	enum Level
	{
		UNKNOW = 0,
		DEBUG,
		INFO,
		WARN,
		ERROR,
		FATAL
	};

	static const char* ToString(LogLevel::Level level);
	static LogLevel::Level FromString(const std::string& str);
};

// 日志事件
class LogEvent {
public:
	typedef std::shared_ptr<LogEvent> ptr;
	LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level,
			const char* file, int32_t line, uint32_t elapse, 
			uint32_t thread_id, uint32_t fiber_id, uint64_t time);

	const char* getFile() const { return m_file; }
	int32_t getLine() const { return m_line; }
	uint32_t getElapse() const { return m_elapse; }
	uint32_t getThreadId() const { return m_threadId; }
	uint32_t getFiberId() const { return m_fiberId; }
	uint64_t getTime() const { return m_time; }
	std::string getContent() const { return m_ss.str(); }
	LogLevel::Level getLevel() const { return m_level; }
	std::shared_ptr<Logger> getLogger() const { return m_logger; }
	std::stringstream& getSS() { return m_ss; }

	void format(const char* fmt, ...);
	void format(const char* fmt, va_list al);
private:
	const char* m_file = nullptr;	// 文件名
	int32_t m_line = 0;				// 行号
	uint32_t m_elapse = 0;			// 程序启动开始到现在的毫秒
	uint32_t m_threadId = 0;		// 线程id
	uint32_t m_fiberId = 0;			// 协程id
	uint64_t m_time = 0;			// 时间戳
	std::stringstream  m_ss;

	std::shared_ptr<Logger> m_logger;
	LogLevel::Level m_level;
};

class LogEventWrap {
public:
	LogEventWrap(LogEvent::ptr e);
	~LogEventWrap();

	LogEvent::ptr getEvent() const { return m_event; }

	std::stringstream& getSS();
private:
	LogEvent::ptr m_event;
};

// 日志格式器
class LogFormatter {
public:
	typedef std::shared_ptr<LogFormatter> ptr;
	LogFormatter(const std::string& pattern);

	std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
// private:
public:
	class FormatItem {
	public:
		typedef std::shared_ptr<FormatItem> ptr;
		virtual ~FormatItem() {}
		virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
	};

	/**
	 * @brief: 解析 m_pattern 设置格式，解析成格式对应对象，存储到 m_items 数组中
	 * @param {*}
	 * @return {*}
	 */	
	void init();

	bool isError() const {return m_error;}
	const std::string getPattern() const {return m_pattern;}
private:
	std::string m_pattern;
	std::vector<FormatItem::ptr> m_items;
	bool m_error = false;
};

// 日志输出器
class LogAppender {
public:
	typedef std::shared_ptr<LogAppender> ptr;
	virtual ~LogAppender() {}

	virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

	virtual std::string toYamlString() = 0;

	void setFormatter(LogFormatter::ptr val);
	LogFormatter::ptr getFormatter() const { return m_formatter; }

	LogLevel::Level getLevel() const { return m_level; }
	void setLevel(LogLevel::Level val) { m_level = val; }
protected:
	LogLevel::Level m_level = LogLevel::DEBUG;
	LogFormatter::ptr m_formatter;
	bool m_hasFormatter = false;
};

// 日志器
class Logger :public std::enable_shared_from_this<Logger> {
	friend class LoggerManager;
public:
	typedef std::shared_ptr<Logger> ptr;

	Logger(const std::string& name = "root");
	void log(LogLevel::Level level, LogEvent::ptr event);

	void debug(LogEvent::ptr event);
	void info(LogEvent::ptr event);
	void warn(LogEvent::ptr event);
	void error(LogEvent::ptr event);
	void fatal(LogEvent::ptr event);

	void addAppender(LogAppender::ptr appender);
	void delAppender(LogAppender::ptr appender);

	void clearAppenders();

	void setLevel(LogLevel::Level val) { m_level = val; }
	LogLevel::Level getLevel() const{ return m_level; }

	std::string getName() const { return m_name; }

	void setFormatter(LogFormatter::ptr val);
	void setFormatter(const std::string& val);
	LogFormatter::ptr getFormatter();

	std::string toYamlString();
private:
	std::string m_name;
	LogLevel::Level m_level;				// 日志等级
	std::list<LogAppender::ptr> m_appenders;
	LogFormatter::ptr m_formatter;			// 日志格式器
	Logger::ptr m_root;
};

// 输出到控制台的 Appender
class StdoutLogAppender : public LogAppender {
public:
	typedef std::shared_ptr<StdoutLogAppender> ptr;
	void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;

	std::string toYamlString() override;
private:
};

// 输出到文件的 Appender
class FileLogAppender : public LogAppender {
public:
	typedef std::shared_ptr<FileLogAppender> ptr;
	FileLogAppender(const std::string filename);

	bool reopen();

	void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;

	std::string toYamlString() override;
private:
	std::string m_filename;
	std::ofstream m_filestream;
	Logger::ptr m_root;
};

class LoggerManager {
public:
	LoggerManager();
	Logger::ptr getLogger(const std::string& name);
	Logger::ptr getRoot() const { return m_root; }

	std::string toYamlString();

	void init();
private:
	std::map<std::string, Logger::ptr> m_loggers;
	Logger::ptr m_root;
};

typedef broccoli::Singleton<LoggerManager> loggerMgr;
}
