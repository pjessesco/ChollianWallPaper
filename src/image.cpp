// Every satellite images used in this project are copyrighted by the Korea Meteorological
// Administration, and they can NOT be used for commercial purposes.
//
// Chollian Wallpaper is distributed under the MIT License.
//
// Copyright (c) 2021-2022 Jino Park
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <string>
#include <vector>

#if defined _WIN32
    #include <windows.h>
#endif

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "image.h"
#include "logger.h"

Image::Image(const std::string &binary) : m_w(0), m_h(0), m_channel(0) {
    unsigned char *data = stbi_load_from_memory((unsigned char*)(binary.c_str()), binary.length(), &m_w, &m_h, &m_channel, 4);
    m_data = std::vector<unsigned char>(data, data + (m_w * m_h * m_channel));
    free(data);
}

void Image::set_as_wallpaper(const std::string &filename) {

#if defined __APPLE__
    std::stringstream ss;

    ss <<
    R"('tell application "System Events"
            set theDesktops to a reference to every desktop
            repeat with aDesktop in theDesktops
                set the picture of aDesktop to ")" + filename + R"("
            end repeat
        end tell')";

    std::string command = "osascript -e "+ ss.str();
    std::system(command.c_str());
#elif defined _WIN32
    std::wstring w_filepath(filename.begin(), filename.end());
    int return_value = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void*)w_filepath.c_str(), SPIF_UPDATEINIFILE);
#endif
}

// Will be used later
void Image::set_as_wallpaper_with_idx(const std::string &filename, unsigned int screen_idx) {
#if defined __APPLE__
    std::stringstream ss;

    ss <<
       R"('tell application "System Events"
            get desktop )" + std::to_string(screen_idx) + R"(
            set the picture of desktop )" + std::to_string(screen_idx) + " to \"" + filename + R"("
        end tell')";

    std::string command = "osascript -e "+ ss.str();
    std::system(command.c_str());
#elif defined _WIN32
    
#endif
}

void Image::to_any_resolution(int width, int height, float earth_height_ratio) {
    remove_alpha();
    remove_description();
    remove_watermark();

    int top_bot_border = static_cast<int>(height * (1.0f - earth_height_ratio) * 0.5f);

    resize_preserve_ratio(height - 2 * top_bot_border);
    add_top_bot_border(top_bot_border);

    add_left_right_border((width - m_w) / 2);
}

void Image::write_png(const std::string &filename) const {
    stbi_write_png(filename.c_str(), m_w, m_h, m_channel, &m_data[0], m_w * m_channel);
}

void Image::resize_preserve_ratio(int height) {
    float ratio = static_cast<float>(height) / static_cast<float>(m_h);
    resize(static_cast<int>(m_w * ratio), height);
}

void Image::resize(int width, int height) {
    std::vector<unsigned char> resized_img(width * height * m_channel);
    stbir_resize_uint8(&m_data[0], m_w, m_h, 0, &resized_img[0], width, height, 0, m_channel);
    m_data = std::move(resized_img);
    m_w = width;
    m_h = height;
}

void Image::remove_alpha() {
    if(m_channel == 4){
        std::vector<unsigned char> rgb_img(m_w * m_h * 3);
        int j = 0;
        for(int i=0;i<m_w * m_h;i++){
            rgb_img[j++] = m_data[4*i];
            rgb_img[j++] = m_data[4*i+1];
            rgb_img[j++] = m_data[4*i+2];
        }

        m_data = std::move(rgb_img);
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
        LOG("This line should not be executed.. number of image channel is not 3 or 4.");
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
    std::vector<unsigned char> new_img(m_w * new_height * m_channel);
    std::copy(m_data.begin(), m_data.begin()+(m_w * m_h * m_channel), new_img.begin()+(size * m_w * m_channel));

    m_data = std::move(new_img);
    m_h = new_height;
}

void Image::add_left_right_border(int size) {
    int new_width = m_w + 2 * size;
    std::vector<unsigned char> new_img(new_width * m_h * m_channel);

    for(int i=0;i<m_h;i++){
        int src_pixel_idx = i * m_w;
        int dst_pixel_idx = i * new_width + size;
        std::copy(m_data.begin()+(src_pixel_idx * m_channel), m_data.begin()+(src_pixel_idx * m_channel) + m_w * m_channel, new_img.begin()+(dst_pixel_idx * m_channel));
    }

    m_data = std::move(new_img);
    m_w = new_width;
}
