//
// Created by pasab on 29.03.2019.
//

#ifndef CPP_PROJEKTE_SHADER_H
#define CPP_PROJEKTE_SHADER_H

#include <GL/glew.h>

#define FAIL_GRACEFULLY {system("pause");  exit(EXIT_FAILURE);}

namespace ParticleSystem {

    class Shader {
        //static functions to load a shader
        public:
            static void AttachShader(GLuint &program, GLenum type, const char *path);
            static void LinkProgram(GLuint program);
            static void DeleteShaders(GLuint program);

        //helper functions
        private:
            static GLuint LoadShader(GLenum type, const char *path);
            static void ValidateShader(GLuint shader, const char *path = "");
            static void ValidateProgram(GLuint program);
    };

}

#endif //CPP_PROJEKTE_SHADER_H
