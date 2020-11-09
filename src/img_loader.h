#pragma once

#include "common.h"

class img_loader {
public:

    enum class byte_pos : uint8_t {
        DATA_POS    = 0x0A,
        IMG_SIZE    = 0x22,
        WIDTH       = 0x12,
        HEIGHT      = 0x16
    };

    img_loader() = delete;
    ~img_loader() = default;
    img_loader(const std::string_view imagePath);
    uint8_t* get_img_buff() const {
        return _imgStart;
    }

    uint32_t get_width() const {
        return _width;
    }

    uint32_t get_height() const {
        return _height;
    }

    GLuint load_bmp();
private:
    std::string_view _imgPath;
    std::vector<uint8_t> _data;
    uint32_t _dataPos = 0;
    uint32_t _imageSize = 0;
    uint32_t _width = 0;
    uint32_t _height = 0;
    uint8_t* _imgStart = nullptr;
};