#include <iostream>
#include <string>
#include <tuple>

#include <boost/locale.hpp>
#include <boost/date_time.hpp>

#include <curl/curl.h>

enum class ImageType{
    FullDome = 1,
    EastAsia = 2
};

enum class Color{
    True = 1,
    Natural = 2
};

inline std::string add_zero_padding(int num){
    return num<10?"0"+std::to_string(num):std::to_string(num);
}

inline auto extract_component(const boost::posix_time::ptime &time){
    return std::make_tuple(std::to_string(time.date().year()),
                           add_zero_padding(time.date().month()),
                           add_zero_padding(time.date().day()),
                           add_zero_padding(time.time_of_day().hours()),
                           add_zero_padding(time.time_of_day().minutes()));
}

std::string url_generator_chollian(const ImageType &imageType,
                                   const Color &color,
                                   const boost::posix_time::ptime &date){
    std::string base_url = "https://nmsc.kma.go.kr/IMG/GK2A/AMI/PRIMARY/L1B/COMPLETE/";

    auto [year, month, day, hours, minutes] = extract_component(date);

    std::string sub1, sub2, sub3, sub4, sub5, sub6, sub7;

    switch (imageType) {
        case ImageType::FullDome:
            sub1 = "FD/";
            sub4 = "fd";
            sub6 = "ge_";
            break;
        case ImageType::EastAsia:
            sub1 = "EA/";
            sub4 = "ea";
            sub6 = "lc_";
            break;
    }

    switch(color){
        case Color::True:
            sub3 = "rgb-true_";
            sub5 = "010";
            break;
        case Color::Natural:
            sub3 = "rgb-natural_";
            sub5 = "020";
            break;
    }

    sub2 = year + month + "/" + day + "/" + hours + "/gk2a_ami_le1b_";
    sub7 = year + month + day + hours + minutes + ".png";

    std::string url = base_url + sub1 + sub2 + sub3 + sub4 + sub5 + sub6 + sub7;

    return url;
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void image_downloader(const std::string &url){

    CURL *curl = curl_easy_init();
    CURLcode res;
    std::string readBuffer;
    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        std::ofstream img("img.png");
        img<<readBuffer;
        img.close();
    }
}

// New URL is generated per 10 min
auto adjust_target_time(const boost::posix_time::ptime &time){

    int remainder = time.time_of_day().minutes() % 10;
    auto new_time = time;
    if(remainder<5){
        new_time -= boost::posix_time::minutes(20 + remainder);
    }
    else{
        new_time -=  boost::posix_time::minutes(10 + remainder);
    }
    return new_time;
}

int main() {
    std::cout << "Get recent chollian 2a satelite image" << std::endl;

    boost::posix_time::ptime utcTime = boost::posix_time::second_clock::universal_time();
    auto adjusted_time = adjust_target_time(utcTime);

    int area;
    int col;

    std::cout<<"1 : FD, 2 : EA : ";
    std::cin>>area;

    std::cout<<"1 : True, 2 : Natural : ";
    std::cin>>col;

    std::string url = url_generator_chollian(ImageType(area), Color(col), adjusted_time);

    std::cout<<url<<std::endl;
    image_downloader(url);

    return 0;
}
