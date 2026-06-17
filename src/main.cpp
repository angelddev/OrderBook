#include  "../include/CsvParser.h"
#include <iostream>

int main(){

    std::vector<MarketData::OrderEvent> Events = MarketData::parseCsvFile("../data/OrderBookActions_APPLE.csv");
    if(Events.empty()){
        std::cout << "Couldn't register any events.\n";
        exit(1);
    }

    for(int i = 0; i < 5; i++){
        MarketData::OrderEvent temp = Events[i];
        std::cout << "At time " << temp.timestamp << ", there have been "<< temp.amount << " stocks updated with ID: "<< temp.orderId << "\n";
    }

}