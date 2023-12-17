#pragma once

#include <glm/glm.hpp>

#include "engine/camera_controller.hpp"
#include "engine/camera.hpp"
#include "engine/events.hpp"

inline constexpr float LENS_FOV {45.0f};
inline constexpr float LENS_NEAR {0.1f};
inline constexpr float LENS_FAR {300.0f};

struct MyCameraController : public bb::CameraController {
    MyCameraController() = default;
    MyCameraController(
        bb::Camera* camera,
        int width,
        int height,
        float fov,
        float near,
        float far,
        const glm::vec3& point,
        float distance_to_point,
        float pitch
    );

    virtual void update_controls(float dt) override;
    virtual void update_camera(float dt) override;
    virtual glm::vec3 get_position() const override;
    virtual glm::vec3 get_rotation() const override;

    void on_key_pressed(const bb::KeyPressedEvent& event);

    glm::vec3 position {};
    float pitch {0.0f};
    float yaw {0.0f};

    glm::vec3 point {};
    float distance_to_point {0.0f};
    float angle_around_point {0.0f};
};
