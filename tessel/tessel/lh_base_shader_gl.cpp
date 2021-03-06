//
//  lh_base_shader_gl.cpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/26.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//
#include "glad.hpp"
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

const char *fontfv = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec3 textColor;"
"void main()\n"
"{\n"
"   FragColor = vec4(textColor, 1.0f);\n"
"}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

CLhShaderGL::CLhShaderGL()
{}

CLhShaderGL::~CLhShaderGL()
{}

void CLhShaderGL::release()
{
    glDeleteVertexArrays(3, _vaos);
    glDeleteBuffers(3, _vbos);
    glfwTerminate();
}

bool CLhShaderGL::Init()
{
    bool rt = true;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
    
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return false;
    };
    
    return true;
}

bool CLhShaderGL::loopmain()
{
    while (!glfwWindowShouldClose(window))
    {
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
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertexshader, NULL);
    glCompileShader(vertex_shader);
    linkshader(vertex_shader, fragment_shader_front, shader_program_front, fontfv);
    linkshader(vertex_shader, fragment_shader_back, shader_program_back, fontfv);
    linkshader(vertex_shader, fragment_shader_side, shader_program_side, fontfv);
}

bool CLhShaderGL::loadshader()
{
    link();
    
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    return true;
}

void CLhShaderGL::draw()
{
    glUseProgram(0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#if 1
    glUseProgram(shader_program_front);
    setcamera(shader_program_front, 0.7, 0.0, 0.0);
    _generate_tri.font_rend(CHARACTE_FRONT);
    glUseProgram(0);
#endif
#if 1
    glUseProgram(shader_program_back);
    setcamera(shader_program_back, 0.0, 0.0, 0.7);
    _generate_tri.font_rend(CHARACTE_BACK);
    glUseProgram(0);
#endif
#if 1
    glUseProgram(shader_program_side);
    setcamera(shader_program_side, 0.0, 0.7, 0.0);
    _generate_tri.font_rend(CHARACTE_SIDE);
    glUseProgram(0);
#endif
}

int CLhShaderGL::run(int args, char **argv)
{
    if (!Init()){
        return 1;
    }
    
    _generate_tri.set_fontfile("/Users/baidu/Microsoft_Yahei.ttf");
    _generate_tri.set_depth(400.0);
    _generate_tri.set_outset(1.0, 1.0);
    scalae *= _generate_tri.get_sizeface_scalae(100.0);
    
    _generate_tri.load_freetype();
    std::wstring text(L"1234567我麟耐啊ABCDEF");
    _generate_tri.insert_words((wchar_t*)text.c_str(), text.size());
    loadshader();
    loopmain();
    release();
    
    return 0;
}

void CLhShaderGL::setcamera(unsigned int pid, float r, float g, float b)
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
    model = glm::translate(model, glm::vec3(-4.0f, 0.0f, -6.0f));
    model = glm::rotate(model, -(3.14f*30.0f/180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scalae, scalae, scalae));
    glUniformMatrix4fv(glGetUniformLocation(pid, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(pid, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(pid, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(glGetUniformLocation(pid, "textColor"), 1, glm::value_ptr(glm::vec3(r, g, b)));
}

void CLhShaderGL::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

