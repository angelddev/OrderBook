#include "CsvParser.h"

#include <array>
#include <charconv>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string_view>

namespace {

    using Fields = std::array<std::string_view, 6>;

    std::string_view trim(std::string_view value) {
        while (!value.empty() && (value.front() == ' ' || value.front() == '\t' || value.front() == '\r')) {
            value.remove_prefix(1);
        }
        while (!value.empty() && (value.back() == ' ' || value.back() == '\t' || value.back() == '\r')) {
            value.remove_suffix(1);
        }
        return value;
    }

    bool splitCsvRow(const std::string& line, Fields& fields) {
        std::string_view remaining(line);
        for (std::size_t index = 0; index < fields.size(); ++index) {
            const std::size_t comma = remaining.find(',');
            fields[index] = trim(remaining.substr(0, comma));
            if (index + 1 < fields.size()) {
                if (comma == std::string_view::npos) {
                    return false;
                }
                remaining.remove_prefix(comma + 1);
            } else if (comma != std::string_view::npos) {
                return false;
            }
        }
        return true;
    }

    template <typename Integer>
    bool parseInteger(std::string_view text, Integer& value) {
        if (text.empty()) {
            return false;
        }
        const char* begin = text.data();
        const auto parsed = std::from_chars(begin, begin + text.size(), value);
        return parsed.ec == std::errc{} && parsed.ptr == begin + text.size();
    }

    bool parseTimestamp(std::string_view text, MarketData::TimestampNanoseconds& result) {
        const std::size_t point = text.find('.');
        std::int64_t seconds = 0;
        if (!parseInteger(text.substr(0, point), seconds) || seconds < 0) {
            return false;
        }

        constexpr std::int64_t scale = 1000000000LL;
        constexpr std::int64_t maxFraction = scale - 1;
        if (seconds > (std::numeric_limits<std::int64_t>::max() - maxFraction) / scale) {
            return false;
        }

        std::int64_t fraction = 0;
        if (point != std::string_view::npos) {
            const std::string_view digits = text.substr(point + 1);
            if (digits.empty() || digits.size() > 9 || !parseInteger(digits, fraction)) {
                return false;
            }
            for (std::size_t count = digits.size(); count < 9; ++count) {
                fraction *= 10;
            }
        }

        result = seconds * scale + fraction;
        return true;
    }

    bool parseEventType(std::string_view text, MarketData::EventType& type) {
        int value = 0;
        if (!parseInteger(text, value)) {
            return false;
        }
        switch (value) {
            case 1: type = MarketData::EventType::NewOrder; return true;
            case 2: type = MarketData::EventType::PartialCancellation; return true;
            case 3: type = MarketData::EventType::FullDeletion; return true;
            case 4: type = MarketData::EventType::VisibleExecution; return true;
            case 5: type = MarketData::EventType::HiddenExecution; return true;
            case 7: type = MarketData::EventType::Halt; return true;
            default: return false;
        }
    }

    bool parseMessageRow(const Fields& fields, MarketData::OrderEvent& event) {
        if (!parseTimestamp(fields[0], event.timestamp) ||
            !parseEventType(fields[1], event.eventType) ||
            !parseInteger(fields[2], event.orderId) ||
            !parseInteger(fields[3], event.amount) ||
            !parseInteger(fields[4], event.price)) {
            return false;
        }

        int direction = 0;
        if (!parseInteger(fields[5], direction) || (direction != 1 && direction != -1)) {
            return false;
        }
        event.direction = direction == 1 ? MarketData::OrderDirection::Buy : MarketData::OrderDirection::Sell;

        if (event.eventType == MarketData::EventType::Halt) {
            return event.orderId == 0 && event.amount == 0 && event.price >= -1 && event.price <= 1 && direction == -1;
        }
        if (event.amount <= 0 || event.price < 0) {
            return false;
        }
        if (event.eventType == MarketData::EventType::HiddenExecution) {
            return event.orderId == 0;
        }
        return event.orderId > 0;
    }

}  // namespace

MarketData::CsvParseReport<MarketData::OrderEvent> MarketData::parseMessageCsv(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open CSV file: " + filePath);
    }

    CsvParseReport<OrderEvent> report;
    std::string line;
    std::size_t lineNumber = 0;
    while (std::getline(file, line)) {
        ++lineNumber;
        if (trim(line).empty()) {
            continue;
        }

        Fields fields;
        OrderEvent event;
        if (!splitCsvRow(line, fields) || !parseMessageRow(fields, event)) {
            ++report.skippedRows;
            if (report.firstInvalidLine == 0) {
                report.firstInvalidLine = lineNumber;
            }
            continue;
        }
        report.rows.push_back(event);
    }

    if (file.bad()) {
        throw std::runtime_error("I/O error while reading CSV file: " + filePath);
    }
    return report;
}