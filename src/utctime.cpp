//
// Created by Jino on 2021/07/22.
//

#include <tuple>

#include "utctime.h"

UTCTime::UTCTime() : m_utc_time(boost::posix_time::second_clock::universal_time()) {}

void UTCTime::adjust_target_time() {
    const int remainder = m_utc_time.time_of_day().minutes() % 10;
    auto new_time = m_utc_time;
    if(remainder<5){
        new_time -= boost::posix_time::minutes(20 + remainder);
    }
    else{
        new_time -=  boost::posix_time::minutes(10 + remainder);
    }
    m_utc_time = new_time;
}
