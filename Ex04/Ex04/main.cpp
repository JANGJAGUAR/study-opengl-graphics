#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <shader.h>

using namespace std;

// function prototypes
GLFWwindow *glAllInit();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow *window, double outX, double outY);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void normalize_cursor_position(double x, double y, float &inX, float &inY);
void update_vb_vertex(float x, float y);
void compute_intersections();
void render();

// global variables
GLFWwindow *window = NULL;
Shader globalShader;

unsigned int SCR_WIDTH = 600;
unsigned int SCR_HEIGHT = 600;

bool drag = false;


unsigned int curveVAO, curveVBO;
unsigned int lineVAO, lineVBO;
unsigned int interVAO, interVBO;

const int curvePoints = 64;

float curve[curvePoints * 2];
float plotArr[2 * 2];
float interV[2 * 2];

int main()
{
    window = glAllInit();
    globalShader.initShader("3.2.shader.vs", "3.2.shader.fs");

    // initialize object list
    for (int i = 0; i < curvePoints; i++) {
        float xi = float(i) / (curvePoints/2) -1.0f;
        curve[2 * i] = xi;
        curve[2 * i + 1] = 2 * (xi * xi) - 0.8 * (xi)-0.42f;
    }

    // render loop
    while (!glfwWindowShouldClose(window)) {
        render();
        glfwPollEvents();
    }

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
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Homework04", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    // set some OpenGL states (for transparency and background color)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    
    // Allow modern extension features
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cout << "GLEW initialisation failed!" << endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(-1);
    }
    
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);
    glGenVertexArrays(1, &curveVAO);
    glGenBuffers(1, &curveVBO);
    glGenVertexArrays(1, &interVAO);
    glGenBuffers(1, &interVBO);
    return window;
}

void render() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    globalShader.use();
    glBindVertexArray(curveVAO);
    glBindBuffer(GL_ARRAY_BUFFER, curveVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(curve), curve, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    globalShader.setVec4("inColor", 1.0f, 0.0f, 0.0f, 1.0f);
    glDrawArrays(GL_LINE_STRIP, 0, curvePoints);
    glBindVertexArray(0);

    if (!drag)
        compute_intersections();

    glBindVertexArray(lineVAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    globalShader.setVec4("inColor", 0.0f, 1.0f, 0.0f, 1.0f);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
    glfwSwapBuffers(window);
    
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    SCR_WIDTH = width;
    SCR_HEIGHT= height;
    glViewport(0, 0, width, height);
}

void cursor_position_callback(GLFWwindow *window, double outX, double outY)
{
    float inX, inY;
    if (drag) {
        normalize_cursor_position(outX, outY, inX, inY);
        update_vb_vertex(inX, inY);
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    double outX, outY;
    float inX, inY;
    
   
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            
            
        glBindVertexArray(lineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glfwGetCursorPos(window, &outX, &outY);
        normalize_cursor_position(outX, outY, inX, inY);
        plotArr[0] = plotArr[2] = inX;
        plotArr[1] = plotArr[3] = inY;
        glBufferData(GL_ARRAY_BUFFER, sizeof(plotArr), plotArr, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        drag = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        drag = false;
    }
}

void normalize_cursor_position(double x, double y, float &inX, float &inY)
{
    inX = ((float)x / (float)SCR_WIDTH) * 2.0f - 1.0f;
    inY = -1.0f * (((float)y / (float)SCR_HEIGHT) * 2.0f - 1.0f);
}

void update_vb_vertex(float x, float y)
{
    plotArr[2] = x;
    plotArr[3] = y;
    
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(plotArr), plotArr);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void compute_intersections() {    
    glBindVertexArray(interVAO);
    glBindBuffer(GL_ARRAY_BUFFER, interVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(interV), interV, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    float D = float(4 * (plotArr[2] - plotArr[0]) * plotArr[0] - 0.8 * (plotArr[2] - plotArr[0]) - (plotArr[3] - plotArr[1])) 
        * float(4 * (plotArr[2] - plotArr[0]) * plotArr[0] - 0.8 * (plotArr[2] - plotArr[0]) - (plotArr[3] - plotArr[1]))
        - 4 * float(2 * (plotArr[2] - plotArr[0]) * (plotArr[2] - plotArr[0])) 
        * float(2 * plotArr[0] * plotArr[0] - 0.8 * plotArr[0] - plotArr[1] - 0.42f);
    float tPlus = (-float(4 * (plotArr[2] - plotArr[0]) * plotArr[0] - 0.8 * (plotArr[2] - plotArr[0]) - (plotArr[3] - plotArr[1])) + sqrt(D)) / (2 * float(2 * (plotArr[2] - plotArr[0]) * (plotArr[2] - plotArr[0])));
    float tMinus = (-float(4 * (plotArr[2] - plotArr[0]) * plotArr[0] - 0.8 * (plotArr[2] - plotArr[0]) - (plotArr[3] - plotArr[1])) - sqrt(D)) / (2 * float(2 * (plotArr[2] - plotArr[0]) * (plotArr[2] - plotArr[0])));



    if ((0 <= tPlus && tPlus <= 1) && (0 <= tMinus && tMinus <= 1)) {
        interV[0] = (plotArr[2] - plotArr[0]) * tPlus + plotArr[0];
        interV[1] = (plotArr[3] - plotArr[1]) * tPlus + plotArr[1];
        interV[2] = (plotArr[2] - plotArr[0]) * tMinus + plotArr[0];
        interV[3] = (plotArr[3] - plotArr[1]) * tMinus + plotArr[1];
        glPointSize(10.0f);
        globalShader.setVec4("inColor", 1.0f, 1.0f, 0.0f, 1.0f);
        glBindVertexArray(interVAO);
        glDrawArrays(GL_POINTS, 0, 2);
        glBindVertexArray(0);
    }
    else if (!(0 <= tPlus && tPlus <= 1) && (0 <= tMinus && tMinus <= 1)) {
        interV[0] = (plotArr[2] - plotArr[0]) * tMinus + plotArr[0];
        interV[1] = (plotArr[3] - plotArr[1]) * tMinus + plotArr[1];
        glPointSize(10.0f);
        globalShader.setVec4("inColor", 1.0f, 1.0f, 0.0f, 1.0f);
        glBindVertexArray(interVAO);
        glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);
    }
    else if ((0 <= tPlus && tPlus <= 1) && !(0 <= tMinus && tMinus <= 1)) {
        interV[0] = (plotArr[2] - plotArr[0]) * tPlus + plotArr[0];
        interV[1] = (plotArr[3] - plotArr[1]) * tPlus + plotArr[1];
        glPointSize(10.0f);
        globalShader.setVec4("inColor", 1.0f, 1.0f, 0.0f, 1.0f);
        glBindVertexArray(interVAO);
        glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);
    }
    else {
        glBindVertexArray(interVAO);
        glBindVertexArray(0);
    }
}
