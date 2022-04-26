/*
 * @Descripttion: 
 * @version: 
 * @Author: cheng
 * @Date: 2022-04-26 20:27:31
 * @LastEditors: cheng
 * @LastEditTime: 2022-04-26 21:43:43
 */
#pragma once

#include <cxxabi.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdint.h>

namespace broccoli {

pid_t GetThreadId();
uint32_t GetFiberId();

template<class T>
const char* TypeToName() {
    static const char* s_name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
    return s_name;
}

}