#include "application.h"

#include "application.h"
#include "shader.h"

class CoreApplication : public Application {
public:
    CoreApplication();

protected:
    virtual void loop();

private:
    const int size = 100;

    // Shader
    Shader vertexShader;
    Shader fragmentShader;
    ShaderProgram shaderProgram;

    // Shader matrix uniform
    glm::mat4 projection = glm::mat4(1.0);
    glm::mat4 view = glm::mat4(1.0);

    GLuint vao, vbo, ibo;
};