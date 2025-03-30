#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

auto g_windowHeight = 480;
auto g_windowWidth = 640;

void glfwResizeCallback(GLFWwindow *wnd, int width, int height){
    g_windowWidth = width;
    g_windowHeight = height;
    glViewport(0,0,g_windowWidth, g_windowHeight);
}

void glfwKeyCallback(GLFWwindow *wnd, int key, int scancode, int action, int mods){
    // std::cout << "key: " << key << " scancode: " << scancode << " action: " << action << " mods: " << mods << std::endl;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(wnd, GL_TRUE);
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Object Viewer", NULL, NULL);
    if (!window)
    {
        std::cerr << "failed build window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetWindowSizeCallback(window, glfwResizeCallback);
    glfwSetKeyCallback(window, glfwKeyCallback);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize GLAD */
    if (!gladLoadGL())
    {
        std::cerr << "Failed to load glad" << std::endl;
        return -1;
    }    
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(0,1,0,1);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}