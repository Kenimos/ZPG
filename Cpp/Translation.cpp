// Translation.cpp
#include "Translation.h"
#include <glm/gtc/matrix_transform.hpp>

Translation::Translation(const glm::vec3& translation)
    : translation(translation) {}

glm::mat4 Translation::getMatrix() const {
    return glm::translate(glm::mat4(1.0f), translation);
}
