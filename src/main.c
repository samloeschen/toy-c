#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "io.h"

const int AA_SAMPLES = 4;
const int DEFAULT_WINDOW_WIDTH = 1024;
const int DEFAULT_WINDOW_HEIGHT = 768;

const char* SHADERS_PATH = "";

static const GLfloat vertexBufferData[] = {
    -0.9f, -0.9f, 0.0f,     0.0f, 0.0f,
    -0.9f,  0.9f, 0.0f,     0.0f, 1.0f,
     0.9f,  0.9f, 0.0f,     1.0f, 1.0f,
     0.9f, -0.9f, 0.0f,     1.0f, 0.0f,
};
static const GLint indexBufferData[] = {
    0, 1, 2,
    2, 0, 3,
};

void handleInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int loadShader(char *path, GLenum shaderType, GLuint *outId) {
    int result = GL_FALSE;
    int infoLogLength;
    char *shaderSource;
    GLuint shaderId;

    result = readAll(path, &shaderSource);
    if (result == IO_ERROR_FILE_DOES_NOT_EXIST) {
        fprintf(stderr, "File does not exist: %s\n", path);
        return result;
    }
    
    shaderId = glCreateShader(shaderType);
    fprintf(stdout, "Compiling shader: %s\n", path);

    glShaderSource(shaderId, 1, (const char**)&shaderSource, NULL);
    glCompileShader(shaderId);

    // check compiled shader
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        GLchar errMsg[infoLogLength + 1];
        glGetShaderInfoLog(shaderId, infoLogLength, NULL, errMsg);
        printf("%s\n", errMsg);
    }
    *outId = shaderId;
    free(shaderSource);
    return result;
}

int loadProgram(char *vertexShaderPath, char *fragmentShaderPath, GLuint *programId) {
    int result = GL_FALSE;
    int infoLogLength;
    GLuint fragmentId, vertexId;
    if (!loadShader(fragmentShaderPath, GL_FRAGMENT_SHADER, &fragmentId)) {
        return -1;
    }
    if (!loadShader(vertexShaderPath, GL_VERTEX_SHADER, &vertexId)) {
        return -1;
    }
    GLuint id = glCreateProgram();
    *programId = id;
    glAttachShader(id, vertexId);
    glAttachShader(id, fragmentId);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &result);
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        GLchar errMsg[infoLogLength + 1];
        glGetProgramInfoLog(id, infoLogLength, NULL, errMsg);
        printf("%s\n", errMsg);
    }
    glDetachShader(id, vertexId);
	glDetachShader(id, fragmentId);

    return result;
}

int main(int argc, char **argv) {
    if (!glfwInit()) {
        fprintf(stderr, "GLFW initialization failed!");
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, AA_SAMPLES);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // makes MacOS happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
    GLFWwindow* window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "Toy C", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to open GLFW window!\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW!\n");
        return -1;
    }

    // set up vao
    GLuint vaoId;
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    // set up test quad
    GLuint vertexBufferId;
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);
    GLuint indexBufferId;
    glGenBuffers(1, &indexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, indexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(indexBufferData), &indexBufferData[0], GL_STATIC_DRAW);

    GLuint programId;
    loadProgram("src/shaders/vertexTest.glsl", "src/shaders/fragmentTest.glsl", &programId);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    while (!glfwWindowShouldClose(window)) {
        handleInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw test quad
        glUseProgram(programId);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

        // positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,     
            3,     
            GL_FLOAT,     
            GL_FALSE,     
            sizeof(float) * 5,
            (void*)0            
        );

        // uvs
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * 5,
            (void*)(sizeof(float)*3)
        );

        glDrawArrays(GL_TRIANGLES, 0, 4);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        glDisableVertexAttribArray(0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    return 0;
}