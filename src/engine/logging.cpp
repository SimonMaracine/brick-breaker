#include <cstdio>
#include <cstdarg>

#include "engine/logging.hpp"

namespace bb {
    void log_message(const char* format, ...) {
        va_list list;
        va_start(list, format);

        std::vprintf(format, list);

        va_end(list);
    }
}
