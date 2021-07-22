//
// Created by Jino on 2021/07/22.
//

#pragma once

#include <boost/locale.hpp>
#include <boost/date_time.hpp>

// Wrapper class of boost time-related libraries
class UTCTime {
public:
    UTCTime();

    // New URL is generated per 10 min
    void adjust_target_time();

    inline auto extract_component() const {
        return std::make_tuple(std::to_string(m_utc_time.date().year()),
                               add_zero_padding(m_utc_time.date().month()),
                               add_zero_padding(m_utc_time.date().day()),
                               add_zero_padding(m_utc_time.time_of_day().hours()),
                               add_zero_padding(m_utc_time.time_of_day().minutes()));
    }

private:
    inline std::string add_zero_padding(int num) const {
        return num<10?"0"+std::to_string(num):std::to_string(num);
    }

    boost::posix_time::ptime m_utc_time;
};
