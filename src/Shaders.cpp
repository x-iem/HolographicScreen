#include <Shaders.h>

Shader::Shader(const char* vertex_str, const char* fragment_str) {
    this->compile_vertex(vertex_str);
    this->compile_fragment(fragment_str);
    this->link_program();
}
void Shader::use() {
    glUseProgram(this->shaderProgram);
}

void Shader::uniformFloat(const char* uniformName, float uniformValue) {
    glUniform1f(glGetUniformLocation(this->shaderProgram, uniformName), uniformValue);
}
void Shader::uniformMat4(const char* uniformName, glm::mat4 uniformValue) {
    glUniformMatrix4fv(glGetUniformLocation(this->shaderProgram, uniformName), 1, GL_FALSE, glm::value_ptr(uniformValue));
}
void Shader::uniformVec3(const char* uniformName, float uniVal1, float uniVal2, float uniVal3) {
    glUniform3f(glGetUniformLocation(this->shaderProgram, uniformName), uniVal1, uniVal2, uniVal3);
}

void Shader::uniformVec4(const char* uniformName, float uniVal1, float uniVal2, float uniVal3, float uniVal4) {
    glUniform4f(glGetUniformLocation(this->shaderProgram, uniformName), uniVal1, uniVal2, uniVal3, uniVal4);
}

int Shader::applyTexture(const char* imgLink, int width, int height, int nrChannels) {
    unsigned char *data = stbi_load(imgLink, &width, &height, &nrChannels, 0);
    unsigned int texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    if (data) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else this->buildStatus = status::IMG_TEXTURE_LOAD_FAILED;
    glBindTexture(GL_TEXTURE_2D, texture);
    return this->buildStatus;
}


int Shader::compile_vertex(const char* vertex_str) {
    int success = 0;

    this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(this->vertexShader, 1, &vertex_str, nullptr);
    glCompileShader(this->vertexShader);

    glGetShaderiv(this->vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(this->vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        this->buildStatus = status::COMPIL_VERTEX_FAILED;
    }

    return 0;
}

int Shader::compile_fragment(const char* fragment_str) {
    int success = 0;

    this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(this->fragmentShader, 1, &fragment_str, nullptr);
    glCompileShader(this->fragmentShader);

    glGetShaderiv(this->fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(this->fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        this->buildStatus = status::COMPIL_FRAGMENT_FAILED;
    }

    return 0;
}
int Shader::link_program() {
    int success = 0;

    this->shaderProgram = glCreateProgram();

    glAttachShader(this->shaderProgram, this->vertexShader);
    glAttachShader(this->shaderProgram, this->fragmentShader);
    glLinkProgram(this->shaderProgram);

    glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(this->shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        this->buildStatus = status::LINK_PROGRAM_FAILED;
    }

    glDeleteShader(this->vertexShader);
    glDeleteShader(this->fragmentShader);
    return 0;
}
