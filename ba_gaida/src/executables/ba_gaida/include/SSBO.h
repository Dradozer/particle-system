//
// Created by Dradozer on 17.04.2019.
//

#ifndef BA_GAIDA_SSBO_H
#define BA_GAIDA_SSBO_H

#include "Shader.h"

namespace ba_gaida{
    class SSBO{
    public:
        template<typename T>
        static void createSSBO(GLuint &id, int bindingID, int size, T *data)
        {
            glCreateBuffers(1, &id);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER,id);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingID, id);
            glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
        };
    };
}

#endif //BA_GAIDA_SSBO_H
