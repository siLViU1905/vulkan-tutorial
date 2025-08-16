#ifndef MVPUFFER_H
#define MVPUFFER_H

#define  GLM_FORCE_RADIANS
#define  GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"

struct MVP
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    glm::vec3 color;

    char padding[4];
};

static_assert(sizeof(MVP) % 16 == 0, "Sizeof MVP is not divisible by 16");

#endif //MVPBUFFER_H
