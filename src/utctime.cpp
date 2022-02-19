//
// Created by Jino on 2021/07/22.
//

#include <ctime>

#include "utctime.h"

UTCTime::UTCTime() {
    std::time_t result = std::time(nullptr);
    m_utc_time = std::gmtime(&result);
}

void UTCTime::adjust_target_time() {
    const int remainder = m_utc_time->tm_min % 10;

    if(remainder<5){
        m_utc_time->tm_min -= (20 + remainder);
    }
    else{
        m_utc_time->tm_min -= (10 + remainder);
    }
    mktime(m_utc_time);
}
