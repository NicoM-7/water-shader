#ifndef SHADERUTILS_HPP
#define SHADERUTILS_HPP

#include <GL/glew.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

std::string readFile(const char *filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open())
    {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::stringstream ss;
    ss << fileStream.rdbuf();
    content = ss.str();
    fileStream.close();
    return content;
}

GLuint compileShader(const char *filePath, GLenum shaderType)
{
    std::string code = readFile(filePath);
    const char *shaderCode = code.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    GLint result = GL_FALSE;
    int infoLogLength;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> errorMsg(infoLogLength + 1);
        glGetShaderInfoLog(shader, infoLogLength, NULL, &errorMsg[0]);
        std::cout << &errorMsg[0] << std::endl;
    }

    return shader;
}

GLuint LoadShaders(const char *vertex_file_path,
                   const char *tcs_file_path,
                   const char *tes_file_path,
                   const char *geo_file_path,
                   const char *fragment_file_path)
{
    GLuint VertexShaderID = compileShader(vertex_file_path, GL_VERTEX_SHADER);
    GLuint TCSShaderID = compileShader(tcs_file_path, GL_TESS_CONTROL_SHADER);
    GLuint TESShaderID = compileShader(tes_file_path, GL_TESS_EVALUATION_SHADER);
    GLuint GeometryShaderID = compileShader(geo_file_path, GL_GEOMETRY_SHADER);
    GLuint FragmentShaderID = compileShader(fragment_file_path, GL_FRAGMENT_SHADER);

    GLuint programID = glCreateProgram();
    glAttachShader(programID, VertexShaderID);
    glAttachShader(programID, TCSShaderID);
    glAttachShader(programID, TESShaderID);
    glAttachShader(programID, GeometryShaderID);
    glAttachShader(programID, FragmentShaderID);
    glLinkProgram(programID);

    GLint result = GL_FALSE;
    int infoLogLength;
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> errorMsg(infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength, NULL, &errorMsg[0]);
        std::cout << &errorMsg[0] << std::endl;
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(TCSShaderID);
    glDeleteShader(TESShaderID);
    glDeleteShader(GeometryShaderID);
    glDeleteShader(FragmentShaderID);

    return programID;
}

void setUniform(GLuint program, const char *name, float value)
{
    GLint loc = glGetUniformLocation(program, name);
    if (loc != -1)
        glUniform1f(loc, value);
}

void setUniform(GLuint program, const char *name, const glm::vec2 &vec)
{
    GLint loc = glGetUniformLocation(program, name);
    if (loc != -1)
        glUniform2fv(loc, 1, &vec[0]);
}

void setUniform(GLuint program, const char *name, const glm::vec3 &vec)
{
    GLint loc = glGetUniformLocation(program, name);
    if (loc != -1)
        glUniform3fv(loc, 1, &vec[0]);
}

void setUniform(GLuint program, const char *name, const glm::mat4 &mat)
{
    GLint loc = glGetUniformLocation(program, name);
    if (loc != -1)
        glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void setUniform(GLuint program, const char *name, int value)
{
    GLint loc = glGetUniformLocation(program, name);
    if (loc != -1)
        glUniform1i(loc, value);
}

#endif