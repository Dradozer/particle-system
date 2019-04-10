//
// Created by Dradozer on 08.04.2019.
//

#ifndef BA_GAIDA_SHADER_H
#define BA_GAIDA_SHADER_H

#define FAIL_PROGRAM {system("pause");  exit(EXIT_FAILURE);}

#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <GL/glew.h>
#include "ConsoleColor.h"

namespace ba_gaida
{
    class Shader
    {
    public:
        static void attachShader(GLuint &program, GLenum type, const char *path);

        static void linkShader(GLuint program);

        static void deleteShader(GLuint program);

    private:
        static GLuint loadShader(GLenum type, const char *path);

        static void validateShader(GLuint shader, const char *path = "");

        static void validateProgram(GLuint program);
    };
}

#endif //BA_GAIDA_SHADER_H
