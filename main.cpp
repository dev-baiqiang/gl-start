#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "gl_core.h"
#include "screenshot.h"

static void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        screenshot(window, "screenshot.tga");
    }
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
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        onDraw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    onDestroy();

    glfwTerminate();
    return 0;
}