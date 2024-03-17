
// A Quadpyramid has 5 sides and each side has 3 or 4 vertices, therefore, the total number
// of vertices is 16 (4 sides * 3 verts + 1 sides * 4 verts), and 48 floats in the vertex array
// since each vertex has 3 components (x,y,z) (= 16 * 3)
//
//            a
//           /  \ 
//          /    \
//         /      \ 
//        e--------d 
//      / |        | \
//     a  |        |  a
//      \ |        | /
//        b--------c
//         \      /
//          \    /
//           \  / 
//            a
//
// Vertex shader: the location (0: position attrib (vec3), 2: color attrib (vec4))
//          -> disuse(1: normal attrib (vec3), 3: texture coordinate attrib (vec2))
// Fragment shader: should catch the vertex color from the vertex shader
    

#ifndef FOOTBALL_H
#define FOOTBALL_H

#include "shader.h"

class FootBall {
public:
    float phi = 1.618/2;
    // vertex position array
    GLfloat fVertices[180]  = { // initialized as size = 1 for each dimension, 72 elements
        .0f, phi,-.5f,  .0f, phi, .5f,   phi, .5f, .0f,
        .0f, phi, .5f,  .0f, phi,-.5f,  -phi, .5f, .0f,

        .0f, phi, .5f,  .5f, .0f, phi,   phi, .5f, .0f,
        .0f, phi, .5f, -phi, .5f, .0f,  -.5f, .0f, phi,
        .0f, phi, .5f, -.5f, .0f, phi,   .5f, .0f, phi,

        .0f, phi,-.5f,  phi, .5f, .0f,   .5f, .0f,-phi,
        .0f, phi,-.5f, -.5f, .0f,-phi,  -phi, .5f, .0f,
        .0f, phi,-.5f,  .5f, .0f,-phi,  -.5f, .0f,-phi,
        /////////////////////////////////////
        phi, .5f, .0f,  .5f, .0f, phi,   phi,-.5f, .0f,
        phi, .5f, .0f,  phi,-.5f, .0f,   .5f, .0f,-phi,

       -phi, .5f, .0f, -phi,-.5f, .0f,  -.5f, .0f, phi,
       -phi, .5f, .0f, -.5f, .0f,-phi,  -phi,-.5f, .0f,
        /////////////////////////////////////
        .0f,-phi, .5f,  phi,-.5f, .0f,  .5f, .0f, phi,
        .0f,-phi, .5f, -.5f, .0f, phi,  -phi,-.5f, .0f,
        .0f,-phi, .5f,  .5f, .0f, phi,  -.5f, .0f, phi,

        .0f,-phi,-.5f,  .5f, .0f,-phi,   phi,-.5f, .0f,
        .0f,-phi,-.5f, -phi,-.5f, .0f,  -.5f, .0f,-phi,
        .0f,-phi,-.5f,  -.5f, .0f,-phi,  .5f, .0f,-phi,

        .0f,-phi, .5f,  .0f,-phi,-.5f,   phi,-.5f, .0f,
        .0f,-phi, .5f, -phi,-.5f, .0f,   .0f,-phi,-.5f
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // normal array
    GLfloat piraNormals[180] = {  // 72 elements
        // If normalized, (0, 1, 2) -> (0/r(5), 1/r(5), 2/r(5)) 
        0, 1, 2,    0, 1, 2,    0, 1, 2,  
        2, 1, 0,    2, 1, 0,    2, 1, 0,  
        
        0, 1, -2,   0, 1, -2,   0, 1, -2,  
        -2, 1, 0,   -2, 1, 0,   -2, 1, 0,  
        0, -1, 0,   0, -1, 0,   0, -1, 0,

        0, 1, -2,   0, 1, -2,   0, 1, -2,
        -2, 1, 0,   -2, 1, 0,   -2, 1, 0,
        0, -1, 0,   0, -1, 0,   0, -1, 0,
        ////////////
        0, 1, -2,   0, 1, -2,   0, 1, -2,
        -2, 1, 0,   -2, 1, 0,   -2, 1, 0,

        0, -1, 0,   0, -1, 0,   0, -1, 0,
        0, 1, -2,   0, 1, -2,   0, 1, -2,
        ////////////
        -2, 1, 0,   -2, 1, 0,   -2, 1, 0,
        0, 1, -2,   0, 1, -2,   0, 1, -2,
        -2, 1, 0,   -2, 1, 0,   -2, 1, 0,

        0, 1, -2,   0, 1, -2,   0, 1, -2,
        -2, 1, 0,   -2, 1, 0,   -2, 1, 0,
        0, -1, 0,   0, -1, 0,   0, -1, 0,

        0, -1, 0,   0, -1, 0,   0, -1, 0,
        0, -1, 0,   0, -1, 0,   0, -1, 0
    };
    GLfloat smooth_piraNormals[180] = {  // 72 elements
        0, 1, 2,    0, 1, 2,    0, 1, 2,
        2, 1, 0,    2, 1, 0,    2, 1, 0,

        0, 1, -2,   0, 1, -2,   0, 1, -2,
        -2, 1, 0,   -2, 1, 0,   -2, 1, 0,
        0, -1, 0,   0, -1, 0,   0, -1, 0,

        0, 1, -2,   0, 1, -2,   0, 1, -2,
        -2, 1, 0,   -2, 1, 0,   -2, 1, 0,
        0, -1, 0,   0, -1, 0,   0, -1, 0,
        ////////////
        0, 1, -2,   0, 1, -2,   0, 1, -2,
        -2, 1, 0,   -2, 1, 0,   -2, 1, 0,

        0, -1, 0,   0, -1, 0,   0, -1, 0,
        0, 1, -2,   0, 1, -2,   0, 1, -2,
        ////////////
        -2, 1, 0,   -2, 1, 0,   -2, 1, 0,
        0, 1, -2,   0, 1, -2,   0, 1, -2,
        -2, 1, 0,   -2, 1, 0,   -2, 1, 0,

        0, 1, -2,   0, 1, -2,   0, 1, -2,
        -2, 1, 0,   -2, 1, 0,   -2, 1, 0,
        0, -1, 0,   0, -1, 0,   0, -1, 0,

        0, -1, 0,   0, -1, 0,   0, -1, 0,
        0, -1, 0,   0, -1, 0,   0, -1, 0
    };
    
    // colour array
    GLfloat piraColors[240] = { // initialized as RGBA sollid color for each face, 96 elements
        1, 1, 1, 1,   1, 1, 1, 1,  .5,.5,.5, 1,
        1, 1, 1, 1,   1, 1, 1, 1,  .5,.5,.5, 1,
        
        1, 1, 1, 1,   0, 0, 0, 1,  .5,.5,.5, 1,
        1, 1, 1, 1,  .5,.5,.5, 1,   0, 0, 0, 1,
        1, 1, 1, 1,   0, 0, 0, 1,   0, 0, 0, 1,
        
        1, 1, 1, 1,  .5,.5,.5, 1,   0, 0, 0, 1,
        1, 1, 1, 1,   0, 0, 0, 1,  .5,.5,.5, 1,
        1, 1, 1, 1,   0, 0, 0, 1,   0, 0, 0, 1,
        ///////////
       .5,.5,.5, 1,   0, 0, 0, 1,  .5,.5,.5, 1,
       .5,.5,.5, 1,  .5,.5,.5, 1,   0, 0, 0, 1,
        
       .5,.5,.5, 1,  .5,.5,.5, 1,   0, 0, 0, 1,
       .5,.5,.5, 1,   0, 0, 0, 1,  .5,.5,.5, 1,
        /////////////
        1, 1, 1, 1,  .5,.5,.5, 1,   0, 0, 0, 1,
        1, 1, 1, 1,   0, 0, 0, 1,  .5,.5,.5, 1,
        1, 1, 1, 1,   0, 0, 0, 1,   0, 0, 0, 1,

        1, 1, 1, 1,   0, 0, 0, 1,  .5,.5,.5, 1,
        1, 1, 1, 1,  .5,.5,.5, 1,   0, 0, 0, 1,
        1, 1, 1, 1,   0, 0, 0, 1,   0, 0, 0, 1,

        1, 1, 1, 1,   1, 1, 1, 1,  .5,.5,.5, 1,
        1, 1, 1, 1,  .5,.5,.5, 1,   1, 1, 1, 1
    };
    
    // texture coord array
    ////////////

    GLfloat piraTexCoords[120] = { // 48 elements
        0.5000, 0.5000,     0.5000, 0.5000,     0.5000, 0.5000,
        0.5000, 0.5000,     0.5000, 0.5000,     0.5000, 0.5000,

        0.5000, 0.5000,     0.5000, 0.5000,     0.5000, 0.5000,
        0.5000, 0.5000,     0.5000, 0.5000,     0.5000, 0.5000,
        0.5000, 0.5000,     0.5000, 0.5000,     0.5000, 0.5000,

        0.5000, 0.5000,     0.5000, 0.5000,     0.5000, 0.5000,
        0.5000, 0.5000,     0.5000, 0.5000,     0.5000, 0.5000,
        0.5000, 0.5000,     0.5000, 0.5000,     0.5000, 0.5000,
        ////////////////
        0.5000, 0.5000,     0.5000, 0.5000,     0.5000, 0.5000,
        0.5000, 0.5000,     0.5000, 0.5000,     0.5000, 0.5000,

        0.5000, 0.5000,     0.5000, 0.5000,     0.5000, 0.5000,
        0.5000, 0.5000,     0.5000, 0.5000,     0.5000, 0.5000,
        ////////////////
        0.5000, 0.5000,     0.5000, 0.5000,     0.5000, 0.5000,
        0.5000, 0.5000,     0.5000, 0.5000,     0.5000, 0.5000,
        0.5000, 0.5000,     0.5000, 0.5000,     0.5000, 0.5000,

        0.5000, 0.5000,     0.5000, 0.5000,     0.5000, 0.5000,
        0.5000, 0.5000,     0.5000, 0.5000,     0.5000, 0.5000,
        0.5000, 0.5000,     0.5000, 0.5000,     0.5000, 0.5000,

        0.5000, 0.5000,     0.5000, 0.5000,     0.5000, 0.5000,
        0.5000, 0.5000,     0.5000, 0.5000,     0.5000, 0.5000
    };
    
    // index array for glDrawElements()
    ///////////// A cube requires 36 indices = 6 sides * 2 tris * 3 verts
    
    GLuint piraIndices[60] = { // 36 elements
        0,   1,  2,                 // (front)
        3,   4,  5,  

        6,   7,  8,                
        9,  10, 11,                 
        12, 13, 14,

        15, 16, 17,
        18, 19, 20,
        21, 22, 23,
        //////////
        24, 25, 26,
        27, 28, 29,

        30, 31, 32,
        33, 34, 35,
        //////////
        36, 37, 38,
        39, 40, 41,
        42, 43, 44,

        45, 46, 47,
        48, 49, 50,
        51, 52, 53,

        54, 55, 56,
        57, 58, 59
    };

    unsigned int VAO[2];
    unsigned int VBO[2];
    unsigned int EBO;
    
    unsigned int vSize = sizeof(fVertices);
    unsigned int nSize = sizeof(piraNormals);
    unsigned int cSize = sizeof(piraColors);
    unsigned int tSize = sizeof(piraTexCoords);
    
    FootBall() {
        initBuffers();
    };
    
    FootBall(float sx, float sy, float sz) {
        scale2(sx, sy, sz);
        initBuffers();
    };
    
    FootBall(float dx, float dy, float dz, float s) {
        scale(s);
        translate(dx, dy, dz);
        initBuffers();
    };
    
    FootBall(float dx, float dy, float dz, float sx, float sy, float sz) {
        scale2(sx, sy, sz);
        translate(dx, dy, dz);
        initBuffers();
    };
    
    void initBuffers() {
        glGenVertexArrays(2, VAO);
        glGenBuffers(2, VBO);
        glGenBuffers(1, &EBO);
        

        // 1. Flat
        glBindVertexArray(VAO[0]);
        
        // copy vertex attrib data to VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, vSize+cSize+nSize+tSize, 0, GL_STATIC_DRAW); // reserve space                         
        glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, fVertices);                  // copy verts at offset 0
        glBufferSubData(GL_ARRAY_BUFFER, vSize, nSize, piraNormals);               // copy norms after verts
        glBufferSubData(GL_ARRAY_BUFFER, vSize+nSize, cSize, piraColors);          // copy cols after norms           
        glBufferSubData(GL_ARRAY_BUFFER, vSize+nSize+cSize, tSize, piraTexCoords); // copy texs after cols
        
        // copy index data to EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(piraIndices), piraIndices, GL_STATIC_DRAW);
        
        // attribute position initialization
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  // position attrib
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)vSize); // normal attrib
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(vSize+nSize)); //color attrib     
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(vSize+cSize+nSize)); // tex coord 
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);


        // 2. Smooth
        glBindVertexArray(VAO[1]);

        // copy vertex attrib data to VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, vSize + cSize + nSize + tSize, 0, GL_STATIC_DRAW); // reserve space                         
        glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, fVertices);                  // copy verts at offset 0
        glBufferSubData(GL_ARRAY_BUFFER, vSize, nSize, smooth_piraNormals);               // copy norms after verts
        glBufferSubData(GL_ARRAY_BUFFER, vSize + nSize, cSize, piraColors);          // copy cols after norms           
        glBufferSubData(GL_ARRAY_BUFFER, vSize + nSize + cSize, tSize, piraTexCoords); // copy texs after cols

        // copy index data to EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(piraIndices), piraIndices, GL_STATIC_DRAW);

        // attribute position initialization
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  // position attrib
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)vSize); // normal attrib
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(vSize + nSize)); //color attrib     
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(vSize + cSize + nSize)); // tex coord 
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(1);
    };
    
    void draw(Shader *shader, bool fsChange) {
        shader->use();
        if (fsChange)
            glBindVertexArray(VAO[0]);
        else
            glBindVertexArray(VAO[1]);
        glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    };
    
    void translate(float dx, float dy, float dz) {
        for (int i = 0; i < 180; i++) {
            if (i % 3 == 0) fVertices[i] += dx;
            else if (i % 3 == 1) fVertices[i] += dy;
            else if (i % 3 == 2) fVertices[i] += dz;
        }
    };
    
    void scale(float s) {
        for (int i = 0; i < 180; i++)
            fVertices[i] *= s;
    };
    
    void scale2(float sx, float sy, float sz) {
        for (int i = 0; i < 180; i++) {
            if (i % 3 == 0) fVertices[i] *= sx;
            else if (i % 3 == 1) fVertices[i] *= sy;
            else if (i % 3 == 2) fVertices[i] *= sz;
        }
    }
};


#endif
