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
#include "ThreadPool.h"

 /**
  * @file: ThreadPool.cpp
  * @brief: C++20
  *
  * @authors ThierrySquirrel
  * @date 2025/12/8
  **/

JellyFish::ThreadPool::ThreadPool() {
	int maxThreadSize = JellyFish::ThreadPoolConstant::DEFAULT_MAX_THREAD_SIZE;
	init(maxThreadSize);
}
JellyFish::ThreadPool::ThreadPool(int& maxThreadSize) {
	init(maxThreadSize);
}

JellyFish::ThreadPool::~ThreadPool() {
	JellyFish::ThreadPool::deleteAll();
}

void JellyFish::ThreadPool::execute(JellyFish::VirtualThreadRun* virtualThreadRun) {
	JellyFish::VirtualThreadRun* thisVirtualThreadRun = virtualThreadRun->bindThisClass();
	containerAll.pushBack(thisVirtualThreadRun);
	containerCondition.notify_one();
}

void JellyFish::ThreadPool::setMaxThreadSize(int& maxThreadSize) {
	this->maxThreadSize = maxThreadSize;
}

int JellyFish::ThreadPool::getMaxThreadSize() {
	return this->maxThreadSize;
}

void JellyFish::ThreadPool::init(int& maxThreadSize) {
	JellyFish::ThreadPool::setMaxThreadSize(maxThreadSize);
	isDeleteAll.store(false);
	JellyFish::ConcurrencyDeque<JellyFish::VirtualThreadRun*> thisDeque(maxThreadSize);
	containerAll = thisDeque;

	for (int i = 0; i < maxThreadSize; i++) {
		JellyFish::ThreadRunAgent runAgent(&containerAll,
			&containerMutex,
			&containerCondition,
			&isDeleteAll);
		std::thread(std::bind(&JellyFish::ThreadRunAgent::agentRun, runAgent)).detach();
	}
}

void JellyFish::ThreadPool::deleteAll() {
	isDeleteAll.store(true);
	containerCondition.notify_all();

	bool deleteAll = false;
	while (!deleteAll) {
		JellyFish::BaseContainer<JellyFish::VirtualThreadRun*> thisContainer = containerAll.tryPopBack();
		bool empty = thisContainer.getIsEmpty();
		if (empty) {
			deleteAll = true;
			break;
		}
		JellyFish::VirtualThreadRun* value = thisContainer.getValue();

		delete value;
		value = nullptr;
	}
}