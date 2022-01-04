//
// Created by Jino on 2021/07/14.
//

#pragma once

#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
#include <map>

// Wrapper class of stb headers

using Resolution = std::pair<int, int>;


const std::vector<Resolution> res_list_4_3{
        {1280, 960},
        {1600, 1200},
        {1920, 1440},
        {2048, 1536},
        {2560, 1920},
        {3840, 2880},
        {4096, 3072},
        {5120, 3840},
        {7680, 5760}
};

const std::vector<Resolution> res_list_16_9{
        {1280, 720},
        {1600, 900},
        {1920, 1080},
        {2048, 1152},
        {2560, 1440},
        {3840, 2160},
        {4096, 2304},
        {5120, 2880},
        {7680, 4320}
};

const std::vector<Resolution> res_list_16_10{
        {1280, 800},
        {1600, 1000},
        {1920, 1200},
        {2048, 1280},
        {2560, 1600},
        {3840, 2400},
        {4096, 2560},
        {5120, 3200},
        {7680, 4800},
};

const std::map<Resolution, std::string> res_map{
    {{1280, 960},  "1280x960"},
    {{1600, 1200}, "1600x1200"},
    {{1920, 1440}, "1920x1440"},
    {{2048, 1536}, "2048x1536"},
    {{2560, 1920}, "2560x1920"},
    {{2880, 2160}, "2880x2160"},
    {{3840, 2880}, "3840x2880"},
    {{4096, 3072}, "4096x3072"},
    {{5120, 3840}, "5120x3840"},
    {{7680, 5760}, "7680x5760"},

    {{1280, 720},  "1280x720"},
    {{1600, 900},  "1600x900"},
    {{1920, 1080}, "1920x1080"},
    {{2048, 1152}, "2048x1152"},
    {{2560, 1440}, "2560x1440"},
    {{2880, 1620}, "2880x1620"},
    {{3840, 2160}, "3840x2160"},
    {{4096, 2304}, "4096x2304"},
    {{5120, 2880}, "5120x2880"},
    {{7680, 4320}, "7680x4320"},

    {{1280, 800},  "1280x800"},
    {{1600, 1000}, "1600x1000"},
    {{1920, 1200}, "1920x1200"},
    {{2048, 1280}, "2048x1280"},
    {{2560, 1600}, "2560x1600"},
    {{2880, 1800}, "2880x1800"},
    {{3840, 2400}, "3840x2400"},
    {{4096, 2560}, "4096x2560"},
    {{5120, 3200}, "5120x3200"},
    {{7680, 4800}, "7680x4800"}
};

const std::map<float, std::string> height_ratio_map{
    {0.1f, "10%"},
    {0.2f, "20%"},
    {0.3f, "30%"},
    {0.4f, "40%"},
    {0.5f, "50%"},
    {0.6f, "60%"},
    {0.7f, "70%"},
    {0.8f, "80%"},
    {0.9f, "90%"},
    {1.0f, "100%"},
};


class Image{
public:
    explicit Image(const std::string &binary);

    // Not allow copy
    Image(const Image &) = delete;
    Image& operator=(const Image &) = delete;

    ~Image(){
        free(m_data);
    }

    void set_as_wallpaper(const std::string &filename) const;

    void to_any_resolution(int width, int height, float earth_height_ratio);

    void write_png(const std::string &filename) const;

    // Image is available it has non-zero width/height, and 3 or 4 color channel.
    inline bool is_available() const{
        return (m_w != 0) && (m_h != 0) && (m_channel == 3 || m_channel == 4);
    }

private:
    void resize_preserve_ratio(int height);
    void resize(int width, int height);
    void remove_alpha();
    void fix_pixel(int w, int h, int r, int g, int b, int a=1);

    // FIXME : It works only with FD
    void remove_description();

    // FIXME : It works only with FD
    void remove_watermark();

    // Add border at top/bottom of the image
    // pixel value : (0, 0, 0, (0))
    void add_top_bot_border(int size);

    // Add border at left/right of the image
    // pixel value : (0, 0, 0, (0))
    void add_left_right_border(int size);

    int m_w;
    int m_h;
    int m_channel;
    unsigned char *m_data;
};
