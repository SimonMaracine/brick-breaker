#pragma once

#include <string>

namespace bb {
    struct ApplicationProperties {
        int width {1024};
        int height {576};
        std::string title {"Custom Engine"};
        bool fullscreen {false};
    };
}
