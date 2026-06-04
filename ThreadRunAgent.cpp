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

#include "pch.h"
#include "ThreadRunAgent.h"

 /**
  * @file: ThreadRunAgent.cpp
  * @brief: C++20
  *
  * @authors ThierrySquirrel
  * @date 2026/6/5
  **/

JellyFish::ThreadRunAgent::ThreadRunAgent() {}
JellyFish::ThreadRunAgent::ThreadRunAgent(JellyFish::ConcurrencyDeque<JellyFish::VirtualThreadRun*>* containerAll,
	std::mutex* containerMutex,
	std::condition_variable* containerCondition,
	std::atomic<bool>* isDeleteAll,
	std::atomic<int>* threadSleepSize,
	JellyFish::CompletableFuture<bool>* threadAllStart,
	std::mutex* threadAllStartMutex) {
	this->containerAll = containerAll;
	this->containerMutex = containerMutex;
	this->containerCondition = containerCondition;
	this->isDeleteAll = isDeleteAll;
	this->threadSleepSize = threadSleepSize;
	this->threadAllStart = threadAllStart;
	this->threadAllStartMutex = threadAllStartMutex;
}
void JellyFish::ThreadRunAgent::agentRun() {
	lockAwait();
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

void JellyFish::ThreadRunAgent::lockAwait() {
	threadAllStartMutex->lock();
	call();
	threadAllStartMutex->unlock();
	std::unique_lock<std::mutex> containerLock(*containerMutex);
	containerCondition->wait(containerLock);
}
void JellyFish::ThreadRunAgent::call() {
	threadSleepSize->fetch_sub(1);
	int thisSize=threadSleepSize->load();
	if (thisSize <= 0) {
		bool allStart = true;
		threadAllStart->tryOneComplete(allStart);
	}
}
