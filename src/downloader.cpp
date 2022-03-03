//
// Created by Jino on 2021/07/14.
//

#include <tuple>
#include <regex>

#include <curl/curl.h>

#include "downloader.h"
#include "logger.h"

std::string url_generator_chollian(DownloadOption option, Color color, const UTCTime &date) {
    const std::string base_url = "https://nmsc.kma.go.kr/IMG/GK2A/AMI/PRIMARY/L1B/COMPLETE/";

    auto [year, month, day, hours, minutes] = date.extract_component();

    std::string sub1, sub2, sub3, sub4;

    switch(color){
        case Color::True:
            sub2 = "rgb-true_";
            sub3 = "010";
            break;
        case Color::Natural:
            sub2 = "rgb-natural_";
            sub3 = "020";
            break;
        case Color::WaterVapor:
            sub2 = "rgb-wv1_";
            sub3 = "020";
            break;
        case Color::Cloud:
            sub2 = "rgb-cloud_";
            sub3 = "020";
            break;
        case Color::Ash:
            sub2 = "rgb-ash_";
            sub3 = "020";
    }

    sub1 = year + month + "/" + day + "/" + hours + "/gk2a_ami_le1b_";
    sub4 = year + month + day + hours + minutes + (option == DownloadOption::Quality ? ".png" : ".srv.png");

    return base_url + "FD/" + sub1 + sub2 + "fd" + sub3 + "ge_" + sub4;
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string download_curl(const std::string &url, struct curl_slist *header){
    CURL *curl = curl_easy_init();
    CURLcode res;
    std::string readBuffer;
    if(curl){
        LOG("Downloading... ");
        if(header){
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
        }
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if(res == CURLE_OK){
            LOG("curl_easy_perform() success");
            LOG("readbuffer : " + readBuffer);
            return readBuffer;
        }
        else{
            LOG("curl_easy_perform() failed: " + std::string(curl_easy_strerror(res)));
        }
    }
    else{
        LOG("Curl is not initialized");
    }

    curl_easy_cleanup(curl);
    return "-1";
}

std::string image_downloader(DownloadOption option, Color color, const UTCTime &date) {

    const std::string url = url_generator_chollian(option, color, date);
    return download_curl(url, nullptr);
}

// Refer https://docs.github.com/en/rest/reference/releases#get-the-latest-release
std::string get_latest_version(){

    const std::string url = "https://api.github.com/repos/pjessesco/ChollianWallPaper/releases/latest";
    struct curl_slist *slist1 = NULL;
    slist1 = curl_slist_append(slist1, "Accept: application/vnd.github.v3+json");
    slist1 = curl_slist_append(slist1, ("user-agent: curl/" + std::string(CURL_VERSION)).c_str());

    std::string json = download_curl(url, slist1);
    std::string version = "Fail";

    std::cout<<json<<std::endl;
    std::regex re(R"(\"tag_name\": \"\d{4}.\d{2}\")");
    std::smatch match;
    if(std::regex_search(json, match, re)){
        version = match.str().substr(13,7);
    }
    return version;
}

std::string generate_filename(const UTCTime &time,
                              Color color,
                              int width,
                              int height,
                              float height_ratio){

    auto [year, month, day, hours, minutes] = time.extract_component();
    std::string color_str;
    switch(color){
        case Color::True:
            color_str = "true";
            break;
        case Color::Natural:
            color_str = "natural";
            break;
        case Color::WaterVapor:
            color_str = "watervapor";
            break;
        case Color::Cloud:
            color_str = "cloud";
            break;
        case Color::Ash:
            color_str = "ash";
            break;
    }

    return year + month + day + hours + minutes + "_" +
           color_str + "_fd" +
           std::to_string(width) + std::to_string(height) + std::to_string(height_ratio) + ".png";
}
