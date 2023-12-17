#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/camera_controller.hpp"
#include "engine/camera.hpp"
#include "engine/events.hpp"
#include "my_camera_controller.hpp"

static constexpr float MIN_DISTANCE_TO_POINT {10.0f};
static constexpr float MAX_DISTANCE_TO_POINT {32.0f};

MyCameraController::MyCameraController(
    bb::Camera* camera,
    int width,
    int height,
    float fov,
    float near,
    float far,
    const glm::vec3& point,
    float distance_to_point,
    float pitch
)
    : bb::CameraController(camera), pitch(pitch), point(point), distance_to_point(distance_to_point) {
    camera->set_projection_matrix(width, height, fov, near, far);
    update_camera(1.0f);
}

void MyCameraController::update_controls(float dt) {

}

void MyCameraController::update_camera(float dt) {
    // Limit distance_to_point
    distance_to_point = std::max(distance_to_point, MIN_DISTANCE_TO_POINT);
    distance_to_point = std::min(distance_to_point, MAX_DISTANCE_TO_POINT);

    // Limit pitch
    pitch = std::min(pitch, 90.0f);
    pitch = std::max(pitch, -90.0f);

    const float horizontal_distance {distance_to_point * glm::cos(glm::radians(pitch))};
    const float vertical_distance {distance_to_point * glm::sin(glm::radians(pitch))};

    const float offset_x {horizontal_distance * glm::sin(glm::radians(angle_around_point))};
    const float offset_z {horizontal_distance * glm::cos(glm::radians(angle_around_point))};

    position.x = point.x - offset_x;
    position.z = point.z - offset_z;
    position.y = point.y + vertical_distance;

    yaw = 180.0f - angle_around_point;

    // Update camera data
    glm::mat4 matrix {glm::mat4(1.0f)};
    matrix = glm::rotate(matrix, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::translate(matrix, -position);

    camera->set_view_matrix(matrix);
}

glm::vec3 MyCameraController::get_position() const {
    return position;
}

glm::vec3 MyCameraController::get_rotation() const {
    return glm::vec3(pitch, yaw, 0.0f);
}

void MyCameraController::on_key_pressed(const bb::KeyPressedEvent& event) {
    switch (event.key) {
        case bb::KeyCode::K_a:
            angle_around_point -= 10.0f;
            break;
        case bb::KeyCode::K_d:
            angle_around_point += 10.0f;
            break;
        case bb::KeyCode::K_w:
            pitch += 10.0f;
            break;
        case bb::KeyCode::K_s:
            pitch -= 10.0f;
            break;
        case bb::KeyCode::K_q:
            distance_to_point += 2.0f;
            break;
        case bb::KeyCode::K_e:
            distance_to_point -= 2.0f;
            break;
        default:
            break;
    }
}
