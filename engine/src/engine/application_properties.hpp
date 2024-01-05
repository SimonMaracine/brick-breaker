#pragma once

#include <string>
#

namespace bb {
    struct ApplicationProperties {
        int width {1024};
        int height {576};
        std::string title {"Custom Engine"};
        bool fullscreen {false};
        void* user_data {nullptr};
        int min_width {640};
        int min_height {360};
        int samples {1};
    };
}
