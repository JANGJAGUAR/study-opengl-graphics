#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <shader.h>
#include <cmath>

using namespace std;

// Function Prototypes
GLFWwindow *glAllInit();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void render();

// Global variables
GLFWwindow *window = NULL;
Shader *globalShader = NULL;
unsigned int SCR_WIDTH = 600;
unsigned int SCR_HEIGHT = 600;
unsigned int VBO, VAO, EBO;
float vertices[] = {
    0.5f,  0.5f, 0.0f, 
    0.5f, -0.5f, 0.0f, 
    -0.5f, -0.5f, 0.0f, 
    -0.5f,  0.5f, 0.0f  
};
unsigned int indices[] = {
    0, 1, 3, 
    1, 2, 3  
};

int main()
{
    window = glAllInit();

    // shader loading and compile
    globalShader = new Shader("4.2.transform.vs", "4.2.transform.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        render();
        glfwPollEvents();
    }
        
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

GLFWwindow *glAllInit()
{
    GLFWwindow *window;
    
    // glfw: initialize and configure
    if (!glfwInit()) {
        printf("GLFW initialisation failed!");
        glfwTerminate();
        exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // glfw window creation
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Homework05", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cout << "GLEW initialisation failed!" << endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(-1);
    }
    
    return window;
}

void render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // 1. Green
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::rotate(transform, (float)glm::radians(360.0f)* (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(0.05f, 0.05f, 1.0f));

    globalShader->use();
    globalShader->setVec4("inColor", 0.0f, 1.0f, 0.0f, 1.0f);
    unsigned int transformLoc = glGetUniformLocation(globalShader->ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
 
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    // 2. Yellow
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(0.5f * sin((float)glfwGetTime()/3) * cos((float)glm::radians(90.0f) * (float)glfwGetTime()), 0.5f * sin((float)glfwGetTime() / 3) * sin((float)glm::radians(90.0f) * (float)glfwGetTime()), 0.0f));
    transform = glm::rotate(transform, (float)glm::radians(90.0f) * (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(0.05f, 0.05f, 1.0f));
    
    globalShader->use();
    globalShader->setVec4("inColor", 1.0f, 1.0f, 0.0f, 1.0f);
    transformLoc = glGetUniformLocation(globalShader->ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    
    // 3. Red
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(0.5f * sin((float)glfwGetTime()/3) * cos((float)glm::radians(90.0f) * (float)glfwGetTime()), 0.5f * sin((float)glfwGetTime() / 3) * sin((float)glm::radians(90.0f) * (float)glfwGetTime()), 0.0f));
    transform = glm::rotate(transform, (float)glm::radians(270.0f) * (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::translate(transform, glm::vec3(0.1f, 0.0f, 0.0f));
    transform = glm::scale(transform, glm::vec3(0.2f, 0.05f, 1.0f));   

    globalShader->use();
    globalShader->setVec4("inColor", 1.0f, 0.0f, 0.0f, 1.0f);
    transformLoc = glGetUniformLocation(globalShader->ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    glfwSwapBuffers(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}