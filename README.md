# JellyFish

JellyFish

[ÖÐÎÄ](./README_zh_CN.md)

Support function:  
- [x] ConcurrencyMap
- [x] ConcurrencyUnorderedMap
- [x] ConcurrencyVector
- [x] ConcurrencyDeque
- [x] ThreadLocalMap
- [x] CompletableFuture
- [x] ThreadPool
- [x] ThreadScheduledOne

# ConcurrencyMap:  
Support multi-threaded read and write  
  
# ConcurrencyUnorderedMap:  
Support multi-threaded read and write  

# ConcurrencyVector:  
Support multi-threaded read and write  

# ConcurrencyDeque:  
Support multi-threaded read and write  

# ThreadLocalMap:  
Support reading and writing local data of the current thread in the thread pool  

# CompletableFuture:  
Support asynchronous task processing. The current required return value can be completed in the future  

# ThreadPool:  
ThreadPool  

# ThreadScheduledOne:  
Tasks executed according to plan. Thread is unique  
 
## Quick Start

```C++
Nuget::Install JellyFish
``` 

 
 # ConcurrencyMap  
 ```C++
#pragma once
#include "JellyFish/ConcurrencyMap.h"
#include <iostream>
#include <string>
class TestMap {
public:void textMap() {
	printf("API test\n");
	JellyFish::ConcurrencyMap<int, std::string> thisMap;
	int key = 1;
	std::string value = "one";
	thisMap.set(key, value);

	std::string get = thisMap.get(key);
	printf("get::%s\n", get.c_str());
}
};
 ```


 # ConcurrencyUnorderedMap  
 ```C++
#pragma once
#include "JellyFish/ConcurrencyUnorderedMap.h"
#include <iostream>
#include <string>
class TestUnorderedMap {
public:void textUnorderedMap() {
	printf("API test\n");
	JellyFish::ConcurrencyUnorderedMap<int, std::string> thisMap;
	int key = 1;
	std::string value = "one";
	thisMap.set(key, value);

	std::string get = thisMap.get(key);
	printf("get::%s\n", get.c_str());
}
};
 ```


 # ConcurrencyVector  
 ```C++
#pragma once
#include "JellyFish/ConcurrencyVector.h"
#include <iostream>
class TestVector {
public:void texVector() {
	printf("API test\n");
	JellyFish::ConcurrencyVector<int> thisVector;
	int value = 20;
	thisVector.pushBack(value);

	JellyFish::BaseContainer<int> baseV = thisVector.tryPopBack();
	std::cout << "IsEmpty::" << baseV.getIsEmpty() << std::endl;

	printf("get::%d\n", baseV.getValue());
}
};
 ```
 

 # ConcurrencyDeque  
 ```C++
#pragma once
#include "JellyFish/ConcurrencyDeque.h"
#include <iostream>
class TestDeque {
public:void texDeque() {
	printf("API test\n");
	JellyFish::ConcurrencyDeque<int> thisDeque;
	int value = 20;
	thisDeque.pushBack(value);

	JellyFish::BaseContainer<int> baseV = thisDeque.tryPopBack();
	std::cout << "IsEmpty::" << baseV.getIsEmpty() << std::endl;

	printf("get::%d\n", baseV.getValue());
}
};
 ```


 # ThreadLocalMap  
 ```C++
#pragma once
#include "JellyFish/ThreadLocalMap.h"
#include <iostream>
class TestThreadLocalMap {
public:void texMap() {
	printf("API test\n");
	JellyFish::ThreadLocalMap<int> thisMap;
	int value = 20;
	thisMap.set(value);

	JellyFish::BaseContainer<int> baseV = thisMap.get();
	std::cout << "IsEmpty::" << baseV.getIsEmpty() << std::endl;

	printf("get::%d\n", baseV.getValue());
}
};
 ```

 # TestFuture  
 ```C++
#pragma once
#include "JellyFish/CompletableFuture.h"
#include <iostream>

class TestFuture {
public:void testFuture() {
	printf("API test\n");
	JellyFish::CompletableFuture<int> future;
	int a = 20;
	future.tryOneComplete(a);

	int time = 100;
	int maxTryCount = 5;

	JellyFish::BaseContainer<int> baseV = future.tryOneGet(time, maxTryCount);
	std::cout << "IsEmpty::" << baseV.getIsEmpty() << std::endl;

	printf("get::%d\n", baseV.getValue());
}
};
 ```


 # ThreadPool  ThreadDemo.h  
 ```C++
#pragma once
#include"JellyFish/VirtualThreadRun.h"

#include <string>
#include <iostream>

#include <thread>
#include <functional>

class ThreadDemo :public JellyFish::VirtualThreadRun {
public:int age;
public:std::string name;
public:ThreadDemo();
public:ThreadDemo(int& age, std::string& name);
public:void demoRun(int& ageA, std::string& nameA);
public:void run()override;
public:JellyFish::VirtualThreadRun* bindThisClass()override;
};
 ```


 # ThreadPool  ThreadDemo.cpp  
 ```C++
#include "ThreadDemo.h"

ThreadDemo::ThreadDemo() {}
ThreadDemo::ThreadDemo(int& age, std::string& name) {
	this->age = age;
	this->name = name;
}
void ThreadDemo::demoRun(int& ageA, std::string& nameA) {

	printf("Name: %s, Age: %d, \n", nameA.c_str(), ageA);
}
void ThreadDemo::run() {
	ThreadDemo::demoRun(this->age, this->name);
}
JellyFish::VirtualThreadRun* ThreadDemo::bindThisClass() {
	return new ThreadDemo(this->age, this->name);
}
 ```


 # ThreadPool  
 ```C++
#pragma once
#include "JellyFish/ThreadPool.h"
#include "ThreadDemo.h"

class TestThreadPool {
public:void testPool() {
	int max = 3;
	JellyFish::ThreadPool* tp = new JellyFish::ThreadPool(max);

	for (int i = 0; i < 20; i++) {
		int age = i;
		std::string name = "HelloWorld";
		ThreadDemo demo(age, name);
		tp->execute(&demo);
	}

	std::this_thread::sleep_for(std::chrono::seconds(2));
	delete tp;
	tp = nullptr;
	printf("ALL\n");
}
};
 ```


 # ThreadScheduledOne  
 ```C++
#pragma once
#include "JellyFish\ThreadScheduledOne.h"
#include "ThreadDemo.h"
class TestScheduled {
public:void test() {
	JellyFish::ThreadScheduledOne* one = new JellyFish::ThreadScheduledOne;
	int age = 20;
	std::string name = "HelloWorld";
	ThreadDemo demo(age, name);
	int time = 500;
	one->execute(&demo, time);

	std::this_thread::sleep_for(std::chrono::seconds(5));
	delete one;
	one = nullptr;
	printf("ALL\n");
}
};
 ```