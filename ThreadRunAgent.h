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

#include <mutex>

#include "VirtualThreadRun.h"
#include "ConcurrencyDeque.h"
#include "CompletableFuture.h"
#include "ThreadRunAgentConstant.h"

#define DllExport __declspec(dllexport)

 /**
  * @file: ThreadRunAgent.h
  * @brief: C++20
  *
  * @authors ThierrySquirrel
  * @date 2026/6/5
  **/

namespace JellyFish {
	class DllExport ThreadRunAgent {
	private:JellyFish::ConcurrencyDeque<JellyFish::VirtualThreadRun*>* containerAll;
	private:std::mutex* containerMutex;
	private:std::condition_variable* containerCondition;
	private:std::atomic<bool>* isDeleteAll;

	private:std::atomic<int>* threadSleepSize;
	private:JellyFish::CompletableFuture<bool>* threadAllStart;
	private:std::mutex* threadAllStartMutex;

	private:std::atomic<int>* threadAllStopSize;
	private:JellyFish::CompletableFuture<bool>* threadAllStop;

	public:ThreadRunAgent();
	public:ThreadRunAgent(JellyFish::ConcurrencyDeque<JellyFish::VirtualThreadRun*>* containerAll,
		std::mutex* containerMutex,
		std::condition_variable* containerCondition,
		std::atomic<bool>* isDeleteAll,
		std::atomic<int>* threadSleepSize,
		JellyFish::CompletableFuture<bool>* threadAllStart,
		std::mutex* threadAllStartMutex,
		std::atomic<int>* threadAllStopSize,
		JellyFish::CompletableFuture<bool>* threadAllStop);
	public:void agentRun();
	
	private:void lockAwait();
	private:void threadAllStoplockAwait();
	
	private:void call();

	private:void threadAllStopCall();
	};
}
