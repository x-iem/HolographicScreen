#ifndef SHADERS_H
#define SHADERS_H

#include <glad/glad.h>
#include <iostream>
#include <math.h>
#include <string>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <stb_image.h>

class Shader {
    public:
        Shader(const char* vertex_str, const char* fragment_str);
        enum status {OK=0, COMPIL_VERTEX_FAILED, COMPIL_FRAGMENT_FAILED, LINK_PROGRAM_FAILED, IMG_TEXTURE_LOAD_FAILED};
        void use();
        void uniformFloat(const char* uniformName, float uniformValue);
        void uniformMat4(const char* uniformName, glm::mat4 uniformValue);
        void uniformVec3(const char* uniformName, float uniVal1, float uniVal2, float uniVal3);
        void uniformVec4(const char* uniformName, float uniVal1, float uniVal2, float uniVal3, float uniVal4);
        int applyTexture(const char* imgLink, int width, int height, int nrChannels);
    private:
        unsigned int ID;
        unsigned int vertexShader, fragmentShader, shaderProgram;
        enum status buildStatus = status::OK;
        char infoLog[512];
        int compile_vertex(const char* vertex_str);
        int compile_fragment(const char* fragment_str);
        int link_program();
};

#endif
