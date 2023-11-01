#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>


template <class T>
inline T constrain(T value, T min, T max) {
    return value < min ? min : value > max ? max : value;
}


enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {

public:

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float yaw;
    float pitch;
    float speed;
    float sensitivity;
    float zoom;

    Camera(
        const glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
        float yaw = -90, float pitch = 0, 
        float speed = 2.5, float sensitivity = 0.1,
        float zoom = 45
    ) : position(position), front(front), yaw(yaw), pitch(pitch), speed(speed), sensitivity(sensitivity), zoom(zoom) {
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }


    void processKeyboard(CameraMovement direction, float dt) {
        float velocity = speed * dt;
        if (direction == FORWARD)
            position += forward * velocity;
        if (direction == BACKWARD)
            position -= forward * velocity;
        if (direction == LEFT)
            position -= right * velocity;
        if (direction == RIGHT)
            position += right * velocity;
        if (direction == UP)
            position += worldUp * velocity;
        if (direction == DOWN)
            position -= worldUp * velocity;
    }
    void processMouseMovement(double dx, double dy) {
        yaw   += dx * sensitivity;
        pitch = constrain(pitch + dy * sensitivity, -89.0, 89.0);
        updateCameraVectors();
    }
    void processMouseScroll(float dy) {
        zoom -= dy;
        zoom = constrain(zoom, 1.0f, 45.0f);
    }
private:

    void updateCameraVectors() {
        float yawRadians = glm::radians(yaw);
        float pitchRadians = glm::radians(pitch);
        float cosPitchRadians = cos(pitchRadians);

        front.x = cos(yawRadians) * cosPitchRadians;
        front.y = sin(pitchRadians);
        front.z = sin(yawRadians) * cosPitchRadians;
        front = glm::normalize(front);

        forward.x = front.x;
        forward.y = 0;
        forward.z = front.z;
        forward = glm::normalize(forward);

        right = glm::normalize(glm::cross(front, worldUp));
        up    = glm::normalize(glm::cross(right, front));
    }

};

#endif