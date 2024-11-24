#pragma once

#include <random>
#include <glm/glm.hpp>
#include <iostream>

class Generator {
public:
    Generator();
    ~Generator();
    float getRandomFloat(float min, float max);
    int getRandomInt(int min, int max);
    glm::vec3 getRandomVec3(float min, float max, bool yZero = false);
private:
    std::mt19937 engine;
};
