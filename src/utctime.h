// Every satellite images used in this project are copyrighted by the Korea Meteorological
// Administration, and they can NOT be used for commercial purposes.
//
// Chollian Wallpaper is distributed under the MIT License.
//
// Copyright (c) 2021-2022 Jino Park
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

// TODO : <ctime> should be replaced by <chrono> after C++20 is fully implemented (std::chrono::utc_clock)
#include <ctime>
#include <string>

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
    inline static std::string add_zero_padding(int num) {
        return num<10?"0"+std::to_string(num):std::to_string(num);
    }

    tm *m_utc_time;
};
