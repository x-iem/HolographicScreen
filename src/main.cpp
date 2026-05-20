#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <Shaders.h>
#include <stb_image.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <tracker.h>

const char* vertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

//out vec3 persoColor;
out vec2 texCoord;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    //persoColor = aColor;
    texCoord = vec2(aTexCoord.x, aTexCoord.y);
}
)";

const char* variableFragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;
//in vec3 persoColor;
in vec2 texCoord;
uniform sampler2D ourTexture;
uniform vec4 persoColor;

void main()
{
    //FragColor = vec4(1.0, 1.0, 0.4, 1.0);
    FragColor = texture(ourTexture, texCoord) * persoColor;
}
)";

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int width = 1920, height = 1080;
    GLFWwindow* window =
        glfwCreateWindow(width, height, "Triangle", nullptr, nullptr);

    if (!window) return -1;

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return -1;

    //shaders
    Shader s1 = Shader(vertexShaderSrc, variableFragmentShaderSrc);

    // VAO, VBO, EBO
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    FaceTracker tracker = FaceTracker(3);
    int camWidth = tracker.getWidth();
    int camHeigth = tracker.getHeight();
    cv::namedWindow("rr");

    float near = 0.1f, far=100.0f, eyeZ=0.6f, left=-0.265f, right=0.265f, top=0.15f, bottom=-0.15f;

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.15f, 0.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        s1.use();
        glBindVertexArray(VAO);
        float factorr = 2.0f;
        std::vector<float> positions = tracker.retrieveFacePos();

        positions = {(positions[0]/camWidth - 0.5f) * 2.0f,
                    -(positions[1]/camHeigth - 0.5f) * 2.0f,
                    positions[2]};

        float eyeX = positions[0];
        float eyeY = positions[1];
        float l = near * (left   - eyeX) / eyeZ;
        float r = near * (right  - eyeX) / eyeZ;
        float b = near * (bottom - eyeY) / eyeZ;
        float t = near * (top    - eyeY) / eyeZ;

        // Matrices
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 cam = glm::mat4(1.0f);
        glm::mat4 proj = glm::perspective(glm::radians(60.0f), (float)width/(float)height, 0.1f, 100.0f);
        //glm::mat4 proj = glm::frustum(l, r, b, t, near, far);

        cam = glm::translate(cam, glm::vec3(positions[0]*1.5f, 1.0f*positions[1], positions[2]/100));
        model = glm::translate(model, glm::vec3(0, -.0f, -5.0f));
        //std::cout << "x:" << positions[0] << " y:" << positions[1] << " z:" << positions[2] << std::endl;
        model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        glm::mat4 view = glm::lookAt(glm::vec3(eyeX, eyeY, eyeZ), glm::vec3(0,0,0), glm::vec3(0,1,0));

        s1.uniformMat4("model", model);
        s1.uniformMat4("view", view);
        s1.uniformMat4("projection", proj);
        float factor = sin(glfwGetTime()*5)/2+0.7f;
        float factor2 = sin(glfwGetTime()*3)/2+0.7f;
        float factor3 = sin(glfwGetTime()*7)/2+0.7f;
        cv::Mat trr = tracker.getFrame();
        //cv::imshow("rr", trr);
        cv::waitKey(30);
        s1.uniformVec4("persoColor", factor, factor3, factor2, 0.0f);
        s1.applyTexture("img/wall.jpg", 512, 512, 3);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
