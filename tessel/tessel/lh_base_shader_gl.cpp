//
//  lh_base_shader_gl.cpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/26.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//
#include "glad.hpp"
#include <iostream>

#include <fstream>
#include <sstream>


#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#include "lh_base_shader_gl.hpp"

const char *vertexshader = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char *frontfv = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n\0";

const char *backfv = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";

const char *sidefv = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
"}\n\0";


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

CLhShaderGL::CLhShaderGL()
{}

CLhShaderGL::~CLhShaderGL()
{}


void CLhShaderGL::insertpoint(vector<float>& triangles, float x, float y, float z)
{
    triangles.push_back(x);
    triangles.push_back(y);
    triangles.push_back(z);
}

void CLhShaderGL::release()
{
    // Cleanup
    for(vector<CDT*>::iterator iter = vec_cdt.begin();
        iter != vec_cdt.end();
        iter++){
        delete *iter;
    }
    vec_cdt.clear();
    
    // Free points
    for(int i = 0; i < polylines.size(); i++) {
        vector<Point*> poly = polylines[i];
        FreeClear(poly);
    }
    
    glDeleteVertexArrays(3, VAOs);
    glDeleteBuffers(3, VBOs);
    glfwTerminate();
}

bool CLhShaderGL::Init()
{
    bool rt = true;
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
    
    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    };
    
    return true;
}

bool CLhShaderGL::loopmain()
{
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    return true;
}

void CLhShaderGL::linkshader(unsigned int& vs, unsigned int& fs, unsigned int& program, const char* strshader)
{
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    program = glCreateProgram();
    
    glShaderSource(fs, 1, &strshader, NULL);
    glCompileShader(fs);
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
}

void CLhShaderGL::link()
{
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexshader, NULL);
    glCompileShader(vertexShader);
    linkshader(vertexShader, fragmentShaderFront, shaderProgramFront, frontfv);
    linkshader(vertexShader, fragmentShaderBack, shaderProgramBack, backfv);
}

bool CLhShaderGL::loadshader()
{
    link();
    
    create_vertex_buffer(deep);
    //create_vertex_buffer_loacl();
    bind_vertex_buffer();
    
    glEnableVertexAttribArray(0);
    glBindVertexArray(0); // no need to unbind at all as we directly bind a different VAO the next few lines
    return true;
}

void CLhShaderGL::create_vertex_buffer(float deep)
{
    scalae = 0.01f;
    triangle_points.clear();
    for(vector< vector<Triangle*> >::iterator iter = triangles.begin();
        iter != triangles.end();
        iter++){
        int num = iter->size();
        for (int i = 0; i < num; i++) {
            Triangle& t = *(iter->at(i));
            Point& a = *t.GetPoint(0);
            Point& b = *t.GetPoint(1);
            Point& c = *t.GetPoint(2);
            insertpoint(triangle_points, a.x/64.0f, a.y/64.0f, 0.0f);
            insertpoint(triangle_points, b.x/64.0f, b.y/64.0f, 0.0f);
            insertpoint(triangle_points, c.x/64.0f, c.y/64.0f, 0.0f);
            
            insertpoint(back_triangle_points, a.x/64.0f, a.y/64.0f, -deep);
            insertpoint(back_triangle_points, b.x/64.0f, b.y/64.0f, -deep);
            insertpoint(back_triangle_points, c.x/64.0f, c.y/64.0f, -deep);
            //printf("(%.2f,%.2f),(%.2f,%.2f),(%.2f,%.2f)\n",a.x, a.y, b.x, b.y, c.x, c.y);
        }
    }
}

void CLhShaderGL::create_vertex_buffer_loacl()
{
    int iszie = 1026;
    triangle_points.clear();
    triangle_points.resize(iszie);
    memcpy(triangle_points.data(),
           arzi,
           triangle_points.size()*sizeof(float));
    
    
    back_triangle_points.clear();
    back_triangle_points.resize(iszie);
    memcpy(back_triangle_points.data(),
           arzi,
           back_triangle_points.size()*sizeof(float));
    for(int i = 0; i < iszie; )
    {
        back_triangle_points[i+2] = -deep;
        i += 3;
    }
}


void CLhShaderGL::bindshader(unsigned int& vao, unsigned int& vbo, unsigned int size, const char* datas)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 size,
                 datas, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
}

void CLhShaderGL::bind_vertex_buffer()
{
    glGenVertexArrays(3, VAOs);
    glGenBuffers(3, VBOs);
    
    bindshader(VAOs[0], VBOs[0], triangle_points.size()* sizeof(float), (const char*)triangle_points.data());
    bindshader(VAOs[1], VBOs[1], back_triangle_points.size()* sizeof(float), (const char*)back_triangle_points.data());
    
}

void CLhShaderGL::drawshader(unsigned int& fs, unsigned int& vao, unsigned int& vbo, unsigned int size)
{
    setcamera(fs);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, size);
}

void CLhShaderGL::draw()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    drawshader(shaderProgramFront, VAOs[0], VBOs[0], triangle_points.size());
    drawshader(shaderProgramBack, VAOs[1], VBOs[1], back_triangle_points.size());
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CLhShaderGL::loaddatas()
{
    vector<p2t::Point*> polyline;
    string line;
    ifstream myfile("/Users/baidu/lh_plays/tessel/a_pos.txt");
    if (myfile.is_open()) {
        while (!myfile.eof()) {
            getline(myfile, line);
            if (line.size() == 0) {
                if (polyline.size() > 0)
                {
                    polylines.push_back(polyline);
                    polyline.clear();
                }
                continue;
            }
            istringstream iss(line);
            vector<string> tokens;
            copy(istream_iterator<string>(iss), istream_iterator<string>(),
                 back_inserter<vector<string> >(tokens));
            double x = str2f(tokens[0]);
            double y = str2f(tokens[1]);
            polyline.push_back(new Point(x, y));
        }
        myfile.close();
    } else {
        cout << "File not opened" << endl;
    }
    
    if (polyline.size() > 0)
    {
        polylines.push_back(polyline);
        polyline.clear();
    }
    
    
    for(vector< vector<Point*> >::iterator iter = polylines.begin();
        iter != polylines.end();
        iter++){
        
        CDT* cdt = new CDT(*iter);
        //if(0) {
        // Add head hole
        //vector<Point*> head_hole = CreateHeadHole();
        //当前轮廓iter如果有洞head_hole，需要手动AddHole进来。
        //cdt->AddHole(head_hole.size);
        //polylines.push_back(head_hole);
        //}
        
        cdt->Triangulate();
        triangles.push_back(cdt->GetTriangles());
        vec_cdt.push_back(cdt);
    }
}

int CLhShaderGL::run(int args, char **argv)
{
    if (!Init()){
        return 1;
    }
    
    loaddatas();
    loadshader();
    loopmain();
    release();
    
    return 0;
}

float CLhShaderGL::str2f(const std::string& s)
{
    std::istringstream i(s);
    float x;
    if (!(i >> x))
        return 0;
    return x;
}

void CLhShaderGL::setcamera(unsigned int pid)
{
    glUseProgram(pid);
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
    projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
    model = glm::rotate(model, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scalae, scalae, scalae));
    glUniformMatrix4fv(glGetUniformLocation(pid, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(pid, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(pid, "model"), 1, GL_FALSE, glm::value_ptr(model));
    //shaderProgramOrange
    
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void CLhShaderGL::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

