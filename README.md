## Order Book Reconstruction and Simulation

This is a C++ project I'm building to replay historical limit-order-book events and study how data structures affect correctness and performance. I'm interested in C++ and high-performance systems, and this project is a step-by-step way to learn how market data represents orders and executions.


#### Why I built this

As someone passionate about competitive programming and the ICPC, I wanted to take algorithms beyond problem solving and see how they behave inside a real system. I was especially interested in how choosing the right data structures and implementation details can make a measurable difference when processing millions of events.

I felt like Finance is a perfect environment for that. Market systems constantly process orders, cancellations, and executions while requiring both precision and extremely low latency. This gave me an opportunity to explore not only the algorithms behind an order book, but also the systems side of C++: efficient data and string manipulation, appropriate types, memory usage, headers, CMake, and compilation.

The goal is to continuously simulate and benchmark different approaches and actually see how these decisions affect performance at scale.

And what better language to explore that than C++, a language I enjoy for its performance, control, practicality, and elegance?

#### Project goals

My long-term direction for this system is to:

- Parse historical order event data from CSV files.
- Reconstruct the current state of the limit order book over time.
- Model bid and ask liquidity and price levels.
- Replay historical events in sequence.
- Build a matching engine for order execution logic.
- Validate my simulated results against observable market behavior.

#### Current status

The CSV parser reads the full LOBSTER message file, validates the six fields, and stores prices as integer price units and timestamps as integer nanoseconds. The current order-book prototype keeps bid and ask price levels, FIFO order queues, and cached per-price aggregate quantities. It processes submissions, cancellations, deletions, and visible executions; hidden executions do not change displayed liquidity.

Each active order is stored once in the ID index and linked into its price-level FIFO queue. This avoids a second full order copy in the queue and lets known orders be unlinked without scanning every order at that price. Price levels are ordered maps; their aggregate quantities are updated incrementally. These are data-structure choices.

This is an incremental prototype, not yet a reconstruction of the complete historical book. Unknown order IDs are reported.

This is roughly what the execution of the program looks like ![alt text](image.png)

#### Data source

I am using sample market data from the LOBSTER dataset. `OrderBookActions_APPLE.csv` contains timestamped message rows with event type, order ID, size, price, and direction. LOBSTER prices are represented as dollars multiplied by 10,000.

#### Architecture

The codebase is organized these components:

- `src/main.cpp` - parses the selected message file, replays its events, and prints a summary
- `src/CsvParser.cpp` - validates and parses LOBSTER message rows
- `include/CsvParser.h` - event types, exact market-data types, and parser result
- `src/BookReconstruction.cpp` - event updates for the prototype order book
- `include/BookReconstruction.h` - order-book data structures and API

#### Roadmap

##### Phase 1: Data ingestion
- [x] Set up the C++ project structure
- [x] Parse the full message CSV with field and value validation
- [x] Preserve price units and nanosecond timestamps as integers

##### Phase 2: Order book reconstruction
- [x] Represent bid and ask price levels
- [x] Track orders by ID and aggregate quantity by price
- [x] Apply known submissions, cancellations, deletions, and visible executions
- [ ] Validate the complete book, including queue priority, against reference data

##### Phase 3: Replay and simulation
- [x] Process parsed events in file order
- [ ] Add replay controls and snapshots over time

##### Phase 4: Matching engine
- [ ] Implement matching logic for incoming orders
- [ ] Validate simulated executions against market events

##### Phase 5: Analysis and validation
- [ ] Compare reconstructed state vs expected market behavior
- [ ] Produce summaries of order flow and liquidity dynamics
- [ ] Document results and engineering choices

#### Build and run

Requirements:

- C++20 compatible compiler
- CMake 3.15 or newer

From the project root:

```bash
cmake -S . -B build
cmake --build build
./build/OrderBookExecutable [message-csv-path]
```

If no path is supplied, the executable reads `data/OrderBookActions_APPLE.csv`. Prices printed by the program are raw LOBSTER units: dollars multiplied by 10,000.
