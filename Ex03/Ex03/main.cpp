#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <shader.h> // include from utils

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void key_callback(GLFWwindow *window, int key, int scancode, int action , int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// settings
unsigned int SCR_WIDTH = 600;
unsigned int SCR_HEIGHT = 600;

//bool verticalFlip = false;
bool redgreenToggle = true;
float colorR = 1.0f, colorG = 0.0f, colorB = 0.0f;
double xpos=300, ypos=300;

int main()
{
    if (!glfwInit())
    {
        printf("GLFW initialisation failed!");
        glfwTerminate();
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Homework03", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialisation failed!");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // shader loading and compile
    Shader ourShader("shader03.vs", "shader03.fs");

    float vertices[] = {
         0.1f,  0.1f, 0.0f,     // top right
         0.1f, -0.1f, 0.0f,     // bottom right
        -0.1f, -0.1f, 0.0f,     // bottom left
        -0.1f,  0.1f, 0.0f      // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,
        1, 2, 3
    };
    
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        ourShader.setFloat("multX", xpos/300-1);
        ourShader.setFloat("multY", -ypos/300+1);
        ourShader.setVec4("outColor", colorR, colorG, colorB, 1.0);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button ==GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        if (redgreenToggle) {
            colorR = 0.0f;
            colorG = 1.0f;
            colorB = 0.0f;
            redgreenToggle = false;
        }
        else {
            colorR = 1.0f;
            colorG = 0.0f;
            colorB = 0.0f;
            redgreenToggle = true;
        }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &xpos, &ypos);
    }

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}
