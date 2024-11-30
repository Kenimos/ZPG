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

float Generator::getRandomFloatExcluding(float min, float max, float min2, float max2)
{
    if (min2 <= min || max2 >= max || min2 > max2)
    {
        throw std::invalid_argument("Invalid exclusion range.");
    }

    float lowerRange = min2 - min;
    float upperRange = max - max2;
    float totalRange = lowerRange + upperRange;

    std::uniform_real_distribution<float> dist(0.0f, totalRange);
    float rand = dist(engine);

    float result;
    if (rand < lowerRange)
    {
        result = min + rand;
    }
    else
    {
        result = max2 + (rand - lowerRange);
    }

    // Print the actual generated number
    printf("%f ", result);
    return result;
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
