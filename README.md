# Order Book Reconstruction and Simulation

A C++ project focused on reconstructing and simulating a limit order book from historical market data. The goal is to build a practical understanding of how electronic markets process incoming orders, how liquidity is represented, and how executions occur in a matching engine.

This project is meant to grow from data ingestion and book reconstruction into a replayable market simulation. It is designed as a strong first portfolio project for students interested in quantitative finance, market microstructure, trading systems, or low-level systems programming.

## Why this project

Financial markets are driven by continuous streams of orders. Every trade, cancellation, and modification updates the order book and changes the visible liquidity at each price level. By reconstructing this book from historical event data, we can study the flow of orders, market depth, and execution behavior.

This project is motivated by a simple question:

How can we turn raw market event data into a usable reconstruction of the order book and then simulate the matching process behind real price discovery?

## Project goals

The long-term direction for this project is:

- Parse historical order event data from CSV files
- Reconstruct the current state of the limit order book over time
- Model bid/ask liquidity and price levels
- Replay historical events in sequence
- Build a matching engine for order execution logic
- Validate simulated results against observable market behavior

## Current status

The project currently focuses on the foundation:

- reading CSV-based market data
- parsing event records into structured C++ objects
- preparing the project architecture for order book reconstruction

## Data source

The project uses sample market data from the LOBSTER dataset, which provides limit order book event messages for historical trading activity. The files in the data folder contain the event stream for a selected stock, including:

- timestamp
- event type
- order ID
- order size
- price
- side (buy/sell)

This is a realistic data source for learning how exchange-level event streams are represented and consumed.

## Architecture

The project is organized into a few core components:

- `src/main.cpp` - entry point for the project
- `src/CsvParser.cpp` - parses market data from CSV files
- `include/CsvParser.h` - definitions for event structures and parsing logic
- `include/BookReconstruction.h` - planned order book reconstruction interfaces

## Roadmap

### Phase 1: Market data ingestion
- [x] Set up the C++ project structure
- [x] Configure CMake build
- [x] Parse basic historical event CSV files
- [ ] Handle edge cases in CSV parsing
- [ ] Validate schema assumptions against real data

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
```

## Why this is a valuable internship project

This project demonstrates interest in a meaningful technical domain and shows a progression from data processing to systems design:

- data parsing
- event-driven systems
- market structure
- state reconstruction
- matching logic
- simulation and validation

That combination is especially relevant for roles in:

- software engineering
- quantitative engineering
- trading systems
- market data / exchange infrastructure
- algorithmic trading infrastructure

## Future direction

The eventual goal is to evolve the project from a parser into a replayable, event-driven order book engine. That would allow the system to answer questions like:

- What was the visible liquidity at a given point in time?
- How did new orders affect the book?
- Which orders were executed and at what price?
- How can we model market behavior through reconstructed order flow?

## Notes

This project is a strong first step in building a foundation in market microstructure and event-driven systems. It is intentionally scoped to be ambitious, educational, and portfolio-ready without being unrealistic for a student project.

The aim is not just to build a parser, but to build a story: from raw exchange data, to reconstructed market state, to a functioning understanding of how trades are matched in real markets.
