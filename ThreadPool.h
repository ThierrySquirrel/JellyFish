/**
 * Copyright 2026/6/5 ThierrySquirrel
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#pragma once

#include <functional>

#include "ThreadRunAgent.h"
#include "ThreadPoolConstant.h"

#define DllExport __declspec(dllexport)

 /**
  * @file: ThreadPool.h
  * @brief: C++20
  *
  * @authors ThierrySquirrel
  * @date 2026/6/5
  **/

namespace JellyFish {
	class DllExport ThreadPool {
	private:int maxThreadSize;
	private:JellyFish::ConcurrencyDeque<JellyFish::VirtualThreadRun*> containerAll;
	private:std::mutex containerMutex;
	private:std::condition_variable containerCondition;
	private:std::atomic<bool> isDeleteAll;

	private:std::atomic<int> threadSleepSize;
	private:JellyFish::CompletableFuture<bool> threadAllStart;
	private:std::mutex threadAllStartMutex;

	private:std::atomic<int> threadAllStopSize;
	private:JellyFish::CompletableFuture<bool> threadAllStop;

	public:ThreadPool();
	public:ThreadPool(int& maxThreadSize);
	public:~ThreadPool();
	public:void execute(VirtualThreadRun* virtualThreadRun);

	private:void setMaxThreadSize(int& maxThreadSize);
	private:int getMaxThreadSize();

	private:void init(int& maxThreadSize);
	private:void deleteAll();
	};
}

