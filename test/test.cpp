#include <iostream>
#include "../broccoli/log.h"

int main() 
{
	broccoli::Logger::ptr logger(new broccoli::Logger);
	logger->addAppender(broccoli::LogAppender::ptr(new broccoli::StdoutLogAppender));

	broccoli::LogEvent::ptr event(new broccoli::LogEvent(__FILE__, __LINE__, 0, 1, 2, time(0)));
	event->getSS() << "hello broccoli log";

	logger->log(broccoli::LogLevel::DEBUG, event);
	std::cout << "hello broccoli log !" << std::endl;

	return 0;
}