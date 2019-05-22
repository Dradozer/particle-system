//
// Created by Dradozer on 12.04.2019.
//

#ifndef BA_GAIDA_COMPUTESHADER_H
#define BA_GAIDA_COMPUTESHADER_H

#include "Shader.h"
#include "PerformanceSettings.h"

namespace ba_gaida
{
    class ComputeShader
    {
    public:
        static void createComputeShader(GLuint &id, const char *path)
        {
            Shader::attachShader(id, GL_COMPUTE_SHADER, path);
            Shader::linkShader(id);
        }

        static void updateComputeShaderP64(GLuint *id, const float deltaTime, const unsigned int particleCount,
                                           const glm::ivec4 gridSize)
        {
#ifndef maxFPS
            GLenum glError;
            if ((glError = glGetError()) != GL_NO_ERROR)
            {
                std::cout << "4.OpenGL-Error: " << glError << std::endl;
            }
#endif
            glUseProgram(id[0]);
            {
                glUniform1f(id[1], deltaTime);
                glUniform1ui(id[2], particleCount);
                glUniform4i(id[3], gridSize.x, gridSize.y, gridSize.z, gridSize.w);
                glDispatchCompute(ceil(particleCount / 64), 1, 1); // shader layout
                glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
            }
            glUseProgram(0);
#ifndef maxFPS
            if ((glError = glGetError()) != GL_NO_ERROR)
            {
                std::cout << "5.OpenGL-Error: " << glError << std::endl;
            }
#endif
        }

        static void updateComputeShaderD(GLuint *id, const float deltaTime, const unsigned int particleCount,
                                         const glm::ivec4 gridSize)
        {
            int d = gridSize.x * gridSize.y * gridSize.z;
#ifndef maxFPS
            GLenum glError;
            if ((glError = glGetError()) != GL_NO_ERROR)
            {
                std::cout << "4.OpenGL-Error: " << glError << std::endl;
            }
#endif
            glUseProgram(id[0]);
            {
                glUniform1f(id[1], deltaTime);
                glUniform1ui(id[2], particleCount);
                glUniform4i(id[3], gridSize.x, gridSize.y, gridSize.z, gridSize.w);
                glDispatchCompute(ceil(d / 10), 1, 1); // shader layout
                glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
            }
            glUseProgram(0);
#ifndef maxFPS
            if ((glError = glGetError()) != GL_NO_ERROR)
            {
                std::cout << "5.OpenGL-Error: " << glError << std::endl;
            }
#endif
        }
    };
}

#endif //BA_GAIDA_COMPUTESHADER_H
