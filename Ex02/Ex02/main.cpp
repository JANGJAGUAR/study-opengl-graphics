#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action , int mods);

// settings
unsigned int SCR_WIDTH = 600;
unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentGreenShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\n\0";
const char* fragmentRedShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
    "}\n\0";

//globals
bool fillTriangle = false;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
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

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Homework02", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    // Allow modern extension features
    glewExperimental = GL_TRUE;
    
    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialisation failed!");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    int fragmentShaderGreen = glCreateShader(GL_FRAGMENT_SHADER);
    int fragmentShaderRed = glCreateShader(GL_FRAGMENT_SHADER);
    int shaderProgramGreen = glCreateProgram();
    int shaderProgramRed = glCreateProgram();

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShaderGreen, 1, &fragmentGreenShaderSource, NULL);
    glCompileShader(fragmentShaderGreen);
    glShaderSource(fragmentShaderRed, 1, &fragmentRedShaderSource, NULL);
    glCompileShader(fragmentShaderRed);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float verticesGreen[] = {
         0.0f,  0.0f, 0.0f,  // middle
         -0.5f,  0.4f, 0.0f,  // left(top)
         -0.5f,  -0.4f, 0.0f,  // left(bot)
         0.0f,  -0.8f, 0.0f,  // bottom
         0.5f,  -0.4f, 0.0f,  // right(bot)
         0.5f,  0.4f, 0.0f,   // right(top)
         0.0f,  0.8f, 0.0f,  // top
         - 0.5f,  0.4f, 0.0f  // left(top)
    };
    float verticesRed[] = {
         0.0f,  0.8f, 0.0f,  // top
         -0.5f,  0.4f, 0.0f,  // left(top)
         0.5f,  0.4f, 0.0f,  // right(top)
         -0.5f,  -0.4f, 0.0f,  // left(bot)
         0.5f,  -0.4f, 0.0f,  // right(bot)
         0.0f,  -0.8f, 0.0f   // bottom 
    };

    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesGreen), verticesGreen, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRed), verticesRed, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        
        //Green (Default)
        glAttachShader(shaderProgramGreen, vertexShader);
        glAttachShader(shaderProgramGreen, fragmentShaderGreen);
        glLinkProgram(shaderProgramGreen);

        glUseProgram(shaderProgramGreen);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 8);

        //Red (Toggle)
        if (fillTriangle) {
            glAttachShader(shaderProgramRed, vertexShader);
            glAttachShader(shaderProgramRed, fragmentShaderRed);
            glLinkProgram(shaderProgramRed);
            // draw our first triangle

            glUseProgram(shaderProgramRed);
            glBindVertexArray(VAOs[1]);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaderGreen);
    glDeleteShader(fragmentShaderRed);

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

// glfw: keyboard callback
// Toggles whenever P is pressed
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        if (fillTriangle) fillTriangle = false;
        else fillTriangle = true;
    }
}
