#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader
{
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath)
    {

        // 1. retrieve the vertex/fragment source code from filePath
        string vertexCode;
        string fragmentCode;
        string geometryCode;
        ifstream vShaderFile;
        ifstream fShaderFile;
        ifstream gShaderFile;

        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (ifstream::failbit|ifstream::badbit);
        fShaderFile.exceptions (ifstream::failbit|ifstream::badbit);
        gShaderFile.exceptions (ifstream::failbit|ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            stringstream vShaderStream, fShaderStream;

            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            // close file handlers
            vShaderFile.close();
            fShaderFile.close();

            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (ifstream::failure e)
        {
            cout<<"ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ"<<endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();

        // compile shader
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        // delete the shader as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);


    }

    //Set the Current Open GL program
    void use()
    {
        glUseProgram(ID);
    }

    void setInt(const string &name, int value)
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setMat4(const string &name, const glm::mat4 &mat)
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setFloat(const string &name,float value) {
        glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
    }

private:

    void checkCompileErrors(GLuint shader,string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                cout << "ERROR::SHADER_COMPILATION_ERROR of type: "<<type<<"\n"<<infoLog<<endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"<<infoLog<<endl;
            }
        }
    }
};
#endif
