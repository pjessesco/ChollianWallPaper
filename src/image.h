//
// Created by Jino on 2021/07/14.
//

#pragma once

#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>

// Wrapper class of stb headers

using Resolution = std::pair<int, int>;


const std::vector<Resolution> res_list_4_3{
        {640, 480},
        {800, 600},
        {1024, 768},
        {1280, 960},
        {1600, 1200},
        {1920, 1440},
        {2048, 1536},
        {2560, 1920},
        {4096, 3072},
        {6400, 4800}
};

const std::vector<Resolution> res_list_16_9{
        {640, 360},
        {960, 540},
        {1280, 720},
        {1600, 900},
        {1920, 1080},
        {2048, 1152},
        {2560, 1440},
        {3840, 2160},
        {5120, 2880},
        {7680, 4320}
};

const std::vector<Resolution> res_list_16_10{
        {1280, 800},
        {1440, 900},
        {1920, 1200},
        {2560, 1600},
        {2880, 1800},
        {3840, 2400}
};


class Image{
public:
    Image(const std::string &binary);

    // Not allow copy
    Image(const Image &) = delete;
    Image& operator=(const Image &) = delete;

    ~Image(){
        free(m_data);
    }

    void set_as_wallpaper(const std::string &filename) const;

    void to_any_resolution(int width, int height, int top_bot_border);

    void write_png(const std::string &filename) const;

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
