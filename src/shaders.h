#pragma once

#include "common.h"

class shaders {
public:
    shaders() = default;
    ~shaders() = default;

public:
    [[nodiscard]] GLuint load_shaders(
        const std::string_view& vertexShaderPath,
        const std::string_view& fragmentShaderPath
    ) const noexcept;

};


