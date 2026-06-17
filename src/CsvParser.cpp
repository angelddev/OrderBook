#include "../include/CsvParser.h"
#include <iostream>
#include <fstream>
#include <sstream>


std::vector<MarketData::OrderEvent> MarketData::parseCsvFile(std::string filePath){

    std::ifstream file(filePath);
    if(!file.is_open()){
        std::cout << "Couldn't open file.\n";
        return;
    }

    std::string line;
    int cont=1;

    while(getline(file, line)&&cont<5){

        int column = 0;
        std::stringstream information(line);
        std::string token;
        while(getline(information, token, ',')){
            switch(static_cast<MarketData::OrderColumn>(column)){
                case MarketData::OrderColumn::TIMESTAMP :
                std::cout << "This is the amount of time (up to microseconds) of the purchase after 12am: " << token << "\n";  
                break;
                case MarketData::OrderColumn::EVENT : 
                std::cout << "This is the type of event: " << token << "\n"; 
                break;
                case MarketData::OrderColumn::ID :
                std::cout << "This is its ID: " << token << "\n"; 
                break;
                case MarketData::OrderColumn::AMOUNT :
                std::cout << "This is the amount of this stock available: " << token << "\n"; 
                break;
                case MarketData::OrderColumn::PRICE :
                std::cout << "This is the price per stock: " << token << "\n"; 
                break;
                case MarketData::OrderColumn::DIRECTION :
                std::cout << "This is the direciton of this event: " << token << "\n"; 
                break;
            }
            column++;
        }
        cont++;
    }

    return;
}