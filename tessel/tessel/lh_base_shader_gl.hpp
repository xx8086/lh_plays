//
//  lh_base_shader_gl.hpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/26.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#ifndef lh_base_shader_gl_hpp
#define lh_base_shader_gl_hpp

#include "glbase.hpp"
#include "GLFW/glfw3.h"

class CLhShaderGL:public CGL
{
public:
    CLhShaderGL();
    ~CLhShaderGL();
public:
    int run(int args, char *argv[]);
private:
    void processInput(GLFWwindow *window);
    bool LoadDatas();
    bool Init();
    bool LoopMain();
    
private:
    unsigned int SCR_WIDTH = 800;
    unsigned int SCR_HEIGHT = 600;
    
    float firstTriangle[9] = {
        -0.9f, -0.5f, 0.0f,  // left
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top
    };
    float secondTriangle[9] = {
        0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top
    };
    unsigned int VBOs[2];
    unsigned int VAOs[2];
    GLFWwindow* window;
    
    unsigned int vertexShader;
    unsigned int fragmentShaderOrange; // the first fragment shader that outputs the color orange
    unsigned int fragmentShaderYellow; // the second fragment shader that outputs the color yellow
    unsigned int shaderProgramOrange;
    unsigned int shaderProgramYellow; // the second shader program
};


#endif /* lh_base_shader_gl_hpp */
