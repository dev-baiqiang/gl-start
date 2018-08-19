#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "gl_core.h"
#include "util.h"

#define _USE_MATH_DEFINES

#include <math.h>

const unsigned int WINDOW_WIDTH = 400;
const unsigned int WINDOW_HEIGHT = 400;

GLFWwindow *initGL() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16);
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwSetWindowIcon(window, 0, nullptr);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    return window;
}

static const char *vertexShaderSource = getShader("res/vs_uniform.glsl");
static const char *fragmentShaderSource = getShader("res/fs_uniform.glsl");
static constexpr int NUM = 20;
static const int PC = 512;
static const float RADIUS = 0.8;
static unsigned int VBO;
static unsigned int VAOs[NUM];
static GLuint shaderProgram;

void onCreate() {

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    GLfloat *coordA = new GLfloat[(PC + 2) * 2];

    unsigned coordIdx = 0;
    coordA[coordIdx++] = 0.0f;
    coordA[coordIdx++] = 0.0f;

    float angInc = 2.0f * M_PI / static_cast<float>(PC);
    float cosInc = cos(angInc);
    float sinInc = sin(angInc);

    coordA[coordIdx++] = 1.0f * RADIUS;
    coordA[coordIdx++] = 0.0f * RADIUS;

    float xc = 1.0f * RADIUS;
    float yc = 0.0f * RADIUS;
    for (unsigned i = 1; i < PC; ++i) {
        float xcNew = cosInc * xc - sinInc * yc;
        yc = sinInc * xc + cosInc * yc;
        xc = xcNew;
        coordA[coordIdx++] = xc;
        coordA[coordIdx++] = yc;
    }

    coordA[coordIdx++] = 1.0f * RADIUS;
    coordA[coordIdx++] = 0.0f * RADIUS;

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glGenVertexArrays(NUM, VAOs);
    glGenBuffers(1, &VBO);

    for (int i = 0; i < NUM; i++) {
        glBindVertexArray(VAOs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, (PC + 2) * 2 * sizeof(GLfloat), coordA, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPCer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

}


void onDraw() {
    for (int i = NUM - 1; i >= 0; i--) {
        glBindVertexArray(VAOs[i]);
        float timeValue = glfwGetTime();
        float value = (sin(timeValue) / 2.0f) + 0.5f;
        float diff = float(i) / NUM;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "color");
        int vertexRadiusLocation = glGetUniformLocation(shaderProgram, "radius");
        glUseProgram(shaderProgram);
        glUniform1f(vertexRadiusLocation, diff);
        glUniform4f(vertexColorLocation, diff, value, diff, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 0, PC + 2);
        glBindVertexArray(0);
    }
}


void onDestroy() {
    glDeleteVertexArrays(NUM, VAOs);
    glDeleteBuffers(1, &VBO);
}