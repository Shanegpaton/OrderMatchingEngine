#include <iostream>
#include <queue>
#include <vector>
#include <random>
#include <chrono>
#include <mutex>
#include <fstream>
#include <string>
#include <condition_variable>

#include "Order.hpp"
#include "OrderPool.hpp"
#include "SharedOrderState.hpp"
#include "Producer.hpp"
#include "Consumer.hpp"

using BuyQueue = std::priority_queue<Order*, std::vector<Order*>, BuyOrderComparator>;
using SellQueue = std::priority_queue<Order*, std::vector<Order*>, SellOrderComparator>;


// function to write metrics a csv file
void logMetrics(SharedOrderState& sharedState, int duration_us) {
	// add metric headers to vector
	sharedState.metricsVec.push_back(std::to_string((duration_us / 1000000.0)));
	sharedState.metricsVec.push_back(std::to_string(sharedState.totalPrice));
	sharedState.metricsVec.push_back(std::to_string(sharedState.totalQuantity));
	sharedState.metricsVec.push_back(std::to_string(sharedState.totalMatchedOrders));
	sharedState.metricsVec.push_back(std::to_string(sharedState.uuid - 1));
	std::ofstream file("metrics.csv");
	file << "Orders,Total Matches,Shares Sold,Dollars Traded,Time\n";
	size_t size = sharedState.metricsVec.size();
	// write headers to the file
	for (size_t i = sharedState.metricsVec.size() - 1; i > size - 6; i--) {
		file << sharedState.metricsVec[i] << ",";
		sharedState.metricsVec.pop_back();
		
	}
	file << "\n\nPrice,Quantitiy Traded,BuyerID,SellerID";
	// write the mathced orders
	for (int i = 0; i < sharedState.metricsVec.size(); i++) {
		if (i % 4 == 0) {
			file << "\n";
		}
		else {
			file << ",";
		}
		file << sharedState.metricsVec[i];
	}

}

struct Config {
	int threadCount;
	int ordersPerThread;
	int skippedOrders;
};

// function to get configuration input from the user
Config handleInput() {
	Config config;
	std::cout << "How many producer threads would you like?" << std::endl;
	std::cin >> config.threadCount;
	std::cout << "How many orders per producer thread?" << std::endl;
	std::cin >> config.ordersPerThread;
	std::cout << "Print every how many matches? (e.g., enter 100 to print every 100th match)" << std::endl;
	std::cin >> config.skippedOrders;
	return config;
}

int main(int argc, char* argv) {
	Config config = handleInput();
	SharedOrderState sharedState;
	std::vector<std::thread> producers;
	// Preallocate metric storage to avoid reallocations during runtime
	sharedState.metricsVec.reserve(20 + config.ordersPerThread * config.threadCount / config.skippedOrders);
	auto start = std::chrono::high_resolution_clock::now();
	// Start consumer thread for processing and logging trades
	std::thread consumer(consumerLoop, std::ref(sharedState), config.skippedOrders);
	// Start producer threads to generate simulated orders
	for (int i = 0; i < config.threadCount; i++) {
		producers.emplace_back(producerLoop, std::ref(sharedState), config.ordersPerThread);
	}
	// Wait for all producers to finish
	for (auto& p : producers) {
		p.join(); 
	}
	// Signal consumer to terminate after all producers are done
	sharedState.terminate = true;
	sharedState.cv.notify_all();
	consumer.join();
	auto end = std::chrono::high_resolution_clock::now();
	auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	// populate csv with metrics
	logMetrics(sharedState, duration_us);
	return 0;

}