#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

#include "OrderPool.hpp"	
struct SharedOrderState {
	std::priority_queue<Order*, std::vector<Order*>, BuyOrderComparator> buyHeap;
	std::priority_queue<Order*, std::vector<Order*>, SellOrderComparator> sellHeap;
	std::queue<Order*> orderQueue;
	std::vector<std::string> metricsVec;
	std::mutex queueMutex;
	std::mutex orderPoolMutex;
	std::condition_variable cv;
	std::atomic<bool> terminate = false;
	std::atomic<long long> totalWaitTimeMicroseconds = 0;
	std::atomic<unsigned int> uuid = 1;
	unsigned int totalMatchedOrders = 0;
	size_t maxQueueSize = 0;
	unsigned int totalPrice = 0;
	double totalQuantity = 0;
	OrderPool orderPool;
};