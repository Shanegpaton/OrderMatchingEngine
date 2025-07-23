#pragma once
#include <vector>

#include "Order.hpp"

class OrderPool
{
public:
	OrderPool();
	~OrderPool();
	Order* acquire();
	void release(Order* order);

private:
	std::vector<Order*> orders;
};

