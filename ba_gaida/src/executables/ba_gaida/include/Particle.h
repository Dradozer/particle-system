//
// Created by Dradozer on 04.05.2019.
//

#ifndef BA_GAIDA_PARTICLE_H
#define BA_GAIDA_PARTICLE_H

#include <glm/glm.hpp>

struct Particle{
    glm::vec4 position;
    glm::vec4 velocity;
    glm::vec4 arbitraryPosition;
    unsigned int gridID;
    unsigned int memoryPosition;
    float density;
    float pressure;
};

#endif //BA_GAIDA_PARTICLE_H
