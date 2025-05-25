#pragma once
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace utils {
    
    inline std::string trim(const std::string& str) {
        auto start = str.begin();
        while (start != str.end() && std::isspace(*start)) {
            start++;
        }
        
        auto end = str.end();
        do {
            end--;
        } while (std::distance(start, end) > 0 && std::isspace(*end));
        
        return std::string(start, end + 1);
    }
    
    template<typename T>
    std::string toString(const T& value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
    
    inline bool isValidString(const std::string& str) {
        return !trim(str).empty();
    }
    
    inline std::string formatPrice(double price) {
        std::ostringstream oss;
        oss.precision(2);
        oss << std::fixed << price << " RON";
        return oss.str();
    }
}