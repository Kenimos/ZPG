#include "Generator.h"
#include <iostream>

Generator::Generator()
    : engine(std::random_device{}())
{
    std::cout << "Generator constructed." << std::endl;
}

Generator::~Generator()
{
    std::cout << "Generator destructed." << std::endl;
}

float Generator::getRandomFloat(float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(engine);
}

int Generator::getRandomInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine);
}

glm::vec3 Generator::getRandomVec3(float min, float max, bool yZero)
{
    float x = getRandomFloat(min, max);
    float y = yZero ? 2.0f : getRandomFloat(min, max);
    float z = getRandomFloat(min, max);
    return glm::vec3(x, y, z);
}
