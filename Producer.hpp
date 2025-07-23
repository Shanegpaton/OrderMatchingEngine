#pragma once
#include "SharedOrderState.hpp"

/**
 * @brief Creates a single simulated order with random values.
 *
 * The order is drawn from the order pool and populated with randomized price,
 * quantity, timestamp, ID, and order side (buy/sell).
 *
 * @param sharedState The shared state object that includes access to the order pool and UUID.
 * @return Pointer to the newly generated Order.
 */
Order* simulateOrder(SharedOrderState& sharedState);

 /**
  * @brief Simulates the production of orders and enqueues them for consumption.
  *
  * This function creates a specified number of simulated orders and pushes them
  * into the shared order queue. Each order is generated with randomized parameters.
  *
  * @param sharedState The shared state object containing order pool, queue, and synchronization primitives.
  * @param orderCount The number of orders this producer thread should generate.
  */
void producerLoop(SharedOrderState& sharedState, int producerCount);

