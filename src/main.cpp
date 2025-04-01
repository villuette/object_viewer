#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstddef>
#include <glad/glad.h>

#include <iostream>

#include "rendering/shaderProgram.h"
#include "resources/resource_manager.h"

auto g_windowHeight = 480;
auto g_windowWidth = 640;

GLfloat points[] = {
    0.0f,  0.5f,  0.0f, // Верхняя вершина
    -0.5f, -0.5f, 0.0f, // Нижняя левая вершина
    0.5f,  -0.5f, 0.0f  // Нижняя правая вершина

};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f, // Red for top vertex
    0.0f, 1.0f, 0.0f, // Green for bottom-left vertex
    0.0f, 0.0f, 1.0f  // Blue for bottom-right vertex
};
const char *vertex_shader =
    "#version 460\n"
    "layout(location = 0) in vec3 vertex_position;"
    "layout(location = 1) in vec3 vertex_color;"
    "out vec3 color;"
    "void main(){"
    "   color = vertex_color;"
    "   gl_Position = vec4(vertex_position.xy, 0.0, 1.0);"
    "}";

const char *fragment_shader = "#version 460\n"
                              "in vec3 color;"
                              "out vec4 frag_color;"
                              "void main(){"
                              "   frag_color = vec4(color, 1.0);"
                              "}";

void errorCallback(int error, const char *description) {
    std::cerr << "Error: " << description << std::endl;
}
void resizeCallback(GLFWwindow *window, int width = 0, int height = 0) {
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight); // Физические пиксели
    glViewport(0, 0, fbWidth, fbHeight);
}

void keyCallback(GLFWwindow *wnd, int key, int scancode, int action, int mods) {
    // std::cout << "key: " << key << " scancode: " << scancode << " action: "
    // << action << " mods: " << mods << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(wnd, GL_TRUE);
    }
}

int main(int argc, char **argv) {

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *window = glfwCreateWindow(g_windowWidth, g_windowHeight,
                                          "Object Viewer", NULL, NULL);
    if (!window) {
        std::cerr << "failed build window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetErrorCallback(errorCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    /* Initialize GLAD */
    if (!gladLoadGL()) {
        std::cerr << "Failed to load glad" << std::endl;
        return -1;
    }
    if (glfwGetWindowAttrib(window, GLFW_VISIBLE)) { // first time buffer
                                                     // appears
        resizeCallback(window);
    }
    {//resource_manager's context
        ResourceManager resource_manager(
            (std::string(argv[0]))); // m_path now bin
        auto defaultShaderProgram = resource_manager.loadShaders(
            "Default", "resources/shaders/vertex_shader.glsl",
            "resources/shaders/fragment_shader.glsl");
        if (!defaultShaderProgram) {
            std::cerr << "cant load shader program" << std::endl;
        }
        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        // подключить сгенерированный буфер
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        // заполнение информацией (позициями вертекса на видеокарту ???),для
        // текущего binded буфера
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // send vertices to videocard
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        // stride - шаг смещения
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;

        // define color buffer with GL_COLOR_BUFFER_BIT binding
        glClearColor(0, 0, 0, 1);
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);
            defaultShaderProgram->use();
            // избыточно сейчас
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
        }
    }
    glfwTerminate();
    return 0;
}