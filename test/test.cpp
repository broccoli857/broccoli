#include <iostream>
#include "../broccoli/log.h"
#include "../broccoli/util.h"
#include "../broccoli/config.h"

broccoli::ConfigVar<int>::ptr g_int_value_config = broccoli::Config::Lookup("system.port", 8080, "system port");

int main() 
{
	broccoli::Logger::ptr logger(new broccoli::Logger);

	// ����������д���
	logger->addAppender(broccoli::LogAppender::ptr(new broccoli::StdoutLogAppender));

	// ������ļ� 
	broccoli::FileLogAppender::ptr file_appender(new broccoli::FileLogAppender("./log.txt"));
	logger->addAppender(file_appender);


	std::cout << "hello broccoli log !" << std::endl;

	BROCCOLI_LOG_DEBUG(logger) << "hello broccoli log";
	BROCCOLI_LOG_ERROR(logger) << "hello broccoli log error";

	BROCCOLI_LOG_FMT_ERROR(logger, "you are %s", "aaaaa");

	auto l = broccoli::loggerMgr::GetInstance()->getLogger("XX");
	BROCCOLI_LOG_INFO(logger) << "xxxx";

	BROCCOLI_LOG_INFO(logger) << g_int_value_config->getValue();
	BROCCOLI_LOG_INFO(logger) << g_int_value_config->toString();

	return 0;
}