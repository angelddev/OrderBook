# Order Book Reconstruction and Simulation

This is a C++ project I've been working on to reconstruct and simulate a limit order book using historical market data. As someone deeply interested in C++ and high-performance systems, my goal here is to build a practical understanding of how electronic markets process incoming orders, handle liquidity, and execute trades in a matching engine.


## Why I built this

As someone passionate about competitive programming and the ICPC, I wanted to take algorithms beyond problem solving and see how they behave inside a real system. I was especially interested in how choosing the right data structures and implementation details can make a measurable difference when processing millions of events.

Finance felt like the perfect environment for that. Market systems constantly process orders, cancellations, and executions while requiring both precision and extremely low latency. This gave me an opportunity to explore not only the algorithms behind an order book, but also the systems side of C++: efficient data and string manipulation, appropriate types, memory usage, headers, CMake, and compilation.

The goal is to continuously simulate and benchmark different approaches and actually see how these decisions affect performance at scale.

And what better language to explore that than C++—a language I enjoy for its performance, control, practicality, and elegance?

So I built a market data matching engine to reconstruct order books from historical events and simulate the matching process, using it as a hands-on way to explore the relationship between algorithmic correctness, precision, and performance.

## Project goals

My long-term direction for this system is to:

- Parse historical order event data from CSV files.
- Reconstruct the current state of the limit order book over time.
- Model bid and ask liquidity and price levels.
- Replay historical events in sequence.
- Build a matching engine for order execution logic.
- Validate my simulated results against observable market behavior.

## Current status

Right now, I am focusing on the foundation. I've set up the parsing for the CSV-based market data, written the logic to transform event records into structured C++ objects, and prepared the architecture for the full order book reconstruction.

## Data source

I am using sample market data from the LOBSTER dataset, which provides limit order book event messages for historical trading activity. The files in the data folder contain the event stream for a selected stock, including timestamps, event types, order IDs, order sizes, prices, and side (buy/sell). This is a highly realistic data source for learning how exchange-level event streams are actually represented and consumed.

## Architecture (as of now)

The codebase is organized into a few core components:

- `src/main.cpp` - entry point for the project
- `src/CsvParser.cpp` - parses market data from CSV files
- `include/CsvParser.h` - definitions for event structures and parsing logic
- `include/BookReconstruction.h` - planned order book reconstruction interfaces

## Roadmap

### Phase 1: Data ingestion
- [x] Set up the C++ project structure
- [x] Parse basic historical event CSV files while handling edge cases

### Phase 2: Order book reconstruction
- [ ] Represent bid and ask side data structures
- [ ] Reconstruct the book after each event
- [ ] Track price levels and order quantities
- [ ] Handle order cancellations and executions

### Phase 3: Replay and simulation
- [ ] Sequence historical events in time order
- [ ] Reconstruct book state over the trading day
- [ ] Add event-driven replay simulation

### Phase 4: Matching engine
- [ ] Implement matching logic for incoming orders
- [ ] Support buy and sell execution rules
- [ ] Validate execution outcomes against market events

### Phase 5: Analysis and validation
- [ ] Compare reconstructed state vs expected market behavior
- [ ] Produce summaries of order flow and liquidity dynamics
- [ ] Document results and engineering choices

## Build and run

Requirements:

- C++20 compatible compiler
- CMake 3.15 or newer

From the project root:

```bash
cmake -S . -B build
cmake --build build
./build/OrderBookExecutable
