#include <iostream>
#include "../broccoli/log.h"
#include "../broccoli/util.h"

int main() 
{
	broccoli::Logger::ptr logger(new broccoli::Logger);
	logger->addAppender(broccoli::LogAppender::ptr(new broccoli::StdoutLogAppender));

	//broccoli::LogEvent::ptr event(new broccoli::LogEvent(__FILE__, __LINE__, 0, broccoli::GetThreadId(),
		//broccoli::GetFiberId(), time(0)));
	//event->getSS() << "hello broccoli log";

	//logger->log(broccoli::LogLevel::DEBUG, event);
	std::cout << "hello broccoli log !" << std::endl;

	BROCCOLI_LOG_DEBUG(logger) << "hello broccoli log";
	BROCCOLI_LOG_ERROR(logger) << "hello broccoli log error";

	return 0;
}