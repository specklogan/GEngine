#ifndef SHADER_H
#define SHADER_H

#include <GLFW/glad.h> // include glad to get all the required OpenGL headers
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

    }
};
  
#endif