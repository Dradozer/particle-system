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
        void createComputeShader(GLuint &id, const char *path);

        void updateComputeShader(GLuint *id, const float deltaTime, const int particleCount);

        template<typename T>
        void createSSBO(GLuint &id, int bindingID, int size, T *data);
    private:
    };
}

#endif //BA_GAIDA_COMPUTESHADER_H
