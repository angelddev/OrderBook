#include "../include/CsvParser.h"
#include "../include/BookReconstruction.h"

#include <iostream>
#include <string>

int main() {
    const std::string dataPath = "data/OrderBookActions_APPLE.csv";
    const std::vector<MarketData::OrderEvent> events = MarketData::parseCsvFile(dataPath);

    if (events.empty()) {
        std::cout << "Couldn't register any events.\n";
        return 1;
    }

    const std::size_t previewCount = std::min<std::size_t>(10, events.size());
    std::cout << "Previewing the first " << previewCount << " parsed events:\n";

    for (std::size_t i = 0; i < previewCount; ++i) {
        const MarketData::OrderEvent& event = events[i];
        std::cout << "At time " << event.timestamp
                  << ", eventType=" << static_cast<int>(event.eventType)
                  << ", orderId=" << event.orderId
                  << ", amount=" << event.amount
                  << ", price=" << event.price
                  << ", direction=" << (event.direction == MarketData::OrderDirection::Buy ? "Buy" : "Sell")
                  << "\n";
    }

    return 0;
}