#include <iostream>
#include "../broccoli/log.h"
#include "../broccoli/util.h"

int main() 
{
	broccoli::Logger::ptr logger(new broccoli::Logger);

	logger->addAppender(broccoli::LogAppender::ptr(new broccoli::StdoutLogAppender));

	broccoli::FileLogAppender::ptr file_appender(new broccoli::FileLogAppender("./log.txt"));
	logger->addAppender(file_appender);

	std::cout << "hello broccoli log !" << std::endl;

	BROCCOLI_LOG_DEBUG(logger) << "hello broccoli log";
	BROCCOLI_LOG_ERROR(logger) << "hello broccoli log error";

	BROCCOLI_LOG_FMT_ERROR(logger, "you are %s", "aaaaa");

	auto l = broccoli::loggerMgr::GetInstance()->getLogger("XX");
	BROCCOLI_LOG_INFO(logger) << "xxxx";

	return 0;
}