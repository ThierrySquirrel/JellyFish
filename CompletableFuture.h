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

#include "BaseContainerBuild.h"

#define DllExport __declspec(dllexport)

 /**
  * @file: CompletableFuture.h
  * @brief: C++20
  *
  * @authors ThierrySquirrel
  * @date 2025/12/8
  **/

namespace JellyFish {
	template <typename V>
	class DllExport CompletableFuture {
	private:std::atomic<bool> isComplete;
	private:V value;
	private:std::condition_variable timeoutCondition;
	private:std::mutex timeoutConditionMutex;
	
	public:CompletableFuture<V>() {
		isComplete.store(false);
	}

	  
	public:void tryOneComplete(V& value) {
			this->value = value;
			isComplete.store(true);
			timeoutCondition.notify_one();
	}

	public:JellyFish::BaseContainer<V> tryOneGet(int& milliseconds, int& maxTryCount) {
		bool complete;
		for (int i = 0; i < maxTryCount; i++) {

			complete = this->isComplete.load();
			if (complete) {
				return JellyFish::BaseContainerBuild<V>::buildSuccess(this->value);
			}

			std::unique_lock<std::mutex> lock(timeoutConditionMutex);
			std::cv_status status = timeoutCondition.wait_for(lock, std::chrono::milliseconds(milliseconds));

			complete = this->isComplete.load();
			if (complete) {
				return JellyFish::BaseContainerBuild<V>::buildSuccess(this->value);
			}
		}

		return JellyFish::BaseContainerBuild<V>::buildFail();
	}

	};
}