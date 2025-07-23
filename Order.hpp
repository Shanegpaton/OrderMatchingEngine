#pragma once
#include <cstdint>

enum class OrderSide { BUY, SELL };

struct Order
{
	int id;
	double quantity;
	double price;
	int64_t timeStamp;
	OrderSide side;
};

struct BuyOrderComparator {
	bool operator()(const Order* a, const Order* b) {
		if (a->price > b->price) return false;
		if (a->price < b->price) return true;
		if (a->timeStamp > b->timeStamp) return false;
		if (a->timeStamp < b->timeStamp) return true;
		return a->id > b->id;
	}
};

struct SellOrderComparator {
	bool operator()(const Order* a, const Order* b) {
		if (a->price > b->price) return true;
		if (a->price < b->price) return false;
		if (a->timeStamp > b->timeStamp) return false;
		if (a->timeStamp < b->timeStamp) return true;
		return a->id > b->id;
	}
};

