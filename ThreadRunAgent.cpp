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

#include "pch.h"
#include "ThreadRunAgent.h"

 /**
  * @file: ThreadRunAgent.cpp
  * @brief: C++20
  *
  * @authors ThierrySquirrel
  * @date 2025/12/8
  **/

JellyFish::ThreadRunAgent::ThreadRunAgent() {}
JellyFish::ThreadRunAgent::ThreadRunAgent(JellyFish::ConcurrencyDeque<JellyFish::VirtualThreadRun*>* containerAll,
	std::mutex* containerMutex,
	std::condition_variable* containerCondition,
	std::atomic<bool>* isDeleteAll) {
	this->containerAll = containerAll;
	this->containerMutex = containerMutex;
	this->containerCondition = containerCondition;
	this->isDeleteAll = isDeleteAll;
}
void JellyFish::ThreadRunAgent::agentRun() {
	bool deleteAll = isDeleteAll->load();
	while (!deleteAll) {
		JellyFish::BaseContainer<JellyFish::VirtualThreadRun*> threadRun = containerAll->tryPopBack();
		bool empty = threadRun.getIsEmpty();
		if (empty) {
			std::unique_lock<std::mutex> containerLock(*containerMutex);
			containerCondition->wait(containerLock);
			deleteAll = isDeleteAll->load();
			continue;
		}

		JellyFish::VirtualThreadRun* value = threadRun.getValue();
		value->run();

		delete value;
		value = nullptr;

		deleteAll = isDeleteAll->load();
	}
}
