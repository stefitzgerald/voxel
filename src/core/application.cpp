#include "application.h"

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

Application* currentApplication = nullptr;

Application& Application::getInstance() {
    if (currentApplication) {
        return *currentApplication;
    } else {
        throw std::runtime_error("There is no current application");
    }
}

Application::Application() : state(STATE_READY), width(1024), height(768), title("Application") {
    currentApplication = this;

    std::cout << "[Info] GLFW initialization" << std::endl;

    if (!glfwInit()) {
        throw std::runtime_error("Couldn't init GLFW");
    }

    int major = 3;
    int minor = 3;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Couldn't create a window");
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if (err != GLEW_OK) {
        glfwTerminate();
        throw std::runtime_error(std::string("Couldn't initialize GLEW, error = ") + (const char*) glewGetErrorString(err));
    }

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported " << version << std::endl;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

GLFWwindow* Application::getWindow() const {
    return window;
}

void Application::exit() {
    state = STATE_EXIT;
}

float Application::getFrameDeltaTime() const {
    return deltaTime;
}

float Application::getTime() const {
    return time;
}

void Application::run() {
    state = STATE_RUN;

    glfwMakeContextCurrent(window);
    time = glfwGetTime();

    while (state == STATE_RUN) {
        float t = glfwGetTime();
        deltaTime = t - time;
        time = t;

        detectWindowDimensionChange();

        loop();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void Application::detectWindowDimensionChange() {
    int w, h;
    glfwGetWindowSize(getWindow(), &w, &h);
    dimensionChanged = (w != width || h != height);
    if (dimensionChanged) {
        width = w;
        height = h;
        glViewport(0, 0, width, height);
    }
}

void Application::loop() {
    std::cout << "[INFO] : loop" << std::endl;
}

int Application::getWidth() {
    return width;
}

int Application::getHeight() {
    return height;
}

float Application::getWindowRatio() {
    return float(width) / float(height);
}

bool Application::windowDimensionChanged() {
    return dimensionChanged;
}