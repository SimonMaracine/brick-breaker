#include <vector>
#include <string>
#include <utility>
#include <optional>
#include <cstddef>

#include <engine/engine.hpp>

#include "menu_selection.hpp"

struct LevelsScene : public bb::Scene {
    LevelsScene()
        : bb::Scene("levels") {}

    void on_enter() override;
    void on_exit() override;
    void on_update() override;

    void on_window_resized(const bb::WindowResizedEvent& event);
    void on_key_released(const bb::KeyReleasedEvent& event);

    std::size_t reload_levels();

    std::optional<std::string> get_level_name(const std::string& file_path);

    bb::Camera2D cam_2d;

    std::vector<std::pair<std::string, std::string>> level_paths;
    MenuSelection<int> level_selection;

    static constexpr int MAX_LEVELS {7};
};
