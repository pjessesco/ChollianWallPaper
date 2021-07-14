//
// Created by Jino on 2021/07/14.
//

#pragma once

#include <string>
#include <iostream>
#include <tuple>
#include <cstdlib> // for calloc

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>


// Wrapper class of stb headers

class Image{
public:
    Image(const std::string &binary){
        m_data = stbi_load_from_memory((stbi_uc *)(binary.c_str()), binary.length(), &m_w, &m_h, &m_channel, 4);
    }

    // Not allow copy
    Image(const Image &) = delete;
    Image& operator=(const Image &) = delete;

    ~Image(){
        free(m_data);
    }

    void write_png(const std::string &filename) const{
        stbi_write_png(filename.c_str(), m_w, m_h, m_channel, m_data, m_w * m_channel);
    }

    void resize(int width, int height){
        unsigned char *resized_img = (unsigned char *)malloc(width * height * m_channel * sizeof(unsigned char));
        stbir_resize_uint8(m_data, m_w, m_h, 0, resized_img, width, height, 0, m_channel);
        free(m_data);
        m_data = resized_img;
        m_w = width;
        m_h = height;
    }

    void remove_alpha(){
        if(m_channel == 4){
            unsigned char *rgb_img = (unsigned char *)malloc(m_w * m_h * 3 * sizeof(unsigned char));
            int j = 0;
            for(int i=0;i<m_w * m_h * 4;i++){
                if(i%4 == 3){
                    continue;
                }
                rgb_img[j] = m_data[i];
                j++;
            }
            free(m_data);
            m_data = rgb_img;
            m_channel = 3;
        }
    }

    void fix_pixel(int w, int h, int r, int g, int b, int a=1){
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

    // FIXME : It works only with FD
    void remove_description(){
        for(int i=0;i<m_w;i++){
            for(int j=0;j<m_w*0.03;j++){
                fix_pixel(i, j, 0, 0, 0, 0);
            }
        }
    }

    // FIXME : It works only with FD
    void remove_watermark(){
        for(int i=0;i<m_w*0.156;i++){
            for(int j=m_h-1; j>m_h*0.962;j--){
                fix_pixel(i, j, 0, 0, 0, 0);
            }
        }
    }

    // Add border at top/bottom of the image
    // pixel value : (0, 0, 0, (0))
    void add_top_bot_border(int size){
        int new_height = m_h + 2 * size;
        unsigned char *new_img = (unsigned char *)calloc(m_w * new_height * m_channel, sizeof(unsigned char));
        memcpy(new_img + (size * m_w * m_channel), m_data, m_w * m_h * m_channel);

        free(m_data);
        m_data = new_img;
        m_h = new_height;
    }

    // Add border at left/right of the image
    // pixel value : (0, 0, 0, (0))
    void add_left_right_border(int size){
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


private:
    int m_w;
    int m_h;
    int m_channel;
    unsigned char *m_data;
};





