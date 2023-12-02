#pragma once

#include <utility>

namespace bb {
    struct GlInfoDebug {
        static void initialize_debugging();
        static std::pair<int, int> get_version_number();
        static const unsigned char* get_opengl_version();
        static const unsigned char* get_glsl_version();
        static const unsigned char* get_vendor();
        static const unsigned char* get_renderer();
    };
}
