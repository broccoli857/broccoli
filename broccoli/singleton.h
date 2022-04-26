/*
 * @Descripttion: 
 * @version: 
 * @Author: cheng
 * @Date: 2022-03-25 20:38:56
 * @LastEditors: cheng
 * @LastEditTime: 2022-04-26 21:43:33
 */
#pragma once

#include <memory>

namespace broccoli {

	template<class T, class X=void, int N=0>
	class Singleton {
	public:
		static T* GetInstance() {
			static T v;
			return &v;
		}
	};

	template<class T, class X=void, int N=0>
	class SingletonPtr {
	public:
		static std::shared_ptr<T> GetInstance() {
			std::shared_ptr<T> v(new T);
			return v;
		}
	};

}