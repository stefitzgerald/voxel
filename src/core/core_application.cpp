#include "core_application.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <iostream>
#include <vector>

#include "asset.h"
#include "ogl/glError.h"

struct VertexType {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec4 color;
};

float heightMap(const glm::vec2 position) {
    return 2.0 * sin(position.x) * sin(position.y);
}

VertexType getHeightMap(const glm::vec2 position) {
    const glm::vec2 dx(1.0, 0.0);
    const glm::vec2 dy(0.0, 1.0);

    VertexType v;
    float h = heightMap(position);
    float hx = 100.f * (heightMap(position + 0.01f * dx) -h);
    float hy = 100.f * (heightMap(position + 0.01f * dy) -h);

    v.position = glm::vec3(position, h);
    v.normal = glm::normalize(glm::vec3(-hx, -hy, 1.0));

    float c = sin(h * 5.f) * 0.5 + 0.5;
    v.color = glm::vec4(c, 1.0 -c, 1.0, 1.0);
    return v;
}

CoreApplication::CoreApplication() 
    : Application(), 
    vertexShader(SHADER_DIR "/shader.vert", GL_VERTEX_SHADER),
    fragmentShader(SHADER_DIR "/shader.frag", GL_FRAGMENT_SHADER),
    shaderProgram({vertexShader, fragmentShader}) 
{
    glCheckError(__FILE__, __LINE__);

    std::vector<VertexType> vertices;
    std::vector<GLuint> index;

    for (int y = 0; y <= size; y++) {
        for (int x = 0; x <= size; x++) {
            float xx = (x - size / 2) * 0.1f;
            float yy = (y - size / 2) * 0.1f;
            vertices.push_back(getHeightMap({xx, yy}));
        }
    }

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            index.push_back((x + 0) + (size + 1) * (y + 0));
            index.push_back((x + 1) + (size + 1) * (y + 0));
            index.push_back((x + 1) + (size + 1) * (y + 1));

            index.push_back((x + 1) + (size + 1) * (y + 1));
            index.push_back((x + 0) + (size + 1) * (y + 1));
            index.push_back((x + 0) + (size + 1) * (y + 0));
        }
    }

    std::cout << "vertices=" << vertices.size() << std::endl;
    std::cout << "index=" << index.size() << std::endl;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexType), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint), index.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    shaderProgram.setAttribute("position", 3, sizeof(VertexType), offsetof(VertexType, position));
    shaderProgram.setAttribute("normal", 3, sizeof(VertexType), offsetof(VertexType, normal));
    shaderProgram.setAttribute("color", 4, sizeof(VertexType), offsetof(VertexType, color));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBindVertexArray(0);
}

void CoreApplication::loop() {
    if (glfwWindowShouldClose(getWindow())) {
        exit();
    }

    float t = getTime();
    projection = glm::perspective(float(2.0 * atan(getHeight() / 1920.f)), getWindowRatio(), 0.1f, 100.f);
    view = glm::lookAt(glm::vec3(20.0 * sin(t), 20.0 * cos(t), 20.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.use();

    shaderProgram.setUniform("projection", projection);
    shaderProgram.setUniform("view", view);

    glCheckError(__FILE__, __LINE__);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glCheckError(__FILE__, __LINE__);
    glDrawElements(
        GL_TRIANGLES,
        size * size * 2 * 3,
        GL_UNSIGNED_INT,
        nullptr
    );

    glBindVertexArray(0);
    shaderProgram.unuse();
}