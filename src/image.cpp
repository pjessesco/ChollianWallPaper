//
// Created by Jino on 2021/07/14.
//

#include "image.h"

#include <string>
#include <tuple>
#include <cstdlib> // for calloc
#include <filesystem>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Image::Image(const std::string &binary) {
    m_data = stbi_load_from_memory((unsigned char*)(binary.c_str()), binary.length(), &m_w, &m_h, &m_channel, 4);
}

void Image::set_as_wallpaper(const std::string &filename) const{

#ifdef __APPLE__
    // Save image if an executable is in the bundle package.

    std::string bundle_resource_path = std::filesystem::current_path().string() + "/../Resources/";
    if(std::filesystem::exists(bundle_resource_path)){
        write_png(bundle_resource_path + filename);
    }

    std::stringstream ss;

    ss <<"'tell application \"System Events\"\n"
       <<"set theDesktops to a reference to every desktop\n"
       <<"repeat with aDesktop in theDesktops\n"
       <<"set the picture of aDesktop to \"" + bundle_resource_path + filename + "\"\n"
       <<"end repeat\n"
       <<"end tell'\n";

    std::string command = "osascript -e "+ ss.str();
    std::system(command.c_str());
#endif
}

void Image::to_any_resolution(int width, int height, int top_bot_border) {
    remove_alpha();
    remove_description();
    remove_watermark();

    resize_preserve_ratio(height - 2 * top_bot_border);
    add_top_bot_border(top_bot_border);

    add_left_right_border((width - m_w) / 2);
}

void Image::write_png(const std::string &filename) const {
    stbi_write_png(filename.c_str(), m_w, m_h, m_channel, m_data, m_w * m_channel);
}

void Image::resize_preserve_ratio(int height) {
    float ratio = (float)height / m_h;
    resize((int)(m_w * ratio), height);
}

void Image::resize(int width, int height) {
    unsigned char *resized_img = (unsigned char *)malloc(width * height * m_channel * sizeof(unsigned char));
    stbir_resize_uint8(m_data, m_w, m_h, 0, resized_img, width, height, 0, m_channel);
    free(m_data);
    m_data = resized_img;
    m_w = width;
    m_h = height;
}

void Image::remove_alpha() {
    if(m_channel == 4){
        unsigned char *rgb_img = (unsigned char *)malloc(m_w * m_h * 3 * sizeof(unsigned char));
        int j = 0;
        for(int i=0;i<m_w * m_h;i++){
            rgb_img[j++] = m_data[4*i];
            rgb_img[j++] = m_data[4*i+1];
            rgb_img[j++] = m_data[4*i+2];
        }
        free(m_data);
        m_data = rgb_img;
        m_channel = 3;
    }
}

void Image::fix_pixel(int w, int h, int r, int g, int b, int a) {
    int pixel_idx = h * m_w + w;
    if(m_channel == 4){
        m_data[pixel_idx*4] = r;
        m_data[pixel_idx*4 + 1] = g;
        m_data[pixel_idx*4 + 2] = b;
        m_data[pixel_idx*4 + 3] = a;
    }
    else if(m_channel == 3){
        m_data[pixel_idx*3] = r;
        m_data[pixel_idx*3 + 1] = g;
        m_data[pixel_idx*3 + 2] = b;
    }
    else{
        exit(-1);
    }
}

void Image::remove_description() {
    for(int i=0;i<m_w;i++){
        for(int j=0;j<m_w*0.03;j++){
            fix_pixel(i, j, 0, 0, 0, 0);
        }
    }
}

void Image::remove_watermark() {
    for(int i=0;i<m_w*0.156;i++){
        for(int j=m_h-1; j>m_h*0.962;j--){
            fix_pixel(i, j, 0, 0, 0, 0);
        }
    }
}

void Image::add_top_bot_border(int size) {
    int new_height = m_h + 2 * size;
    unsigned char *new_img = (unsigned char *)calloc(m_w * new_height * m_channel, sizeof(unsigned char));
    memcpy(new_img + (size * m_w * m_channel), m_data, m_w * m_h * m_channel);

    free(m_data);
    m_data = new_img;
    m_h = new_height;
}

void Image::add_left_right_border(int size) {
    int new_width = m_w + 2 * size;
    unsigned char *new_img = (unsigned char *)calloc(new_width * m_h * m_channel, sizeof(unsigned char));

    for(int i=0;i<m_h;i++){
        int src_pixel_idx = i * m_w;
        int dst_pixel_idx = i * new_width + size;
        memcpy(new_img + (dst_pixel_idx * m_channel), m_data + (src_pixel_idx * m_channel), m_w * m_channel);
    }

    free(m_data);
    m_data = new_img;
    m_w = new_width;
}
