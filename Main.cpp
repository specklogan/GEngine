#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <GLEW/glew.h>
#include "logging.cpp"
#include <GLFW/glfw3.h>
#include <limits.h>
#include "shader.cpp"
#include <fstream>
#include <GLM/vec3.hpp>
#include <GLM/vec4.hpp>
#include <math.h>
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION    
#include "STB/stb_image.h"
using namespace std;

Logging myLogger;

const char *shaderLocation = "./data/shader/vertex.glsl";
const char *fragLocation = "./data/shader/fragment.glsl";
//string shaderString = readFileAsString(shaderLocation);
//string fragString = readFileAsString(fragLocation);
//const char *vertexShaderSource = shaderString.c_str();
//const char *fragmentShaderSource = fragString.c_str();


bool lineMode = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        if (lineMode == true) {
            lineMode = false;
        }
        else {
            lineMode = true;
        }
    }
};

int main(void)
{

    static string versionName = "GEngine 0.1a";
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "GEngine 0.1a", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glewInit();
    glfwSwapInterval(1);

    bool hasDoneBefore = false;

    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    //Image stuff testing


    // float triangleTwo[] = {
    //     0.0f, 1.0f, 0.0f,
    //     0.25f, 0.0f, 0.0f,
    //     1.0f, 0.5f, 0.0f
    // };

    unsigned int indices[] = {
        0,1,3,
        1,2,3
    };

    unsigned int VBO; //Vertex Buffer Object 
    /*
        First part of the pipelines, indices (from our verticies thingy) are given to the GPU
    */

    glGenBuffers(1, &VBO); /*

        We generate a buffer and assign the ID to VBO (something we created), we bound the data to the buffer,
        then send "buffer" that data with our third command

    */

    //VAO time, more effecient method to help writing a lot of things
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //Color attribute
    /*glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);*/

    // unsigned int VBOTWO;
    // glGenBuffers(1, &VBOTWO);
    // unsigned int VAOTWO;
    // glGenVertexArrays(1, &VAOTWO);
    // glBindVertexArray(VAOTWO);
    // glBindBuffer(GL_ARRAY_BUFFER, VBOTWO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(triangleTwo), triangleTwo, GL_STATIC_DRAW);
    // glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    // glEnableVertexAttribArray(0);

    //Create shader class

    //EBO can be used to not overuse the same vertexes for the same objects
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    int width, height, nrChannels;
    unsigned char *data = stbi_load("./data/image/brick_block.png", &width, &height, &nrChannels, 0);
    if (data) {
        myLogger.writeConfig("Data exists", myLogger.openLogFile());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        printf("Loaded image, %dpx width, a height of %dpx\n", width, height);
    }
    else {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free(data);

    Shader engineShader("./data/shader/vertex.glsl", "./data/shader/fragment.glsl");

    //Since openGL doesnt know how to interpret the vertices, we have to tell it how to, 
    //Each vertex is made up of 3 position values, XYZ, with each being 4 bytes.
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        if (!hasDoneBefore) {
            myLogger.writeConfig("Initializing log running version " + versionName, myLogger.openLogFile());
            hasDoneBefore = true;
        }
        glViewport(0, 0, width, height);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        engineShader.use();

        double time = glfwGetTime();
        if (lineMode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        };

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(VAOTWO);
        // glDrawArrays(GL_TRIANGLES, 0,3);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        glfwSetKeyCallback(window, key_callback);
    }

    glfwTerminate();
    return 0;
}