#include "Producer.hpp"

#include <random>
#include <chrono>


void producerLoop(SharedOrderState& sharedState, int orderCount) {
	for (int i = 0; i < orderCount; i++) {
		Order* simOrder = simulateOrder(sharedState);
		{
			std::lock_guard<std::mutex> guard(sharedState.queueMutex);
			sharedState.orderQueue.push(simOrder);
		}
		sharedState.cv.notify_one();
	}
}

Order* simulateOrder(SharedOrderState& sharedState) {
	Order* simOrder;
	{
		std::lock_guard<std::mutex> lock(sharedState.orderPoolMutex);
		simOrder = sharedState.orderPool.acquire();
	}
	// create a random seed for each threads engine
	int64_t time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	thread_local std::default_random_engine engine(std::random_device{}());
	thread_local std::uniform_real_distribution<double> priceDis(1.0, 5.0);
	thread_local std::uniform_real_distribution<double> quantDis(.5, 5000);
	thread_local std::uniform_int_distribution<int> sideDis(0, 1);
	simOrder->price = priceDis(engine);
	simOrder->timeStamp = time;
	simOrder->quantity = quantDis(engine);
	simOrder->id = sharedState.uuid.fetch_add(1);
	simOrder->side = sideDis(engine) == 0 ? OrderSide::BUY : OrderSide::SELL;
	return simOrder;
}