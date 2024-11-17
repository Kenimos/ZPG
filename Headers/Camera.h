#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <vector>
#include "Observer.h"
#include "Subject.h"
#include <GLFW/glfw3.h>

class Camera : public Subject
{
public:
    Camera(const glm::vec3& position);

    void processKeyboard(GLFWwindow* window, float deltaTime);
    void processMouseMovement(double xpos, double ypos);
    void processMouseScroll(double yoffset);

    void setAspectRatio(float aspectRatio);

    const glm::vec3& getPosition() const;
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::vec3 getFront() const { return front; }

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    float yaw;
    float pitch;
    float lastX;
    float lastY;
    bool firstMouse;

    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    float aspectRatio;

    void updateCameraVectors();
    void notifyViewMatrixChanged();
    void notifyProjectionMatrixChanged();
};

#endif // CAMERA_H
