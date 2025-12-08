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
#include "ThreadScheduledOne.h"

 /**
  * @file: ThreadScheduledOne.cpp
  * @brief: C++20
  *
  * @authors ThierrySquirrel
  * @date 2025/12/8
  **/

JellyFish::ThreadScheduledOne::ThreadScheduledOne() {
	JellyFish::ThreadScheduledOne::init();
}
JellyFish::ThreadScheduledOne::~ThreadScheduledOne() {
	JellyFish::ThreadScheduledOne::deleteAll();
}

void JellyFish::ThreadScheduledOne::execute(VirtualThreadRun* virtualThreadRun, int& millisecond) {
	JellyFish::VirtualThreadRun* bindThisClass = virtualThreadRun->bindThisClass();
	JellyFish::ThreadRunScheduledAgent agent(bindThisClass, millisecond, &isDeleteAll);
	std::thread(std::bind(&JellyFish::ThreadRunScheduledAgent::agentRun, agent)).detach();
}

void JellyFish::ThreadScheduledOne::init() {
	isDeleteAll.store(false);
}
void JellyFish::ThreadScheduledOne::deleteAll() {
	isDeleteAll.store(true);
}