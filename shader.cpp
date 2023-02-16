#ifndef SHADER_H
#define SHADER_H

//#include <GLFW/glad.h>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iostream>

class Shader
{
public:
    // the program ID
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath) {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vertexFile;
        std::ifstream fragmentFile;

        vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            vertexFile.open(vertexPath);
            fragmentFile.open(fragmentPath);

            std::stringstream vertexStream, fragmentStream;
            vertexStream << vertexFile.rdbuf();
            fragmentStream << fragmentFile.rdbuf();

            //close files once we read them
            vertexFile.close();
            fragmentFile.close();
          
            //convert the stream we got into a string
            vertexCode = vertexStream.str();
            fragmentCode = fragmentStream.str();

        }
        catch (std::ifstream::failure& e) {
            std::cout << "Error in shader file reading: " << e.what() << std::endl;
        }

        const char* vertexShaderCode = vertexCode.c_str();
        const char* fragmentShaderCode = fragmentCode.c_str();
        unsigned int vertex, fragment; 
        //here we create the vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexShaderCode, NULL);
        glCompileShader(vertex);
        
        //create fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
        glCompileShader(fragment);

        //now we utilize shader program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        //clear a bit of memory
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use() {
        //We never tell it to use the program until now
        glUseProgram(ID);
    }
};
  
#endif