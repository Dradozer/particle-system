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
        static void createComputeShader(GLuint &id, const char *path)
        {
            Shader::attachShader(id, GL_COMPUTE_SHADER, path);
            Shader::linkShader(id);
        }

        static void initComputeShader(GLuint *id, const unsigned int particleCount,
                                         const glm::ivec4 gridSize)
        {
            GLenum glError;
            if ((glError = glGetError()) != GL_NO_ERROR)
            {
                std::cout << "4.OpenGL-Error: " << glError << std::endl;
            }
            glUseProgram(id[0]);
            {
                glUniform1ui(id[2], particleCount);
                glUniform4i(id[3], gridSize.x, gridSize.y, gridSize.z, gridSize.w);
            }
            glUseProgram(0);
            if ((glError = glGetError()) != GL_NO_ERROR)
            {
                std::cout << "5.1 OpenGL-Error: " << glError << std::endl;
            }
        }

        static void updateComputeShaderParticle(GLuint *id, const float deltaTime,
                const unsigned int particleCount ,const glm::vec4 settings ,
                const glm::vec4 forces, const float buoyCoeff, const float temperature)
        {
            GLenum glError;
            if ((glError = glGetError()) != GL_NO_ERROR)
            {
                std::cout << "4.OpenGL-Error: " << glError << std::endl;
            }
            glUseProgram(id[0]);
            {
                glUniform1f(id[1], deltaTime);
                glUniform4fv(id[4],1, &settings[0]);
                glUniform4fv(id[5],1 ,&forces[0]);
                glUniform1f(id[6],buoyCoeff);
                glUniform1f(id[7],temperature);
                glDispatchCompute(ceil(particleCount / 64), 1, 1); // shader layout
                glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
            }
            glUseProgram(0);
            if ((glError = glGetError()) != GL_NO_ERROR)
            {
                std::cout << "5.2 OpenGL-Error: " << glError << std::endl;
            }
        }


        static void updateComputeShaderP64DT(GLuint *id, const float deltaTime, const unsigned int particleCount)
        {
            GLenum glError;
            if ((glError = glGetError()) != GL_NO_ERROR)
            {
                std::cout << "4.OpenGL-Error: " << glError << std::endl;
            }
            glUseProgram(id[0]);
            {
                glUniform1f(id[1], deltaTime);
                glDispatchCompute(ceil(particleCount / 64), 1, 1); // shader layout
                glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
            }
            glUseProgram(0);
            if ((glError = glGetError()) != GL_NO_ERROR)
            {
                std::cout << "5.2 OpenGL-Error: " << glError << std::endl;
            }
        }

        static void updateComputeShaderP64(GLuint *id, const unsigned int particleCount)
        {
            GLenum glError;
            if ((glError = glGetError()) != GL_NO_ERROR)
            {
                std::cout << "4.OpenGL-Error: " << glError << std::endl;
            }
            glUseProgram(id[0]);
            {
                glDispatchCompute(ceil(particleCount / 64), 1, 1); // shader layout
                glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
            }
            glUseProgram(0);
            if ((glError = glGetError()) != GL_NO_ERROR)
            {
                std::cout << "5.3 OpenGL-Error: " << glError << std::endl;
            }
        }

        static void updateComputeShaderD(GLuint *id, const glm::ivec4 gridSize)
        {
            int d = gridSize.x * gridSize.y * gridSize.z;
            GLenum glError;
            if ((glError = glGetError()) != GL_NO_ERROR)
            {
                std::cout << "4.OpenGL-Error: " << glError << std::endl;
            }
            glUseProgram(id[0]);
            {
                glDispatchCompute(ceil(d / 10), 1, 1); // shader layout
                glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
            }
            glUseProgram(0);
            if ((glError = glGetError()) != GL_NO_ERROR)
            {
                std::cout << "5.4 OpenGL-Error: " << glError << std::endl;
            }
        }

        static void updateComputeShaderPrefix(GLuint *id, const glm::ivec4 gridSize, unsigned int step)
        {
            int d = gridSize.x * gridSize.y * gridSize.z;
            GLenum glError;
            if ((glError = glGetError()) != GL_NO_ERROR)
            {
                std::cout << "4.OpenGL-Error: " << glError << std::endl;
            }
            glUseProgram(id[0]);
            {
                glUniform1ui(id[2], step);
                glDispatchCompute(ceil(d / 10), 1, 1); // shader layout
                glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
            }
            glUseProgram(0);
            if ((glError = glGetError()) != GL_NO_ERROR)
            {
                std::cout << "5.5 OpenGL-Error: " << glError << std::endl;
            }
        }
    };
}

#endif //BA_GAIDA_COMPUTESHADER_H
