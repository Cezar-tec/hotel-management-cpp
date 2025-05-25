#include "exceptions.h"

#include <iostream>
#include <ctime>

namespace ExceptionLogger {
    void logException(const HotelException& e) {
        std::time_t now = std::time(0);
        std::tm* timeinfo = std::localtime(&now);
        
        std::cerr << "[" << (timeinfo->tm_year + 1900) << "-" 
                  << (timeinfo->tm_mon + 1) << "-" << timeinfo->tm_mday 
                  << " " << timeinfo->tm_hour << ":" << timeinfo->tm_min 
                  << ":" << timeinfo->tm_sec << "] ";
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
    }
}