#include "../include/CsvParser.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace {

    std::string trim(const std::string& value) {
        std::size_t start = 0;
        while (start < value.size() && std::isspace(static_cast<unsigned char>(value[start])) != 0) {
            ++start;
        }

        std::size_t end = value.size();
        while (end > start && std::isspace(static_cast<unsigned char>(value[end - 1])) != 0) {
            --end;
        }

        return value.substr(start, end - start);
    }

    bool parseEventType(const std::string& token, MarketData::EventType& eventType) {
        try {
            const int value = std::stoi(token);
            switch (value) {
                case 1: eventType = MarketData::EventType::NewOrder; return true;
                case 2: eventType = MarketData::EventType::PartialCancellation; return true;
                case 3: eventType = MarketData::EventType::FullDeletion; return true;
                case 4: eventType = MarketData::EventType::VisibleExecution; return true;
                case 5: eventType = MarketData::EventType::HiddenExecution; return true;
                case 7: eventType = MarketData::EventType::Halt; return true;
                default: return false;
            }
        } catch (...) {
            return false;
        }
    }

    bool parseDirection(const std::string& token, MarketData::OrderDirection& direction) {
        if (token == "1") {
            direction = MarketData::OrderDirection::Buy;
            return true;
        }
        if (token == "-1") {
            direction = MarketData::OrderDirection::Sell;
            return true;
        }
        return false;
    }

}  // namespace

std::vector<MarketData::OrderEvent> MarketData::parseCsvFile(std::string filePath) {
    std::vector<MarketData::OrderEvent> events;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "Couldn't open file.\n";
        return events;
    }

    std::string line;
    std::size_t skippedRows = 0;

    while (std::getline(file, line)) {
        const std::string trimmedLine = trim(line);
        if (trimmedLine.empty() || trimmedLine[0] == '#') {
            ++skippedRows;
            continue;
        }

        std::stringstream information(trimmedLine);
        std::string token;
        std::vector<std::string> columns;

        while (std::getline(information, token, ',')) {
            columns.push_back(trim(token));
        }

        if (columns.size() != 6) {
            ++skippedRows;
            continue;
        }

        MarketData::OrderEvent currentEvent;
        bool validRow = true;

        for (std::size_t columnIndex = 0; columnIndex < columns.size(); ++columnIndex) {
            const std::string& columnValue = columns[columnIndex];
            if (columnValue.empty()) {
                validRow = false;
                break;
            }

            switch (static_cast<MarketData::OrderColumn>(columnIndex)) {
                case MarketData::OrderColumn::TIMESTAMP: {
                    try {
                        currentEvent.timestamp = std::stod(columnValue);
                    } catch (...) {
                        validRow = false;
                    }
                    break;
                }
                case MarketData::OrderColumn::EVENT: {
                    validRow = parseEventType(columnValue, currentEvent.eventType);
                    break;
                }
                case MarketData::OrderColumn::ID: {
                    try {
                        currentEvent.orderId = std::stoll(columnValue);
                    } catch (...) {
                        validRow = false;
                    }
                    break;
                }
                case MarketData::OrderColumn::AMOUNT: {
                    try {
                        currentEvent.amount = std::stoi(columnValue);
                    } catch (...) {
                        validRow = false;
                    }
                    break;
                }
                case MarketData::OrderColumn::PRICE: {
                    try {
                        currentEvent.price = std::stod(columnValue);
                    } catch (...) {
                        validRow = false;
                    }
                    break;
                }
                case MarketData::OrderColumn::DIRECTION: {
                    validRow = parseDirection(columnValue, currentEvent.direction);
                    break;
                }
            }

            if (!validRow) {
                break;
            }
        }

        if (!validRow) {
            ++skippedRows;
            continue;
        }

        events.push_back(currentEvent);
    }

    if (skippedRows > 0) {
        std::cout << "Skipped " << skippedRows << " invalid row(s) while parsing CSV.\n";
    }

    std::cout << "Parsed " << events.size() << " valid events from " << filePath << ".\n";
    return events;
}