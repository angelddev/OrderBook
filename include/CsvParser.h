#pragma once

#include <string>
#include <vector>

namespace MarketData {

    enum class OrderDirection {
        Buy = 1,
        Sell = -1
    };

    enum class EventType {
        NewOrder = 1,
        PartialCancellation = 2,
        FullDeletion = 3,
        VisibleExecution = 4,
        HiddenExecution = 5,
        Halt = 7
    };

    struct OrderEvent {
        double timestamp = 0.0;
        EventType eventType = EventType::NewOrder;
        long long orderId = 0;
        int amount = 0;
        double price = 0.0;
        OrderDirection direction = OrderDirection::Buy;
    };

    enum class OrderColumn {
        TIMESTAMP = 0,
        EVENT = 1,
        ID = 2,
        AMOUNT = 3,
        PRICE = 4,
        DIRECTION = 5
    };

    std::vector<OrderEvent> parseCsvFile(std::string filePath);

} 
