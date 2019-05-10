//
// Created by Dradozer on 04.05.2019.
//

#ifndef BA_GAIDA_PARTICLE_H
#define BA_GAIDA_PARTICLE_H

#include <glm/glm.hpp>

struct Particle{
    glm::vec4 position;
    glm::vec4 velocity;
    unsigned int gridID;
    int pad1,pad2,pad3;
};

#endif //BA_GAIDA_PARTICLE_H
