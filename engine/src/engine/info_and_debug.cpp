#include <utility>
#include <cassert>

#include <glad/glad.h>

#include "engine/panic.hpp"
#include "engine/info_and_debug.hpp"
#include "engine/logging.hpp"

namespace bb {
    static void error_callback(
        GLenum,
        GLenum,
        GLuint id,
        GLenum severity,
        GLsizei,
        const GLchar* message,
        const void*
    ) {
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:
                log_message("(%d) OpenGL: %s\n", id, message);

                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
            case GL_DEBUG_SEVERITY_LOW:
                log_message("(%d) OpenGL: %s\n", id, message);

                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                assert(false);

                break;
        }
    }

    void GlInfoDebug::initialize_debugging() {
        glDebugMessageCallback(error_callback, nullptr);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageControl(
            GL_DONT_CARE,
            GL_DONT_CARE,
            GL_DEBUG_SEVERITY_NOTIFICATION,
            0,
            nullptr,
            GL_FALSE
        );
    }

    std::pair<int, int> GlInfoDebug::get_version_number() {
        int major, minor;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);

        return std::make_pair(major, minor);
    }

    const unsigned char* GlInfoDebug::get_opengl_version() {
        return glGetString(GL_VERSION);
    }

    const unsigned char* GlInfoDebug::get_glsl_version() {
        return glGetString(GL_SHADING_LANGUAGE_VERSION);
    }

    const unsigned char* GlInfoDebug::get_vendor() {
        return glGetString(GL_VENDOR);
    }

    const unsigned char* GlInfoDebug::get_renderer() {
        return glGetString(GL_RENDERER);
    }
}
