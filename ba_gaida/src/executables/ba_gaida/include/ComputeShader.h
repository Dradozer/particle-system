//
// Created by Dradozer on 12.04.2019.
//

#ifndef BA_GAIDA_COMPUTESHADER_H
#define BA_GAIDA_COMPUTESHADER_H

#include "Shader.h"

namespace ba_gaida
{
    class ComputeShader
    {
    public:
        ComputeShader(GLuint &id, const char *path);

        void updateComputeShader(GLuint *id, const float deltaTime, const int particleCount);

    private:
    };
}

#endif //BA_GAIDA_COMPUTESHADER_H
