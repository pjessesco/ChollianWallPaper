//
// Created by Jino on 2021/07/14.
//

#pragma once

#include <string>
#include <iostream>
#include <tuple>

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


private:
    int m_w;
    int m_h;
    int m_channel;
    unsigned char *m_data;
};





