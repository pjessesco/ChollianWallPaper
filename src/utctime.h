//
// Created by Jino on 2021/07/22.
//

#pragma once

// TODO : <ctime> should be replaced by <chrono> after C++20 is fully implemented (std::chrono::utc_clock)
#include <ctime>

// Wrapper class of time-related libraries
class UTCTime {
public:
    UTCTime();

    // New URL is generated per 10 min
    void adjust_target_time();

    inline auto extract_component() const {
        return std::make_tuple(std::to_string(m_utc_time->tm_year + 1900),
                               add_zero_padding(m_utc_time->tm_mon + 1),
                               add_zero_padding(m_utc_time->tm_mday),
                               add_zero_padding(m_utc_time->tm_hour),
                               add_zero_padding(m_utc_time->tm_min));
    }

private:
    inline std::string add_zero_padding(int num) const {
        return num<10?"0"+std::to_string(num):std::to_string(num);
    }

    tm *m_utc_time;
};
