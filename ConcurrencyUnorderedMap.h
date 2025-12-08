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

#include <unordered_map>
#include <vector>

#include "ConcurrencyConstant.h"
#include "SharedMutexAgent.h"

#define DllExport __declspec(dllexport)

 /**
  * @file: ConcurrencyUnorderedMap.h
  * @brief: C++20
  *
  * @authors ThierrySquirrel
  * @date 2025/12/8
  **/

namespace JellyFish {
	template <typename K, typename V>
	class DllExport ConcurrencyUnorderedMap {
	private:std::unordered_map<int, std::unordered_map<K, V>>containerAll;
	private:int locksOffset;
	private:std::vector<SharedMutexAgent> locksContainer;
	
	public:ConcurrencyUnorderedMap<K, V>() {
		int locksOffset = JellyFish::ConcurrencyConstant::DEFAULT_LOCKS_OFFSET;
		this->init(locksOffset);
	}

	public:ConcurrencyUnorderedMap<K, V>(int& locksOffset) {
		this->init(locksOffset);
	}
	
	public:void set(K& key, V& value) {
		int mutexOffset = getMutexOffset(key);

		locksContainer[mutexOffset].lock();
		containerAll[mutexOffset][key] = value;
		locksContainer[mutexOffset].unlock();
	}

	public: V get(K& key) {
		int mutexOffset = getMutexOffset(key);

		locksContainer[mutexOffset].lockShared();
		V value = containerAll[mutexOffset][key];
		locksContainer[mutexOffset].unlockShared();

		return value;
	}

	public:bool isFind(K& key) {

		typename std::unordered_map<K, V>::const_iterator mapFind;
		typename std::unordered_map<K, V>::const_iterator mapEnd;

		int mutexOffset = getMutexOffset(key);

		locksContainer[mutexOffset].lockShared();
		mapFind = containerAll[mutexOffset].find(key);
		mapEnd = containerAll[mutexOffset].end();
		locksContainer[mutexOffset].unlockShared();

		return mapFind != mapEnd;
	}

	public:std::size_t getSize() {
		std::size_t allsize = 0;
		int forSize = this->locksOffset;

		for (int i = 0; i < forSize; i++) {
			locksContainer[i].lockShared();
			allsize += containerAll[i].size();
			locksContainer[i].unlockShared();
		}
		return allsize;
	}

	public:std::unordered_map<K, V> getAll() {
		std::unordered_map<K, V> all;
		int forSize = this->locksOffset;

		for (int i = 0; i < forSize; i++) {
			locksContainer[i].lockShared();
			std::unordered_map<K, V> thisMap = containerAll[i];
			locksContainer[i].unlockShared();

			all.insert(thisMap.begin(), thisMap.end());
		}
		return all;
	}

	public:void deleteValue(K& key) {
		int mutexOffset = getMutexOffset(key);

		locksContainer[mutexOffset].lock();
		containerAll[mutexOffset].erase(key);
		locksContainer[mutexOffset].unlock();
	}


	private:int getMutexOffset(K& key) {
		std::hash<K> hashKey;
		std::size_t hashKeyValue = hashKey(key);
		int thisLocksOffset = 0;
		thisLocksOffset = hashKeyValue % locksOffset;

		return thisLocksOffset;
	}

	private:void init(int& locksOffset) {
		this->locksOffset = locksOffset;
		for (int i = 0; i < locksOffset; i++) {
			SharedMutexAgent thisMutex;
			locksContainer.push_back(thisMutex);
			containerAll[i] = std::unordered_map<K, V>();
		}
	}
	};
}

