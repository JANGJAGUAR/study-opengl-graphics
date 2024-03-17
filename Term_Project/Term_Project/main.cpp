#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <cmath>
#include <string>
//#define STB_IMAGE_IMPLEMENTATION
#include <shader.h>
#include <cube.h>
#include <arcball.h>

#include <stb_image.h>
#include <mass.h>
#include <plane.h>
#include <text.h>
#include <Model.h>

#include "quadpyra.h"
#include "football.h"
#include "cubebar.h"


using namespace std;


// Function Prototypes
GLFWwindow *glAllInit();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action , int mods);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow *window, double x, double y);
void loadTexture();
void render();
void particleInit(int i);
void updateACTIVEData();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


// Global variables
GLFWwindow *mainWindow = NULL;
Shader *lightingShader = NULL;
Shader* l1 = NULL;
Shader* l2 = NULL;
Shader* l3 = NULL;
Shader *goalpostShader = NULL;
Shader* boxShader = NULL;
Shader* buttonShader = NULL;

Shader* BallShader = NULL;
Shader* BlueShader = NULL;


Shader* textShader = NULL;
Text* text = NULL;
Shader* groundShader = NULL;
Shader* particleShader = NULL;
int nParticle = 500;
Mass** particle;

Shader *lampShader = NULL;
Shader *globalShader = NULL;
unsigned int SCR_WIDTH = 1200;
unsigned int SCR_HEIGHT = 1200;

Cubebar* BG;
Cubebar* Back;
Cubebar* Side1;
Cubebar* Side2;
Cubebar* Side3;

FootBall* ball;

Cubebar* goalpost;
Cubebar* postside1;
Cubebar* postside2;

Cubebar* powerbar;
Cubebar* powerbutton;
Cubebar* directbar;
Cubebar* directbutton;

Cubebar* target;


Shader* rbshader;
Model* ourModel;

Cube *lamp;
glm::mat4 projection, view, viewp, viewr, modelr, model, model1, model2, model3, model4;

// flat <-> smooth change
bool fsChange = true;

// for arcball
float arcballSpeed = 0.2f;
static Arcball camArcBall(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
static Arcball modelArcBall(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
bool arcballCamRot = true;

bool shootstart = false;
bool nextgame = true;
int gamenum = -1;
float cTime = 0.0f;
float cTimebutton = 0.0f;

int score = 0;
bool xstop = false;
float xstoptime = 0.0f;
bool ystop = false;
float ystoptime = 0.0f;
float direct = 0;
float power = 0;


// for camera
glm::vec3 cameraPos(0.0f, 1.5f, 7.0f);
glm::vec3 cameraOrigPos(0.0f, 0.0f, 9.0f); 
glm::vec3 modelPan(-1.0f, 0.0f, 3.0f);///////////////////////////init

// for lighting
glm::vec3 lightSize(0.1f, 0.1f, 0.1f);
glm::vec3 lightPos(1.5f, 2.0f, 1.5f);
glm::vec3 spotDirect(-1.0f, -1.0f, -1.0f);

glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 objectColor(0.8f, 0.8f, 0.8f);
float ambientStrenth = 30.0f;
float specularStrength = 0.1f;
float specularPower = 64.0f;

// for texture
static unsigned int texture;// Array of texture ids.

Plane* ground;                                  // ground
float groundY = 0.0f;                            // ground's y coordinates
float groundScale = 20.0f;
// for ACTIVEation
enum RenderMode { INIT, ACTIVE, INACTIVE };
RenderMode* renderMode;
float timeT = 0.0f;       
float deltaT = 1.0f / 30.0f;

bool spaceOn = false;
bool game[10] = { 0,0,0,0,0, 0,0,0,0,0 };
float wmin[10] = { -0.15f,  0.6f, -0.8f,  0.6f, -0.15f, -0.8f,  0.6f, -0.8f, -0.15f, -0.15f };
float wmax[10] = { 0.15f,  0.8f, -0.6f,  0.8f,  0.15f, -0.6f,  0.8f, -0.6f,  0.15f,  0.15f };
float hmin[10] = { 0.3f,  0.6f,  0.3f,  0.0f,   0.6f,  0.0f,  0.3f,  0.6f,   0.0f,   0.3f };
float hmax[10] = { 0.5f,  0.8f,  0.5f,  0.2f,   0.8f,  0.2f,  0.5f,  0.8f,   0.2f,   0.5f };

float RNG(float min, float max)
{
    return min + rand() / (float)RAND_MAX * (max - min);
}

int main()
{
    srand(0);
    mainWindow = glAllInit();

    rbshader = new Shader("res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag");
    ourModel = new Model((GLchar*)"res/models/nanosuit/nanosuit.obj");

    textShader = new Shader("text.vs", "text.frag");
    text = new Text((char*)"fonts/arial.ttf", textShader, SCR_WIDTH, SCR_HEIGHT);
    particleShader = new Shader("particle.vs", "particle.fs");
    groundShader = new Shader("ground.vs", "ground.fs");

    // shader loading and compile (by calling the constructor)
    lightingShader = new Shader("09lighting_maps.vs", "09lighting_maps.fs");
    l1 = new Shader("09lighting_maps.vs", "09lighting_maps.fs");
    l2 = new Shader("09lighting_maps.vs", "09lighting_maps.fs");
    l3 = new Shader("09lighting_maps.vs", "09lighting_maps.fs");
    lampShader = new Shader("09lamp.vs", "09lamp.fs");
    globalShader = new Shader("4.2.transform.vs", "4.2.transform.fs");
    goalpostShader = new Shader("basic_lighting.vs", "basic_lighting.fs");

    boxShader = new Shader("basic_lighting.vs", "basic_lighting.fs");
    buttonShader = new Shader("basic_lighting.vs", "basic_lighting.fs");
    
    BallShader = new Shader("global2.vs", "global2.fs");
    BlueShader = new Shader("global.vs", "global.fs");
    
    goalpostShader->use();
    glm::mat4 goalpostProjection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    goalpostShader->setMat4("projection", goalpostProjection);
    goalpostShader->setVec3("objectColor", objectColor);
    goalpostShader->setVec3("lightColor", lightColor);
    goalpostShader->setVec3("lightPos", lightPos);
    goalpostShader->setVec3("viewPos", cameraPos);
    goalpostShader->setFloat("ambientStrenth", ambientStrenth);
    goalpostShader->setFloat("specularStrength", specularStrength);
    goalpostShader->setFloat("specularPower", specularPower);
    
    boxShader->use();
    boxShader->setMat4("projection", goalpostProjection);
    boxShader->setVec3("lightColor", lightColor);
    boxShader->setVec3("lightPos", lightPos);
    boxShader->setVec3("viewPos", cameraPos);

    buttonShader->use();
    buttonShader->setMat4("projection", goalpostProjection);
    buttonShader->setVec3("lightColor", lightColor);
    buttonShader->setVec3("lightPos", lightPos);
    buttonShader->setVec3("viewPos", cameraPos);
    BallShader->use();
    BallShader->setMat4("projection", goalpostProjection);
    BlueShader->use();
    BlueShader->setMat4("projection", goalpostProjection);
    ///////////Modeling
    rbshader->use();
    rbshader->setMat4("projection", goalpostProjection);



    groundShader->use();
    groundShader->setMat4("projection", goalpostProjection);
    particleShader->use();
    particleShader->setMat4("projection", goalpostProjection);
    particle = new Mass * [nParticle];
    renderMode = new RenderMode[nParticle];
    for (int i = 0; i < nParticle; i++) {
        // particle initialization
        particleInit(i);
    }
    ground = new Plane(0.0f, 0.0f, 0.0f, groundScale);
    updateACTIVEData();

    // projection and view matrix
    lightingShader->use();
    projection = glm::perspective(glm::radians(45.0f),
                                  (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    lightingShader->setMat4("projection", projection);
    l1->use();
    l1->setMat4("projection", projection);
    l2->use();
    l2->setMat4("projection", projection);
    l3->use();
    l3->setMat4("projection", projection);

    lampShader->use();
    lampShader->setMat4("projection", projection);
    
    // load texture
    loadTexture();
    
    // create a cube
    ball = new FootBall();

    goalpost = new Cubebar();
    postside1 = new Cubebar();
    postside2 = new Cubebar();


    powerbar = new Cubebar();;
    powerbutton = new Cubebar();
    directbar = new Cubebar();
    directbutton = new Cubebar();


    target = new Cubebar();
    
    BG = new Cubebar();
    Back = new Cubebar();
    Side1 = new Cubebar();
    Side2 = new Cubebar();

    Side3 = new Cubebar();
    lamp = new Cube();

    while (!glfwWindowShouldClose(mainWindow)) {
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
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // glfw window creation
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "TermProjrct", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // OpenGL states
    glClearColor(0.2f, 0.2f, 0.2f, 0.1f);
    //glClearColor(0.4f, 0.5f, 1.0f, 1.0f);//sky
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Allow modern extension features
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cout << "GLEW initialisation failed!" << endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(-1);
    }
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    return window;
}

void loadTexture() {
    
    // Create texture ids.
    glGenTextures(1, &texture);
    
    // All upcomming GL_TEXTURE_2D operations now on "texture" object
    glBindTexture(GL_TEXTURE_2D, texture);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);   // vertical flip the texture
    unsigned char *image = stbi_load("field1.png", &width, &height, &nrChannels, 0);
    
    
    GLenum format;
    if (nrChannels == 1) format = GL_RED;
    else if (nrChannels == 3) format = GL_RGB;
    else if (nrChannels == 4) format = GL_RGBA;
    
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap(GL_TEXTURE_2D);
    
}

void particleInit(int i) {
    // particle initialization
    float mass = RNG(1, 1.5);
    particle[i] = new Mass(mass);
    // Homework13 here: particle coordinates initialization to (0.0, 5.0, 0.0)
    particle[i]->setPosition(0.0, 0.1, 0.0);
    // Homework13 here: particle velocitiy initialization to (0.0, 0.0, 0.0)
    particle[i]->setVelocity(0.0, 0.0, 0.0);
    // Homework13 here: particle accelaration initialization to (0.0, 0.0, 0.0)
    particle[i]->setAcceleration(0.0, 0.0, 0.0);

    // render mode initialization
    renderMode[i] = INIT;
}
void updateACTIVEData() {
    int max_fire = 5;	// # maximum fired particles per frame
    int cur_fire = 0;
    for (int i = 0; i < nParticle; i++) {
        if (cur_fire < max_fire && renderMode[i] == INACTIVE) {
            particleInit(i);
            float xForce = RNG(-20, 20);
            float yForce = RNG(230, 250);
            float zForce = RNG(-20, 20);
            // Homework13 here: Euler simulation with forces in 3 directions (xForce, yForce, zForce)
            particle[i]->euler(timeT, deltaT, xForce, yForce, zForce);
            renderMode[i] = ACTIVE;
            cur_fire++;
        }
        else if (renderMode[i] == ACTIVE) {
            particle[i]->euler(timeT, deltaT, 0.0, 0.0, 0.0);
            if (particle[i]->p[1] < groundY) {
                particle[i]->p[1] = groundY;
                renderMode[i] = INACTIVE;
            }
        }

    }

    // Homework13 here: update timeT
    //timeT = timeT + deltaT;
}

void render() {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    view = view * camArcBall.createRotationMatrix();

    // Drawing texts
    
    updateACTIVEData();
    viewp = glm::lookAt(glm::vec3(0.0f, 10.0f, 20.0f), glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    viewp = view * camArcBall.createRotationMatrix();
    particleShader->use();
    particleShader->setMat4("view", viewp);
    model = glm::mat4(1.0);
    particleShader->setMat4("model", model);
    
    ////////Modeling
    rbshader->use();
    glm::mat4 viewr = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    viewr = viewr * camArcBall.createRotationMatrix();
    rbshader->setMat4("view", viewr);

    glm::mat4 modelr(1.0);
    modelr = glm::translate(modelr, modelPan);
    modelr = glm::scale(modelr, glm::vec3(0.05f, 0.05f, 0.05f));
    modelr = glm::rotate(modelr, (float)glm::radians(180.0f)+(float)glm::radians(20.0f)*modelPan[0], glm::vec3(0.0f, 1.0f, 0.0f));
    modelr = modelr * modelArcBall.createRotationMatrix();

    rbshader->setMat4("model", modelr);
    ourModel->Draw(rbshader);

    float t = 0;
    if (shootstart)
    {   
        BallShader->use();
        BallShader->setMat4("view", view);
        ////////////////////////////////////////////////////
        //BALL's Action     BALL's Action   BALL's Action   BALL's Action   BALL's Action
        ////////////////////////////////////////////////////
        model = glm::mat4(1.0f);
        model = model * modelArcBall.createRotationMatrix();
        float xpower = 0.80f * (8.0f * sqrt(1- (power * power)+2.0f));
        float ypower = (16 / 10) * (6.0f * power+0.8f);
        //model = glm::rotate(model, (float)glm::radians(360.0f) * (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        t = (float)glfwGetTime() - cTime;
        model = glm::translate(model, glm::vec3(direct*5.0f*t, (t * ypower - ((t * t) * 8 / 10)), -t * xpower));
        model = glm::rotate(model, (float)glfwGetTime()*30.0f, glm::vec3(-1.0f, 0.0f, -1.0f* direct));
        
        if ((t * ypower - ((t * t) * 8 / 10)) < -3.0f)
        {
            cTimebutton = (float)glfwGetTime();
            nextgame = true;
            shootstart = false;
            xstop = false;
            ystop = false;
        }
        
    }
    else
    {
        BallShader->use();
        BallShader->setMat4("view", view);
        model = glm::mat4(1.0f);
        model = model * modelArcBall.createRotationMatrix();
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    }


    model = model * modelArcBall.createRotationMatrix();
    BallShader->setMat4("model", model);
    ball->draw(BallShader, fsChange);


    lightingShader->use();
    lightingShader->setMat4("view", view);



    BlueShader->use();
    BlueShader->setMat4("view", view);

    




    l1->use();
    l1->setMat4("view", view);

    l2->use();
    l2->setMat4("view", view);

    l3->use();
    l3->setMat4("view", view);

    lightingShader->use();
    lightingShader->setMat4("view", view);

    lightingShader->use();
    lightingShader->setMat4("view", view);

    // be sure to activate shader when setting uniforms/drawing objects
    lightingShader->setVec3("light.position", lightPos);
    lightingShader->setVec3("light.direction", spotDirect);
    lightingShader->setFloat("light.cutOff", glm::cos(glm::radians(25.0f)));
    lightingShader->setVec3("viewPos", cameraPos);

    // be sure to activate shader when setting uniforms/drawing objects
    l1->setVec3("light.position", lightPos);
    l1->setVec3("light.direction", spotDirect);
    l1->setFloat("light.cutOff", glm::cos(glm::radians(25.0f)));
    l1->setVec3("viewPos", cameraPos);
    // be sure to activate shader when setting uniforms/drawing objects
    l2->setVec3("light.position", lightPos);
    l2->setVec3("light.direction", spotDirect);
    l2->setFloat("light.cutOff", glm::cos(glm::radians(25.0f)));
    l2->setVec3("viewPos", cameraPos);
    // be sure to activate shader when setting uniforms/drawing objects
    l3->setVec3("light.position", lightPos);
    l3->setVec3("light.direction", spotDirect);
    l3->setFloat("light.cutOff", glm::cos(glm::radians(25.0f)));
    l3->setVec3("viewPos", cameraPos);


    // light properties
    lightingShader->setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
    lightingShader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    lightingShader->setVec3("light.specular", 0.5f, 0.5f, 0.5f);

    l1->setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
    l1->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    l1->setVec3("light.specular", 0.5f, 0.5f, 0.5f);

    l2->setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
    l2->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    l2->setVec3("light.specular", 0.5f, 0.5f, 0.5f);

    l3->setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
    l3->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    l3->setVec3("light.specular", 0.5f, 0.5f, 0.5f);

    // material properties
    lightingShader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    lightingShader->setFloat("material.shininess", 100.0f);

    l1->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    l1->setFloat("material.shininess", 100.0f);

    l2->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    l2->setFloat("material.shininess", 100.0f);

    l3->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    l3->setFloat("material.shininess", 100.0f);


    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(4.5f, 0.1f, 6.0f));
    model = glm::translate(model, glm::vec3(0.0f, -1.8f, 0.065f));
    // model matrix
    lightingShader->setMat4("model", model);
    
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    BG->draw(lightingShader);


    //ball->draw(BallShader, fsChange);

    ///lightShaader3
    model1 = glm::mat4(1.0f);
    model1 = glm::scale(model1, glm::vec3(4.5f, 4.5f, 4.0f));
    model1 = glm::translate(model1, glm::vec3(0.0f, 0.0f, 0.0f));
    model1 = model1 * modelArcBall.createRotationMatrix();
    BlueShader->setMat4("model", model1);
    BlueShader->setVec3("objectColor", glm::vec3(0.4f, 0.5f, 1.0f));
    Back->draw(BlueShader);

    model2 = glm::mat4(1.0f);
    model2 = glm::scale(model2, glm::vec3(0.1f, 20.0f, 50.0f));
    model2 = glm::translate(model2, glm::vec3(-100.0f, 0.0f, 0.065f));
    BlueShader->setMat4("model", model2);
    BlueShader->setVec3("objectColor", glm::vec3(0.4f, 0.5f, 1.0f));
    Side1->draw(BlueShader);

    model3 = glm::mat4(1.0f);
    model3 = glm::scale(model3, glm::vec3(0.1f, 20.0f, 50.0f));
    model3 = glm::translate(model3, glm::vec3(100.0f, 0.0f, 0.065f));
    BlueShader->setMat4("model", model3);
    BlueShader->setVec3("objectColor", glm::vec3(0.4f, 0.5f, 1.0f));
    Side2->draw(BlueShader);

    model4 = glm::mat4(1.0f);
    model4 = glm::scale(model4, glm::vec3(50.0f, 50.0f, 0.1f));
    model4 = glm::translate(model4, glm::vec3(0.0f, 0.0f, -300.0f));
    BlueShader->setMat4("model", model4);
    BlueShader->setVec3("objectColor", glm::vec3(0.4f, 0.5f, 1.0f));
    Side3->draw(BlueShader);



    goalpostShader->use();
    goalpostShader->setMat4("view", view);
    

    //post
    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(4.1f, 0.1f, 0.1f));
    model = glm::translate(model, glm::vec3(0.0f, 20.0f, -40.0f));
    model = model * modelArcBall.createRotationMatrix();
    goalpostShader->setMat4("model", model);
    goalpost->draw(goalpostShader);
    //side1
    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.1f, 2.2f, 0.1f));
    model = glm::translate(model, glm::vec3(-20.0f, 0.4f, -40.0f));
    model = model * modelArcBall.createRotationMatrix();
    goalpostShader->setMat4("model", model);
    postside1->draw(goalpostShader);
    //side2
    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.1f, 2.2f, 0.1f));
    model = glm::translate(model, glm::vec3(20.0f, 0.4f, -40.0f));
    model = model * modelArcBall.createRotationMatrix();
    goalpostShader->setMat4("model", model);
    postside2->draw(goalpostShader);
    
    //powerbar
    //boxShader->use();
    //boxShader->setMat4("view", view);
    //boxShader->setVec3("objectColor", glm::vec3(1.0f, 1.0f, 0.5f));
    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.4f, 2.2f, 0.1f));
    model = glm::translate(model, glm::vec3(6.5f, 0.5f, -40.0f));
    model = model * modelArcBall.createRotationMatrix();
    goalpostShader->setMat4("model", model);
    powerbar->draw(goalpostShader);

    //directbar
    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(4.5f, 0.4f, 0.1f));
    model = glm::translate(model, glm::vec3(0.0f, 6.5f, -40.0f));
    model = model * modelArcBall.createRotationMatrix();
    goalpostShader->setMat4("model", model);
    directbar->draw(goalpostShader);


    buttonShader->use();
    buttonShader->setMat4("view", view);
    buttonShader->setVec3("objectColor", glm::vec3(0.0f, 1.0f, 0.3f));
    float xtime = (float)glfwGetTime() - cTimebutton;
    float ytime = (float)glfwGetTime() - cTimebutton;

    //powerbutton
    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.3f, 0.1f, 0.1f));
    if (ystop == false)
        model = glm::translate(model, glm::vec3(8.65f, 10.0f + sin(4 * ytime) * 9.0f, -39.0f));//1.0~19.0 y
    else
    {
        model = glm::translate(model, glm::vec3(8.65f, 10.0f + sin(4 * ystoptime) * 9.0f, -39.0f));//1.0~19.0 y
        power = (sin(4 * ystoptime)+1)/2; //0~1
    }
        
    model = model * modelArcBall.createRotationMatrix();
    buttonShader->setMat4("model", model);
    powerbutton->draw(buttonShader);

    //directbutton
    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.1f, 0.3f, 0.1f));
    if (xstop == false)
        model = glm::translate(model, glm::vec3(sin(4 * xtime) * 19.0f, 8.65f, -39.0f)); //-19.0~19.0 x
    else
    {
        model = glm::translate(model, glm::vec3(sin(4 * xstoptime) * 19.0f, 8.65f, -39.0f)); //-19.0~19.0 x
        direct = sin(4 * xstoptime);
    }
        
    model = model * modelArcBall.createRotationMatrix();
    buttonShader->setMat4("model", model);
    directbutton->draw(buttonShader);
    
    

    //target box
    float w[10] = { 0.0f,  7.0f, -7.0f,  7.0f,  0.0f, -7.0f,  7.0f, -7.0f,  0.0f,  0.0f};
    float h[10] = { 4.6f,  9.0f,  4.6f,  0.2f,  9.0f,  0.2f,  4.6f,  9.0f,  0.2f,  4.6f};
    
    if (nextgame == true)
    {
        gamenum++;
        nextgame = false;
    }
    boxShader->use();
    boxShader->setMat4("view", view);
    boxShader->setVec3("objectColor", glm::vec3(1.0f, 1.0f, 0.5f));
    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.25f, 0.2f, 0.1f));
    model = glm::translate(model, glm::vec3(w[gamenum], h[gamenum], -40.0f));
    model = model * modelArcBall.createRotationMatrix();
    boxShader->setMat4("model", model);
    target->draw(boxShader);
    

    
    
    //string gntext = "game : " + gamenum;
    //string sctext = [ "01/10", "01/10", "01/10", "01/10", "01/10", "01/10", "01/10", "01/10", "01/10", "01/10"];
    //text->RenderText(gntext, 600.0f, 850.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.2f));
    if (gamenum==0) text->RenderText("GAME  : 01/10", 25.0f, 1100.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));
    else if (gamenum == 1) text->RenderText("GAME  : 02/10", 25.0f, 1100.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));
    else if (gamenum == 2) text->RenderText("GAME  : 03/10", 25.0f, 1100.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));
    else if (gamenum == 3) text->RenderText("GAME  : 04/10", 25.0f, 1100.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));
    else if (gamenum == 4) text->RenderText("GAME  : 05/10", 25.0f, 1100.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));
    else if (gamenum == 5) text->RenderText("GAME  : 06/10", 25.0f, 1100.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));
    else if (gamenum == 6) text->RenderText("GAME  : 07/10", 25.0f, 1100.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));
    else if (gamenum == 7) text->RenderText("GAME  : 08/10", 25.0f, 1100.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));
    else if (gamenum == 8) text->RenderText("GAME  : 09/10", 25.0f, 1100.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));
    else text->RenderText("GAME  : 10/10", 25.0f, 1100.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));
    
    if (game[gamenum] == 0)
    {
        if ((wmin[gamenum] < direct && direct < wmax[gamenum]) && (hmin[gamenum] < power && power < hmax[gamenum]))
        {
            //t초후에
            score++;
            game[gamenum] = 1;
            spaceOn = false;
        }
    }
    
    

    if (score == 0) text->RenderText("SCORE : 00/10", 25.0f, 1070.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));
    else if (score == 1) text->RenderText("SCORE : 01/10", 25.0f, 1070.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));
    else if (score == 2) text->RenderText("SCORE : 02/10", 25.0f, 1070.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));
    else if (score == 3) text->RenderText("SCORE : 03/10", 25.0f, 1070.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));
    else if (score == 4) text->RenderText("SCORE : 04/10", 25.0f, 1070.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));
    else if (score == 5) text->RenderText("SCORE : 05/10", 25.0f, 1070.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));
    else if (score == 6) text->RenderText("SCORE : 06/10", 25.0f, 1070.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));
    else if (score == 7) text->RenderText("SCORE : 07/10", 25.0f, 1070.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));
    else if (score == 8) text->RenderText("SCORE : 08/10", 25.0f, 1070.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));
    else if (score == 9) text->RenderText("SCORE : 09/10", 25.0f, 1070.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));
    else text->RenderText("SCORE : 10/10", 25.0f, 1070.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.5f));


    text->RenderText("SPACE : Shooting", 25.0f, 1000.0f, 0.6f, glm::vec3(0.8f, 0.8f, 0.8f));
    text->RenderText("F : Power Control", 25.0f, 970.0f, 0.6f, glm::vec3(0.8f, 0.8f, 0.8f));
    text->RenderText("D : Direction Control", 25.0f, 940.0f, 0.6f, glm::vec3(0.8f, 0.8f, 0.8f));
    text->RenderText("R : Camera Reset", 25.0f, 910.0f, 0.6f, glm::vec3(0.8f, 0.8f, 0.8f));
    text->RenderText("N : Next Game", 25.0f, 880.0f, 0.6f, glm::vec3(0.8f, 0.8f, 0.8f));
    text->RenderText("<> : Robot Move", 25.0f, 850.0f, 0.6f, glm::vec3(0.8f, 0.8f, 0.8f));
    text->RenderText("DIRECTION", 535.0f, 1005.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.2f));
    text->RenderText("POWER", 1000.0f, 850.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.2f));

    if (gamenum == 10 && score >= 5)
    {
        text->RenderText("Congratulations!!", 460.0f, 1150.0f, 0.8f, glm::vec3(1.0f, 1.0f, 0.0f));
        for (int i = 0; i < nParticle; i++) particle[i]->draw(particleShader, 1.0f, 1.0f, 0.0f);
        for (int i = 0; i < nParticle; i++)
        {
            if (renderMode[i] == INIT)
            {
                renderMode[i] = INACTIVE;
            }
        }
        timeT = 0.0f;
    }
    else if (gamenum == 10 && score < 5)
    {
        text->RenderText("Failed...Try Again!!", 420.0f, 1150.0f, 0.8f, glm::vec3(1.0f, 0.0f, 0.0f));
        for (int i = 0; i < nParticle; i++) particle[i]->draw(particleShader, 1.0f, 0.0f, 0.0f);
        for (int i = 0; i < nParticle; i++)
        {
            if (renderMode[i] == INIT)
            {
                renderMode[i] = INACTIVE;
            }
        }
        timeT = (float)glfwGetTime();
    }



    // cube object

    // lamp
    lampShader->use();
    lampShader->setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, lightSize);
    lampShader->setMat4("model", model);
    lamp->draw(lampShader);
 
    glm::mat4 transform = glm::mat4(1.0f);
    unsigned int transformLoc = glGetUniformLocation(lampShader->ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    glfwSwapBuffers(mainWindow);
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
    projection = glm::perspective(glm::radians(45.0f),
        (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    particleShader->use();
    particleShader->setMat4("projection", projection);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        //RESET
        
        camArcBall.init(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
        modelArcBall.init(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
        modelPan[0] = -1.0f;
        modelPan[1] =  0.0f;
        modelPan[2] =  3.0f;
    }
    else if (key == GLFW_KEY_N && action == GLFW_PRESS) {
        //RESET

        camArcBall.init(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
        modelArcBall.init(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
        nextgame = true;
        shootstart = false;
        cTime = (float)glfwGetTime();
        cTimebutton = (float)glfwGetTime();
        xstop = false;
        ystop = false;

    }///////////////////////////////////////////////////삭제요청
    else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        arcballCamRot = !arcballCamRot;
        if (arcballCamRot) {
            cout << "ARCBALL: Camera rotation mode" << endl;
        }
        else {
            cout << "ARCBALL: Model  rotation mode" << endl;
        }
    }
    /*
    else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        fsChange = !fsChange;
        if (fsChange) {
            cout << "ARCBALL: Flat Shading mode" << endl;
        }
        else {
            cout << "ARCBALL: Smooth Shading mode" << endl;
        }
    }
    */
    else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        //direct
        xstop = true;
        xstoptime= (float)glfwGetTime() - cTimebutton;

    }
    else if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        //power
        ystop = true;
        ystoptime = (float)glfwGetTime() - cTimebutton;
    }

    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        shootstart = true;
        cTime = (float)glfwGetTime();
        spaceOn = true;
        direct = 0.0f;
        power = 0.0f;
    }
    else if (key == GLFW_KEY_LEFT) {
        modelPan[0] -= 0.1;
    }
    else if (key == GLFW_KEY_RIGHT) {
        modelPan[0] += 0.1;
    }
    else if (key == GLFW_KEY_DOWN) {
        modelPan[1] -= 0.1;
    }
    else if (key == GLFW_KEY_UP) {
        modelPan[1] += 0.1;
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (arcballCamRot)
        camArcBall.mouseButtonCallback( window, button, action, mods );
    else
        modelArcBall.mouseButtonCallback( window, button, action, mods );
}

void cursor_position_callback(GLFWwindow *window, double x, double y) {
    if (arcballCamRot)
        camArcBall.cursorCallback( window, x, y );
    else
        modelArcBall.cursorCallback( window, x, y );
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    cameraPos[2] -= (yoffset * 0.5);
}