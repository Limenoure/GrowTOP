#ifndef UTILS__TIMEUTILS__HPP
#define UTILS__TIMEUTILS__HPP

#include <string>
#include <chrono>
#include <ctime>

static std::string GetTimeAsString() 
{
    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm localTime = *std::localtime(&time);

    std::stringstream ss;
    ss << std::put_time(&localTime, "%d/%m %H:%M:%S");

    return ss.str();
}

#endif // UTILS__TIMEUTILS__HPP