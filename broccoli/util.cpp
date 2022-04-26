/*
 * @Descripttion: 
 * @version: 
 * @Author: cheng
 * @Date: 2022-03-25 20:38:56
 * @LastEditors: cheng
 * @LastEditTime: 2022-04-26 21:43:39
 */
#include "util.h"

namespace broccoli {
	pid_t GetThreadId() {
		return syscall(SYS_gettid);
	}

	uint32_t GetFiberId() {
		return 0;
	}
}