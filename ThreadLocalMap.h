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

#include "ThreadLocalMapConstant.h"
#include "ConcurrencyUnorderedMap.h"
#include "BaseContainerBuild.h"

#define DllExport __declspec(dllexport)

 /**
  * @file: ThreadLocalMap.h
  * @brief: C++20
  *
  * @authors ThierrySquirrel
  * @date 2025/12/8
  **/

namespace JellyFish {
	template <typename V>
	class DllExport ThreadLocalMap {
	private:JellyFish::ConcurrencyUnorderedMap<std::size_t, V> containerAll;
	private:int threadOffset;

	public:ThreadLocalMap<V>() {
		int locksOffset = JellyFish::ThreadLocalMapConstant::DEFAULT_THREAD_OFFSET;
		init(locksOffset);
	}

	public:ThreadLocalMap<V>(int& threadOffset) {
		init(threadOffset);
	}

	public:void set(V& value) {
		std::size_t key = this->getKey();
		containerAll.set(key, value);
	}

	public:JellyFish::BaseContainer<V>get() {
		std::size_t key = this->getKey();
		bool isFind = containerAll.isFind(key);
		if (isFind) {
			V value = containerAll.get(key);
			return JellyFish::BaseContainerBuild<V>::buildSuccess(value);
		}
		return JellyFish::BaseContainerBuild<V>::buildFail();
	}

	public:void deleteValue() {
		std::size_t key = this->getKey();
		containerAll.deleteValue(key);
	}

	private:std::size_t getKey() {
		std::thread::id threadId = std::this_thread::get_id();
		std::size_t key = std::hash<std::thread::id>()(threadId);
		return key;
	}

	private:void init(int& threadOffset) {
		this->threadOffset = threadOffset;
		JellyFish::ConcurrencyUnorderedMap<std::size_t, V> thisMap = JellyFish::ConcurrencyUnorderedMap<std::size_t, V>(this->threadOffset);
		this->containerAll = thisMap;
	}

	};
}
