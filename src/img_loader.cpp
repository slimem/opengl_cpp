#pragma once

#include "img_loader.h"

img_loader::img_loader(const std::string_view imagePath)
    : _imgPath(imagePath) {
    if (load_bmp()) {
        _imgStart = nullptr;
    }
}

GLuint
img_loader::load_bmp() {

    std::ifstream bmpFile(_imgPath.data());

    if (!bmpFile.is_open()) {
        std::cerr << "[load_bmp] Cannot open file " << _imgPath << ": File not found\n";
        return 1;
    }
    // check length
    bmpFile.seekg(0, std::ios::end);
    size_t length = bmpFile.tellg();
    bmpFile.seekg(0, std::ios::beg);
    
    // a limit
    if (length > 1'000'000) {
        std::cerr << "[load_bmp] Cannot open file " << _imgPath << ": File too large\n";
        return 1;
    }
    
    std::cout << "[load_bmp] Loading BMP file " << _imgPath << " (" << length << " bytes)\n";

    // read the file
    _data.resize(length);
    bmpFile.read((char*)_data.data(), length);
    bmpFile.close();

    if (_data.size() < 54) {
        std::cerr << "[load_bmp] File " << _imgPath << " is emtpy or malformed\n";
        return 1;
    }

    {
        std::string_view bm((char*)_data.data(), 2);

        if (bm.compare("BM")) {
            std::cerr << "[load_bmp] File " << _imgPath << " is malformed\n";
            return 1;
        }
    }

    _dataPos = *(uint32_t*)&_data[(uint8_t)byte_pos::DATA_POS];
    _imageSize = *(uint32_t*)&_data[(uint8_t)byte_pos::IMG_SIZE];
    _width = *(uint32_t*)&_data[(uint8_t)byte_pos::WIDTH];
    _height = *(uint32_t*)&_data[(uint8_t)byte_pos::HEIGHT];

    // fill in the missing information
    if (_imageSize == 0) {
        _imageSize = _width * _height * 3;
    }

    if (_dataPos == 0) {
        _dataPos = 54;
    }

    if (_imageSize > (_data.size() - _dataPos)) {
        std::cerr << "[load_bmp] File " << _imgPath << " is malformed: Expected Size = " << _imageSize;
        std::cerr << " found " << _data.size() - _dataPos << "\n";
        return 1;
    }
    _imgStart += _dataPos;

    return 0;
}

