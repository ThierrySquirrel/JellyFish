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
#include "ThreadRunScheduledAgent.h"

 /**
  * @file: ThreadRunScheduledAgent.cpp
  * @brief: C++20
  *
  * @authors ThierrySquirrel
  * @date 2025/12/8
  **/

std::condition_variable CONDITION_TIME;
std::mutex MUTEX_TIME;

JellyFish::ThreadRunScheduledAgent::ThreadRunScheduledAgent() {

}
JellyFish::ThreadRunScheduledAgent::ThreadRunScheduledAgent(JellyFish::VirtualThreadRun* virtualThreadRun,
	int& millisecond,
	std::atomic<bool>* isDeleteAll) {

	this->virtualThreadRun = virtualThreadRun;
	this->millisecond = millisecond;
	this->isDeleteAll = isDeleteAll;
}
void JellyFish::ThreadRunScheduledAgent::agentRun() {
	bool deleteAll = isDeleteAll->load();
	while (!deleteAll) {
		virtualThreadRun->run();
		std::unique_lock<std::mutex> lockTime(MUTEX_TIME);
		CONDITION_TIME.wait_for(lockTime, std::chrono::milliseconds(this->millisecond));
		deleteAll = isDeleteAll->load();
	}
	delete virtualThreadRun;
	virtualThreadRun = nullptr;
}
