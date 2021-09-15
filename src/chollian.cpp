//
// Created by Jino on 2021/07/14.
//

#include <curl/curl.h>
#include "chollian.h"

std::string url_generator_chollian(const ImageType &imageType, const Color &color, const UTCTime &date) {
    std::string base_url = "https://nmsc.kma.go.kr/IMG/GK2A/AMI/PRIMARY/L1B/COMPLETE/";

    auto [year, month, day, hours, minutes] = date.extract_component();

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

    const std::string url = base_url + sub1 + sub2 + sub3 + sub4 + sub5 + sub6 + sub7;

    return url;
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string image_downloader(const std::string &url) {

    CURL *curl = curl_easy_init();
    CURLcode res;
    std::string readBuffer;
    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

