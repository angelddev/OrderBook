#pragma once

#include "CsvParser.h"

#include <cstddef>
#include <map>
#include <unordered_map>
#include <vector>

namespace OrderBook {

    enum class EventResult {
        Applied,
        InferredOrderRemoval,
        IgnoredHiddenExecution,
        IgnoredHalt,
        UnknownOrder,
        DuplicateOrderId,
        InvalidQuantity
    };

    struct PriceLevel;

    struct Order {
        long long orderId = 0;
        MarketData::Price price = 0;
        MarketData::Quantity quantity = 0;
        MarketData::OrderDirection side = MarketData::OrderDirection::Buy;
        PriceLevel* level = nullptr;
        Order* previous = nullptr;
        Order* next = nullptr;
    };

    struct PriceLevel {
        MarketData::Price price = 0;
        MarketData::Quantity totalQuantity = 0;
        Order* head = nullptr;
        Order* tail = nullptr;
    };

    class LimitOrderBook {
    public:
        EventResult processEvent(const MarketData::OrderEvent& event);

        std::vector<MarketData::PriceLevel> bestBidLevels(std::size_t count = 5) const;
        std::vector<MarketData::PriceLevel> bestAskLevels(std::size_t count = 5) const;
        MarketData::BookSnapshot topOfBook() const;

        std::size_t totalOrders() const;
        bool containsOrder(long long orderId) const;

    private:
        using BidLevels = std::map<MarketData::Price, PriceLevel, std::greater<MarketData::Price>>;
        using AskLevels = std::map<MarketData::Price, PriceLevel, std::less<MarketData::Price>>;

        BidLevels bids_;
        AskLevels asks_;
        std::unordered_map<long long, Order> orders_;

        EventResult insertOrder(const MarketData::OrderEvent& event);
        EventResult reduceOrder(long long orderId, MarketData::Quantity reduction);
        EventResult eraseOrder(long long orderId);
        static void unlinkOrder(Order& order);
    };

}  // namespace OrderBook