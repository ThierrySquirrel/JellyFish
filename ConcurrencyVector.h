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

#include <vector>

#include "ConcurrencyConstant.h"
#include "SharedMutexAgent.h"
#include "BaseContainerBuild.h"

#define DllExport __declspec(dllexport)

 /**
  * @file: ConcurrencyVector.h
  * @brief: C++20
  *
  * @authors ThierrySquirrel
  * @date 2025/12/8
  **/

namespace JellyFish {
	template <typename V>
	class DllExport ConcurrencyVector {
	private:std::vector<std::vector<V>> containerAll;
	private:std::atomic<int> containerOffset;
	private:int locksOffset;
	private:std::vector<SharedMutexAgent> locksContainer;

	public:ConcurrencyVector<V>& operator=(const ConcurrencyVector<V>& value) {
		containerAll = value.containerAll;
		containerOffset.store(value.containerOffset.load());
		locksOffset = value.locksOffset;
		locksContainer = value.locksContainer;
		return *this;
	}

	public:ConcurrencyVector<V>() {
		int locksOffset = JellyFish::ConcurrencyConstant::DEFAULT_LOCKS_OFFSET;
		this->init(locksOffset);
	}

	public:ConcurrencyVector<V>(int& locksOffset) {
		this->init(locksOffset);
	}

	public:void pushBack(V& value) {
		int mutexOffset = getMutexOffset(value);
		locksContainer[mutexOffset].lock();
		containerAll[mutexOffset].push_back(value);
		locksContainer[mutexOffset].unlock();
	}

	public:JellyFish::BaseContainer<V> tryPopBack() {
		int thisContainerOffset = this->getContainerOffset();

		int size = this->locksOffset;
		for (int i = 0; i < size; i++) {
			locksContainer[thisContainerOffset].lock();
			bool isEmpty = containerAll[thisContainerOffset].empty();
			if (!isEmpty) {
				V value = containerAll[thisContainerOffset].back();
				containerAll[thisContainerOffset].pop_back();
				locksContainer[thisContainerOffset].unlock();

				return JellyFish::BaseContainerBuild<V>::buildSuccess(value);
			}
			locksContainer[thisContainerOffset].unlock();

			thisContainerOffset++;
			if (thisContainerOffset == size) {
				thisContainerOffset = 0;
			}
		}
		return JellyFish::BaseContainerBuild<V>::buildFail();

	}

	public:std::size_t getSize() {
		std::size_t allsize = 0;
		int size = this->locksOffset;

		for (int i = 0; i < size; i++) {
			locksContainer[i].lockShared();
			allsize += containerAll[i].size();
			locksContainer[i].unlockShared();
		}
		return allsize;
	}

	public:std::vector<V> popBackAll() {
		std::vector<V> all;
		int size = this->locksOffset;

		for (int i = 0; i < size; i++) {
			locksContainer[i].lock();
			std::vector<V> thisVector = containerAll[i];
			containerAll[i].clear();
			locksContainer[i].unlock();
			all.insert(all.begin(), thisVector.begin(), thisVector.end());
		}
		return all;
	}

	private:int getMutexOffset(V& key) {
		std::hash<V> hashKey;
		std::size_t hashKeyValue = hashKey(key);
		int thisLocksOffset = 0;
		thisLocksOffset = hashKeyValue % locksOffset;

		return thisLocksOffset;
	}

	private:int getContainerOffset() {
		int fetchAdd = this->containerOffset.fetch_add(1);
		int thisLocksOffset = this->locksOffset;
		return ((fetchAdd % thisLocksOffset) + thisLocksOffset) % thisLocksOffset;
	}

	private:void init(int& locksOffset) {
		this->containerOffset.store(0);
		this->locksOffset = locksOffset;
		for (int i = 0; i < locksOffset; i++) {
			SharedMutexAgent thisMutex;
			locksContainer.push_back(thisMutex);
			std::vector<V> thisContainer;
			containerAll.push_back(thisContainer);
		}
	}

	};
}

