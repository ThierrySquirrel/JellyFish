/**
 * Copyright 2025/12/8 ThierrySquirrel
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

#define DllExport __declspec(dllexport)

 /**
  * @file: ThreadRunAgent.h
  * @brief: C++20
  *
  * @authors ThierrySquirrel
  * @date 2025/12/8
  **/

namespace JellyFish {
	class DllExport ThreadRunAgent {
	private:JellyFish::ConcurrencyDeque<JellyFish::VirtualThreadRun*>* containerAll;
	private:std::mutex* containerMutex;
	private:std::condition_variable* containerCondition;
	private:std::atomic<bool>* isDeleteAll;

	public:ThreadRunAgent();
	public:ThreadRunAgent(JellyFish::ConcurrencyDeque<JellyFish::VirtualThreadRun*>* containerAll,
		std::mutex* containerMutex,
		std::condition_variable* containerCondition,
		std::atomic<bool>* isDeleteAll);
	public:void agentRun();
	};
}
