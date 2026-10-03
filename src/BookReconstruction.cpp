#include "BookReconstruction.h"

#include <algorithm>

namespace OrderBook {

    EventResult LimitOrderBook::insertOrder(const MarketData::OrderEvent& event) {
        if (event.orderId <= 0 || event.amount <= 0 || event.price < 0) {
            return EventResult::InvalidQuantity;
        }
        if (orders_.contains(event.orderId)) {
            return EventResult::DuplicateOrderId;
        }

        auto addToSide = [this, &event](auto& levels) {
            PriceLevel& level = levels[event.price];
            level.price = event.price;
            auto [orderIt, inserted] = orders_.emplace(
                event.orderId,
                Order{event.orderId, event.price, event.amount, event.direction, &level, level.tail, nullptr});
            if (!inserted) {
                return false;
            }

            Order& order = orderIt->second;
            if (level.tail != nullptr) {
                level.tail->next = &order;
            } else {
                level.head = &order;
            }
            level.tail = &order;
            level.totalQuantity += event.amount;
            return true;
        };

        const bool inserted = event.direction == MarketData::OrderDirection::Buy
            ? addToSide(bids_)
            : addToSide(asks_);
        return inserted ? EventResult::Applied : EventResult::DuplicateOrderId;
    }

    void LimitOrderBook::unlinkOrder(Order& order) {
        PriceLevel& level = *order.level;
        if (order.previous != nullptr) {
            order.previous->next = order.next;
        } else {
            level.head = order.next;
        }
        if (order.next != nullptr) {
            order.next->previous = order.previous;
        } else {
            level.tail = order.previous;
        }
        order.previous = nullptr;
        order.next = nullptr;
    }

    EventResult LimitOrderBook::reduceOrder(long long orderId, MarketData::Quantity reduction) {
        if (reduction <= 0) {
            return EventResult::InvalidQuantity;
        }
        const auto orderIt = orders_.find(orderId);
        if (orderIt == orders_.end()) {
            return EventResult::UnknownOrder;
        }

        Order& order = orderIt->second;
        PriceLevel* const level = order.level;
        const MarketData::Price price = order.price;
        const MarketData::OrderDirection side = order.side;
        const bool missingEarlierVolume = reduction > order.quantity;
        const MarketData::Quantity appliedReduction = std::min(reduction, order.quantity);
        order.quantity -= appliedReduction;
        level->totalQuantity -= appliedReduction;

        if (order.quantity == 0) {
            unlinkOrder(order);
            orders_.erase(orderIt);
            if (level->head == nullptr) {
                if (side == MarketData::OrderDirection::Buy) {
                    bids_.erase(price);
                } else {
                    asks_.erase(price);
                }
            }
        }

        return missingEarlierVolume ? EventResult::InferredOrderRemoval : EventResult::Applied;
    }

    EventResult LimitOrderBook::eraseOrder(long long orderId) {
        const auto orderIt = orders_.find(orderId);
        if (orderIt == orders_.end()) {
            return EventResult::UnknownOrder;
        }

        Order& order = orderIt->second;
        PriceLevel* const level = order.level;
        const MarketData::Price price = order.price;
        const MarketData::OrderDirection side = order.side;
        level->totalQuantity -= order.quantity;
        unlinkOrder(order);
        orders_.erase(orderIt);
        if (level->head == nullptr) {
            if (side == MarketData::OrderDirection::Buy) {
                bids_.erase(price);
            } else {
                asks_.erase(price);
            }
        }
        return EventResult::Applied;
    }

    EventResult LimitOrderBook::processEvent(const MarketData::OrderEvent& event) {
        switch (event.eventType) {
            case MarketData::EventType::NewOrder:
                return insertOrder(event);
            case MarketData::EventType::PartialCancellation:
            case MarketData::EventType::VisibleExecution:
                return reduceOrder(event.orderId, event.amount);
            case MarketData::EventType::FullDeletion:
                return eraseOrder(event.orderId);
            case MarketData::EventType::HiddenExecution:
                return EventResult::IgnoredHiddenExecution;
            case MarketData::EventType::Halt:
                return EventResult::IgnoredHalt;
        }
        return EventResult::IgnoredHalt;
    }

    std::vector<MarketData::PriceLevel> LimitOrderBook::bestBidLevels(std::size_t count) const {
        std::vector<MarketData::PriceLevel> result;
        result.reserve(std::min(count, bids_.size()));
        for (auto it = bids_.begin(); it != bids_.end() && result.size() < count; ++it) {
            result.push_back({it->first, it->second.totalQuantity});
        }
        return result;
    }

    std::vector<MarketData::PriceLevel> LimitOrderBook::bestAskLevels(std::size_t count) const {
        std::vector<MarketData::PriceLevel> result;
        result.reserve(std::min(count, asks_.size()));
        for (auto it = asks_.begin(); it != asks_.end() && result.size() < count; ++it) {
            result.push_back({it->first, it->second.totalQuantity});
        }
        return result;
    }

    MarketData::BookSnapshot LimitOrderBook::topOfBook() const {
        MarketData::BookSnapshot snapshot;
        if (!asks_.empty()) {
            snapshot.bestAsk = {asks_.begin()->first, asks_.begin()->second.totalQuantity};
        }
        if (!bids_.empty()) {
            snapshot.bestBid = {bids_.begin()->first, bids_.begin()->second.totalQuantity};
        }
        return snapshot;
    }

    std::size_t LimitOrderBook::totalOrders() const {
        return orders_.size();
    }

    bool LimitOrderBook::containsOrder(long long orderId) const {
        return orders_.contains(orderId);
    }

}  // namespace OrderBook