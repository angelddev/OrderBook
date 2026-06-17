#pragma once
#include<string>
#include <vector>


namespace MarketData {

    enum class OrderDirection {
        Buy = 1,
        Sell = 2
    };

    struct OrderEvent {
        double timestamp;
        int eventType;
        long long orderId;
        int amount;
        double price;
        OrderDirection direction;
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
