
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
    

#ifndef QUADPYRA_H
#define QUADPYRA_H

#include "shader.h"

class QuadPyra {
public:
    
    // vertex position array
    GLfloat pyraVertices[48]  = { // initialized as size = 1 for each dimension, 72 elements
        .0f, .5f, .0f, -.5f,-.5f, .5f,   .5f,-.5f, .5f,                     // a, b,c (front)
        .0f, .5f, .0f,  .5f,-.5f, .5f,   .5f,-.5f,-.5f,                     // a, c,d (right) 
        .0f, .5f, .0f,  .5f,-.5f,-.5f,  -.5f,-.5f,-.5f,                     // a, d,e (back)
        .0f, .5f, .0f,  -.5f,-.5f,-.5f, -.5f,-.5f, .5f,                     // a, e,b (left)
        -.5f,-.5f, .5f,  .5f,-.5f, .5f,  .5f,-.5f,-.5f, -.5f,-.5f,-.5f      // b,c,d,e (bottom)
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // normal array
    GLfloat piraNormals[48] = {  // 72 elements
        // If normalized, (0, 1, 2) -> (0/r(5), 1/r(5), 2/r(5)) 
        0, 1, 2,    0, 1, 2,    0, 1, 2,                // (front)
        2, 1, 0,    2, 1, 0,    2, 1, 0,                // (right)
        0, 1, -2,   0, 1, -2,   0, 1, -2,               // (back)
        -2, 1, 0,   -2, 1, 0,   -2, 1, 0,               // (left)
        0, -1, 0,   0, -1, 0,   0, -1, 0,   0, -1, 0    // (bottom)
        
    };
    GLfloat smooth_piraNormals[48] = {  // 72 elements
        // a ( 0, 1, 0 )
        // b ( (0-(2/r(5))+0)/3 , (-1+(1/r(5))+(1/r(5)))/3 , (0+0+(2/r(5)))/3 ) = ( -0.2981, -0.0352,  0.2981 )
        // c ( (0+0+(2/r(5)))/3 , (-1+(1/r(5))+(1/r(5)))/3 , (0+(2/r(5))+0)/3 ) = ( 0.2981,  -0.0352,  0.2981 )
        // d ( (0+(2/r(5))+0)/3 , (-1+(1/r(5))+(1/r(5)))/3 , (0+0-(2/r(5)))/3 ) = ( 0.2981,  -0.0352, -0.2981 )
        // e ( (0+0-(2/r(5)))/3 , (-1+(1/r(5))+(1/r(5)))/3 , (0-(2/r(5))+0)/3 ) = ( -0.2981, -0.0352, -0.2981 )
        0, 1, 0,    -0.2981, -0.0352,  0.2981,     0.2981, -0.0352,  0.2981,                                                // a, b,c (front)
        0, 1, 0,     0.2981, -0.0352,  0.2981,     0.2981, -0.0352, -0.2981,                                                // a, c,d (right) 
        0, 1, 0,     0.2981, -0.0352, -0.2981,    -0.2981, -0.0352, -0.2981,                                                // a, d,e (back)
        0, 1, 0,    -0.2981, -0.0352, -0.2981,    -0.2981, -0.0352,  0.2981,                                                // a, e,b (left)
        -0.2981, -0.0352,  0.2981,   0.2981,  -0.0352,  0.2981,   0.2981,  -0.0352, -0.2981,   -0.2981, -0.0352, -0.2981    // b,c,d,e (bottom)

    };
    
    // colour array
    GLfloat piraColors[64] = { // initialized as RGBA sollid color for each face, 96 elements
        1, 0, 0, 1,   1, 0, 0, 1,   1, 0, 0, 1,                 // red      (front)
        0, 1, 0, 1,   0, 1, 0, 1,   0, 1, 0, 1,                 // green    (right)
        0, 0, 1, 1,   0, 0, 1, 1,   0, 0, 1, 1,                 // blue     (back)
        1, 1, 0, 1,   1, 1, 0, 1,   1, 1, 0, 1,                 // yellow   (left)
        1, 0, 1, 1,   1, 0, 1, 1,   1, 0, 1, 1,   1, 0, 1, 1    // magenta  (bottom)
    };
    
    // texture coord array
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GLfloat piraTexCoords[32] = { // 48 elements
        0.5005, 0.2150,     0.3828,0.3828,      0.6181,0.3828,      // a, b,c (front)       519/1037=0.5005, 223/1037=0.2150
        0.7859, 0.5005,     0.6181,0.3828,      0.6181,0.6181,      // a, c,d (right)       815/1037=0.7859
        0.5005, 0.7859,     0.6181,0.6181,      0.3828,0.6181,      // a, d,e (back)        
        0.2150, 0.5005,     0.3828,0.6181,      0.3828,0.3828,      // a, e,b (left)
        0.3828, 0.3828,     0.6181,0.3828,      0.6181,0.6181,      0.3828,0.6181           // b,c,d,e (bottom)    397/1037=0.3828,     641/1037=0.6181 
   
    };
    
    // index array for glDrawElements()
    ///////////// A cube requires 36 indices = 6 sides * 2 tris * 3 verts
    
    GLuint piraIndices[18] = { // 36 elements
        0,   1,  2,                 // (front)
        3,   4,  5,                 // (right)
        6,   7,  8,                 // (back)
        9,  10, 11,                 // (left)
        12, 13, 14,  14, 15, 12     // (bottom-square)
    };

    unsigned int VAO[2];
    unsigned int VBO[2];
    unsigned int EBO;
    
    unsigned int vSize = sizeof(pyraVertices);
    unsigned int nSize = sizeof(piraNormals);
    unsigned int cSize = sizeof(piraColors);
    unsigned int tSize = sizeof(piraTexCoords);
    
    QuadPyra() {
        initBuffers();
    };
    
    QuadPyra(float sx, float sy, float sz) {
        scale2(sx, sy, sz);
        initBuffers();
    };
    
    QuadPyra(float dx, float dy, float dz, float s) {
        scale(s);
        translate(dx, dy, dz);
        initBuffers();
    };
    
    QuadPyra(float dx, float dy, float dz, float sx, float sy, float sz) {
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
        glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, pyraVertices);                  // copy verts at offset 0
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
        glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, pyraVertices);                  // copy verts at offset 0
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
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    };
    
    void translate(float dx, float dy, float dz) {
        for (int i = 0; i < 48; i++) {
            if (i % 3 == 0) pyraVertices[i] += dx;
            else if (i % 3 == 1) pyraVertices[i] += dy;
            else if (i % 3 == 2) pyraVertices[i] += dz;
        }
    };
    
    void scale(float s) {
        for (int i = 0; i < 48; i++)
            pyraVertices[i] *= s;
    };
    
    void scale2(float sx, float sy, float sz) {
        for (int i = 0; i < 48; i++) {
            if (i % 3 == 0) pyraVertices[i] *= sx;
            else if (i % 3 == 1) pyraVertices[i] *= sy;
            else if (i % 3 == 2) pyraVertices[i] *= sz;
        }
    }
};


#endif
