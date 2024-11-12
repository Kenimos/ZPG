// Scale.h

#ifndef SCALE_H
#define SCALE_H

#include <glm/glm.hpp>

class Scale
{
public:
    Scale();
    ~Scale();

    void setScale(const glm::vec3& scale);
    glm::vec3 getScale() const;

private:
    glm::vec3 scale;
};

#endif // SCALE_H
