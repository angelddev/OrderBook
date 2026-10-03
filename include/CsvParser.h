#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace MarketData {

    using Price = std::int64_t;
    using Quantity = std::int64_t;
    using TimestampNanoseconds = std::int64_t;

    inline constexpr Price EmptyAskPrice = 9999999999LL;
    inline constexpr Price EmptyBidPrice = -9999999999LL;

    enum class OrderDirection : int {
        Buy = 1,
        Sell = -1
    };

    enum class EventType : int {
        NewOrder = 1,
        PartialCancellation = 2,
        FullDeletion = 3,
        VisibleExecution = 4,
        HiddenExecution = 5,
        Halt = 7
    };

    struct OrderEvent {
        TimestampNanoseconds timestamp = 0;
        EventType eventType = EventType::NewOrder;
        long long orderId = 0;
        Quantity amount = 0;
        Price price = 0;
        OrderDirection direction = OrderDirection::Buy;
    };

    struct PriceLevel {
        Price price = 0;
        Quantity totalQuantity = 0;

        bool operator==(const PriceLevel&) const = default;
    };

    struct BookSnapshot {
        PriceLevel bestAsk{EmptyAskPrice, 0};
        PriceLevel bestBid{EmptyBidPrice, 0};

        bool operator==(const BookSnapshot&) const = default;
    };

    template <typename Row>
    struct CsvParseReport {
        std::vector<Row> rows;
        std::size_t skippedRows = 0;
        std::size_t firstInvalidLine = 0;
    };

    CsvParseReport<OrderEvent> parseMessageCsv(const std::string& filePath);

}  // namespace MarketData