//
// Created by Jino on 2021/07/14.
//

#pragma once

#include <string>
#include <cstdlib>
#include <sstream>

// Wrapper class of stb headers

class Image{
public:
    Image(const std::string &binary);

    // Not allow copy
    Image(const Image &) = delete;
    Image& operator=(const Image &) = delete;

    ~Image(){
        free(m_data);
    }

    void set_as_wallpaper(const std::string &filename);

    // Generate 1920 x 1080 image
    void to_any_resolution(int width, int height, int top_bot_border);


private:
    void write_png(const std::string &filename) const;
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
