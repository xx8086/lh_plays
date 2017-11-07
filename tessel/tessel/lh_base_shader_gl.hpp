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
#include "lh_generate_tri.hpp"

class CLhShaderGL:public CGL
{
public:
    CLhShaderGL();
    ~CLhShaderGL();
public:
    int run(int args, char *argv[]);
private:
    void processInput(GLFWwindow *window);
    bool loadshader();
    bool Init();
    void link();
    bool loopmain();
    void release();
    void setcamera(unsigned int, float, float, float);
    void draw();
    void linkshader(unsigned int&, unsigned int&, unsigned int&, const char*);
    
private:
    CGenerateTri _generate_tri;
    
private:
    unsigned int SCR_WIDTH = 800;
    unsigned int SCR_HEIGHT = 600;
    
    unsigned int _vbos[3];
    unsigned int _vaos[3];
    GLFWwindow* window;
    
    unsigned int vertex_shader;
    unsigned int fragment_shader_front;
    unsigned int fragment_shader_back;
    unsigned int fragment_shader_side;
    unsigned int shader_program_front;
    unsigned int shader_program_back;
    unsigned int shader_program_side;
    
    float scalae = 0.01f;
};


#endif /* lh_base_shader_gl_hpp */
