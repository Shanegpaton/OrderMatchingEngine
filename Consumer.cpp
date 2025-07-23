#include "Consumer.hpp"
#include <string>

void matchOrders(SharedOrderState& sharedState, int skippedOrders) {
	Order* buyTop;
	Order* sellTop;
	// continue matching as long as top buy >= top sell price
	while (sharedState.buyHeap.size() > 0 && sharedState.sellHeap.size() > 0 && sharedState.buyHeap.top()->price >= sharedState.sellHeap.top()->price) {
		// Match as much quantity as possible
		buyTop = sharedState.buyHeap.top();
		sellTop = sharedState.sellHeap.top();
		double orderQuant = std::min(buyTop->quantity, sellTop->quantity);
		buyTop->quantity -= orderQuant;
		sellTop->quantity -= orderQuant;
		sharedState.totalQuantity += orderQuant;
		sharedState.totalPrice += orderQuant * sellTop->price;
		// log the match based on the skip interval
		if (sharedState.totalMatchedOrders % skippedOrders == 0) {
			sharedState.metricsVec.push_back(std::to_string(sellTop->price));
			sharedState.metricsVec.push_back(std::to_string(orderQuant));
			sharedState.metricsVec.push_back(std::to_string(buyTop->id));
			sharedState.metricsVec.push_back(std::to_string(sellTop->id));
		}
		// pop orders when they have no shares left
		if (buyTop->quantity == 0) {
			sharedState.buyHeap.pop();
			{
				std::lock_guard<std::mutex> lock(sharedState.orderPoolMutex);
				sharedState.orderPool.release(buyTop);
			}
			sharedState.totalMatchedOrders++;
		}
		if (sellTop->quantity == 0) {
			sharedState.sellHeap.pop();
			{
				std::lock_guard<std::mutex> lock(sharedState.orderPoolMutex);
				sharedState.orderPool.release(sellTop);
			}
			sharedState.totalMatchedOrders++;
		}
	}
}


void consumerLoop(SharedOrderState& sharedState, int skippedOrders) {
	constexpr size_t MAX_BATCH_SIZE = 2000;
	std::vector<Order*> batch;
	batch.reserve(MAX_BATCH_SIZE);
	// while there are shares left to match or the producer threads are still adding orders
	while (!sharedState.terminate || !sharedState.orderQueue.empty()) {
		sharedState.maxQueueSize = std::max(sharedState.orderQueue.size(), sharedState.maxQueueSize);
		Order* nextOrder = nullptr;
		// lock the queue and wiat until there are available orders or termination
		std::unique_lock<std::mutex> lock(sharedState.queueMutex);
		sharedState.cv.wait(lock, [&sharedState] { return !sharedState.orderQueue.empty() || sharedState.terminate; });
		// batch orders from the shared queue to reduce locking and unlocking
		for (int i = 0; i < std::min(MAX_BATCH_SIZE, sharedState.orderQueue.size()); i++) {
			nextOrder = sharedState.orderQueue.front();
			sharedState.orderQueue.pop();
			batch.push_back(nextOrder);
		}
		lock.unlock();
		// match in order to ensure fair trades
		for (Order* order : batch) {
			if (order == nullptr) {
				continue;
			}
			order->side == OrderSide::BUY ? sharedState.buyHeap.push(order) : sharedState.sellHeap.push(order);
			matchOrders(sharedState, skippedOrders);
		}
		batch.clear();
	}
}