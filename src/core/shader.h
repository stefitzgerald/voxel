#pragma once

#define GLM_FORCE_RADIANS
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <initializer_list>
#include <map>
#include <string>

class Shader;
class ShaderProgram;

class Shader {
public:
    Shader(const std::string& filename, GLenum type);
    GLuint getHandle() const;
    ~Shader();
private:
    GLuint handle;
    friend class ShaderProgram;
};

class ShaderProgram {
public:
    ShaderProgram(std::initializer_list<Shader> shaderList);

    void use() const;
    void unuse() const;

    GLuint getHandle() const;

    GLint attribute(const std::string& name);
    void setAttribute(const std::string& name, GLint size, GLsizei stride, GLuint offset, GLboolean normalize, GLenum type);
    void setAttribute(const std::string& name, GLint size, GLsizei stride, GLuint offset, GLboolean normalize);
    void setAttribute(const std::string& name, GLint size, GLsizei stride, GLuint offset, GLenum type);
    void setAttribute(const std::string& name, GLint size, GLsizei stride, GLuint offset);

    GLint uniform(const std::string& name);
    GLint operator[](const std::string& name);

    void setUniform(const std::string& name, float x, float y, float z);
    void setUniform(const std::string& name, const glm::vec3& v);
    void setUniform(const std::string& name, const glm::dvec3& v);
    void setUniform(const std::string& name, const glm::vec4& v);
    void setUniform(const std::string& name, const glm::dvec4& v);
    void setUniform(const std::string& name, const glm::dmat4& m);
    void setUniform(const std::string& name, const glm::mat4& m);
    void setUniform(const std::string& name, const glm::mat3& m);
    void setUniform(const std::string& name, float val);
    void setUniform(const std::string& name, int val);

    ~ShaderProgram();

private:
    ShaderProgram();

    std::map<std::string, GLint> uniforms;
    std::map<std::string, GLint> attributes;

    GLuint handle;
    void link();
};