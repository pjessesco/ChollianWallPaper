//
// Created by Jino on 2021/07/14.
//

#include <curl/curl.h>
#include "downloader.h"
#include "logger.h"

std::string url_generator_chollian(DownloadOption option, ImageType imageType, Color color, const UTCTime &date) {
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
    sub7 = year + month + day + hours + minutes + (option==DownloadOption::Quality?".png":".srv.png");

    return base_url + sub1 + sub2 + sub3 + sub4 + sub5 + sub6 + sub7;
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
        LOG("Downloading... ");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if(res == CURLE_OK){
            LOG("curl_easy_perform() success");
            return readBuffer;
        }
        else{
            LOG("curl_easy_perform() failed: " + std::string(curl_easy_strerror(res)));
        }
    }
    else{
        LOG("Curl is not initialized");
    }
    return "-1";
}


std::string generate_filename(const UTCTime &time,
                              Color color,
                              ImageType img_type,
                              int width,
                              int height){

    auto [year, month, day, hours, minutes] = time.extract_component();
    std::string color_str;
    switch(color){
        case Color::True:
            color_str = "true";
            break;
        case Color::Natural:
            color_str = "natural";
            break;
    }

    std::string img_type_str;
    switch(img_type){
        case ImageType::FullDome:
            img_type_str = "fd";
            break;
        case ImageType::EastAsia:
            img_type_str = "ea";
            break;
    }

    return year + month + day + hours + minutes + "_" +
           color_str + "_" + img_type_str +
           std::to_string(width) + std::to_string(height) + ".png";
}
