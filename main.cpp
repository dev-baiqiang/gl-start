#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "gl_core.h"

static void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    GLFWwindow *window = initGL();

    if (!window) {
        return -1;
    }

    onCreate();
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(1.0f, 0.3f, 0.3f, 0.3f);
        glClear(GL_COLOR_BUFFER_BIT);

        onDraw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    onDestroy();

    glfwTerminate();
    return 0;
}