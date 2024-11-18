
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <iostream>

Camera::Camera(const glm::vec3 &position)
    : position(position),
      front(glm::vec3(0.0f, 0.0f, -1.0f)),
      up(glm::vec3(0.0f, 1.0f, 0.0f)),
      yaw(-90.0f),
      pitch(0.0f),
      lastX(400),
      lastY(300),
      firstMouse(true),
      movementSpeed(5.5f),
      mouseSensitivity(0.1f),
      zoom(45.0f),
      aspectRatio(800.0f / 600.0f)
{
    updateCameraVectors();
}

void Camera::processKeyboard(GLFWwindow *window, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= glm::normalize(glm::cross(front, up)) * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += glm::normalize(glm::cross(front, up)) * velocity;

    notifyViewMatrixChanged();
}

void Camera::processMouseMovement(double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = lastY - static_cast<float>(ypos); // Reversed since y-coordinates range from bottom to top

    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Constrain the pitch angle to prevent screen flipping
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    updateCameraVectors();
    notifyViewMatrixChanged();
}

void Camera::processMouseScroll(double yoffset)
{
    zoom -= static_cast<float>(yoffset);
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;

    notifyProjectionMatrixChanged();
}

void Camera::setAspectRatio(float aspectRatio)
{
    this->aspectRatio = aspectRatio;
    notifyProjectionMatrixChanged();
}

const glm::vec3 &Camera::getPosition() const
{
    return position;
}

void Camera::updateCameraVectors()
{
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
}

void Camera::notifyViewMatrixChanged()
{
    notifyObservers();
}

void Camera::notifyProjectionMatrixChanged()
{
    notifyObservers();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return glm::perspective(glm::radians(zoom), aspectRatio, 0.1f, 100.0f);
}

void Camera::setFirstMouse(bool value)
{
    firstMouse = value;
}