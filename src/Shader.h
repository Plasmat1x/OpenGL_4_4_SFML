#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <sstream>
#include <string>
#include <fstream>
#include <iostream>

class Shader
{
public:

    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void Use();

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

    void setVec2(const std::string& name, glm::vec2& value) const;
    void setVec3(const std::string& name, glm::vec3& value) const;
    void setVec4(const std::string& name, glm::vec4& value) const;

    void setMat4(const std::string& name, glm::mat4& value) const;
    void setMat3(const std::string& name, glm::mat3& value) const;
    void setMat2(const std::string& name, glm::mat2& value) const;
};