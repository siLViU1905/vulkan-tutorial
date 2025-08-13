#ifndef MVPUFFER_H
#define MVPUFFER_H
#include "../include/glm/glm.hpp"

struct MVP
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
};

#endif //MVPBUFFER_H
