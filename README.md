# Order Matching Engine

A high-performance, multithreaded **order matching engine** built in modern C++. This project simulates the core logic behind a financial exchange, processing real-time buy/sell orders with efficient batching and thread synchronization.

Built as a systems-level project to demonstrate my ability to write concurrent, performant, and testable C++ code — ideal for backend infrastructure, finance systems, or real-time data processing roles.

---

## Why I Built This

I wanted to challenge myself with a project that:
- Applies **low-level systems programming** techniques
- Requires **efficient memory and thread management**
- Involves **algorithmic thinking** (matching logic, queues, prioritization)
- Produces **measurable performance metrics**

---

## Features

- **Multithreaded architecture** using `std::thread`, `mutex`, and `condition_variable`
- **Producer-consumer pipeline** to simulate order inflow
- **Batch processing** for throughput optimization
- **Custom matching logic** prioritizing price and FIFO time
- **Performance logging** (orders/sec, latency, match rate)
- **CSV export** for analysis

---

## Project Structure

OrderMatchingEngine/
├── include/ # Header files (Order.hpp, Matcher.hpp, etc.)
├── src/ # Implementation (main.cpp, Producer.cpp, etc.)
├── data/ # Output: matched orders, logs
├── demo/ # Optional demo video or screenshots
├── CMakeLists.txt # CMake config
└── README.md # You're here!

---

## ⚙️ Build & Run

### Prerequisites

- C++17 or newer
- CMake 3.10+

### Clone & Compile

```bash
git clone https://github.com/Shanegpaton/OrderMatchingEngine.git
cd OrderMatchingEngine
mkdir build && cd build
cmake ..
make

### Run

./OrderMatchingEngine



