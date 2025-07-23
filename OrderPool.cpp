#include "OrderPool.hpp"

OrderPool::OrderPool() {
	orders.reserve(220000);
	for (int i = 0; i < orders.capacity(); i++) {
		orders.push_back(new Order);
	}
};
OrderPool::~OrderPool() {
	for (Order* order : orders) {
		delete order;
	}
}

Order* OrderPool::acquire() {
	if (!orders.empty()) {
		Order* order = orders.back();
		orders.pop_back();
		return order;
	}
	else {
		return new Order;
	}

}

void OrderPool::release(Order* order)
{
	orders.push_back(order);

}

