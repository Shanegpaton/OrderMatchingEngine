#pragma once
#include "SharedOrderState.hpp"

/**
 * Matches buy and sell orders from the priority queues.
 *
 * Orders are matched based on price compatibility (buy price >= sell price).
 * Trades are executed by transferring quantity, and relevant trade metrics
 * (e.g., price, quantity, buyer/seller IDs) are logged every `skippedOrders` matches.
 *
 * @param sharedState Shared state containing order queues, mutexes, and metrics.
 * @param skippedOrders Determines how frequently to log matched order data.
 */
void matchOrders(SharedOrderState& sharedState, int skippedOrders);

/**
 * Continuously consumes orders from the queue and processes them in batches.
 *
 * Orders are pulled from the shared queue, added to the appropriate buy/sell heap,
 * and matched via matchOrders. The loop runs until termination is requested
 * and the queue is empty.
 *
 * @param sharedState Shared state for accessing the order queue and related resources.
 * @param skippedOrders Interval for logging trade data during matching.
 */
void consumerLoop(SharedOrderState& sharedState, int skippedOrders);