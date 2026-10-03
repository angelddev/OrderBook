#include "BookReconstruction.h"
#include "CsvParser.h"

#include <array>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

namespace {

    constexpr std::array<const char*, 7> resultNames{
        "applied", "orders removed after omitted earlier volume", "hidden executions ignored",
        "halts ignored", "unknown order IDs", "duplicate order IDs", "invalid quantities"
    };

    void printLevels(const char* side, const std::vector<MarketData::PriceLevel>& levels) {
        std::cout << side << ":\n";
        for (const auto& level : levels) {
            std::cout << "  price_units=" << level.price
                      << ", quantity=" << level.totalQuantity << '\n';
        }
    }

}  // namespace

int main(int argc, char* argv[]) {
    const std::string dataPath = argc > 1 ? argv[1] : "data/OrderBookActions_APPLE.csv";

    try {
        const auto parsed = MarketData::parseMessageCsv(dataPath);
        std::cout << "Parsed events: " << parsed.rows.size()
                  << ", skipped malformed rows: " << parsed.skippedRows;
        if (parsed.firstInvalidLine != 0) {
            std::cout << " (first at line " << parsed.firstInvalidLine << ')';
        }
        std::cout << '\n';

        if (parsed.rows.empty()) {
            std::cerr << "No valid events to process.\n";
            return 1;
        }

        OrderBook::LimitOrderBook book;
        std::array<std::size_t, resultNames.size()> outcomes{};
        for (const auto& event : parsed.rows) {
            ++outcomes[static_cast<std::size_t>(book.processEvent(event))];
        }

        std::cout << "Event outcomes:\n";
        for (std::size_t index = 0; index < outcomes.size(); ++index) {
            std::cout << "  " << resultNames[index] << ": " << outcomes[index] << '\n';
        }
        std::cout << "Tracked orders at end: " << book.totalOrders() << '\n';
        printLevels("Best bids", book.bestBidLevels(5));
        printLevels("Best asks", book.bestAskLevels(5));
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Order book run failed: " << error.what() << '\n';
        return 1;
    }
}