#pragma once

#include <string>

namespace bb {
    class SoundData {
    public:
        SoundData(const std::string& file_path);
        ~SoundData();

        SoundData(const SoundData&) = delete;
        SoundData& operator=(const SoundData&) = delete;
        SoundData(SoundData&&) = delete;
        SoundData& operator=(SoundData&&) = delete;

        void* get_chunk() const { return chunk; }
    private:
        void* chunk {nullptr};
    };
}
