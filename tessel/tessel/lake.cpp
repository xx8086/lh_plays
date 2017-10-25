//
//  lake.cpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/24.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#include "lake.hpp"


//外围轮廓线

GLdouble CLake::vCoast[COAST_POINTS][3] = {{-70.0, 30.0, 0.0 },
    {-50.0, 30.0, 0.0 },
    {-50.0, 27.0, 0.0 },
    { -5.0, 27.0, 0.0 },
    {  0.0, 20.0, 0.0 },
    {  8.0, 10.0, 0.0 },
    { 12.0,  5.0, 0.0 },
    { 10.0,  0.0, 0.0 },
    { 15.0,-10.0, 0.0 },
    { 20.0,-20.0, 0.0 },
    { 20.0,-35.0, 0.0 },
    { 10.0,-40.0, 0.0 },
    {  0.0,-30.0, 0.0 },
    { -5.0,-20.0, 0.0 },
    {-12.0,-10.0, 0.0 },
    {-13.0, -5.0, 0.0 },
    {-12.0,  5.0, 0.0 },
    {-20.0, 10.0, 0.0 },
    {-30.0, 20.0, 0.0 },
    {-40.0, 15.0, 0.0 },
    {-50.0, 15.0, 0.0 },
    {-55.0, 20.0, 0.0 },
    {-60.0, 25.0, 0.0 },
    {-70.0, 25.0, 0.0 }};

//湖的轮廓线
GLdouble CLake::vLake[LAKE_POINTS][3] = {{ 10.0, -20.0, 0.0 },
    { 15.0, -25.0, 0.0 },
    { 10.0, -30.0, 0.0 },
    {  5.0, -25.0, 0.0 }};

int CLake::iMode = LINE_LOOP;

CLake::CLake()
{}

CLake::~CLake()
{}


void CLake::SetupRC()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void CLake::tessError(GLenum code)
{
    const char *str = (const char*)gluErrorString(code);
    glutSetWindowTitle(str);
}

void CLake::RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3f(1.0f, 0.0f, 1.0f);
    glPushMatrix();
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    switch(iMode)
    {
        case LINE_LOOP:
        {
            glBegin(GL_LINE_LOOP);
            for (int i = 0; i < COAST_POINTS; ++i)
            {
                glVertex3dv(vCoast[i]);
            }
            glEnd();
        }
            break;
        case TESS:
        {
            //创建镶嵌器对象
            GLUtesselator *pTess = gluNewTess();
            //设置回调函数
            gluTessCallback(pTess, GLU_TESS_BEGIN, (void (CALLBACK *)())glBegin);
            gluTessCallback(pTess, GLU_TESS_END, (void (CALLBACK *)())glEnd);
            gluTessCallback(pTess, GLU_TESS_VERTEX, (void (CALLBACK *)())glVertex3dv);
            gluTessCallback(pTess, GLU_TESS_ERROR, (void (CALLBACK *)())tessError);
            
            //开始一个多边形
            gluTessBeginPolygon(pTess, NULL);
            //开始一个轮廓
            gluTessBeginContour(pTess);
            //设置轮廓的顶点
            for (int i = 0; i < COAST_POINTS; ++i)
            {
                gluTessVertex(pTess, vCoast[i], vCoast[i]);
            }
            gluTessEndContour(pTess);
            gluTessEndPolygon(pTess);
            gluDeleteTess(pTess);
        }
            break;
        case COMPLEX:
        {
            GLUtesselator *pTess = gluNewTess();
            gluTessCallback(pTess, GLU_TESS_BEGIN, (void (CALLBACK *)())glBegin);
            gluTessCallback(pTess, GLU_TESS_END, (void (CALLBACK *)())glEnd);
            gluTessCallback(pTess, GLU_TESS_VERTEX, (void (CALLBACK *)())glVertex3dv);
            gluTessCallback(pTess, GLU_TESS_ERROR, (void (CALLBACK *)())tessError);
            
            //指定奇数的轮廓为填充，偶数的轮廓是镂空的。这也是默认的设置
            //gluTessProperty(pTess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);
            //gluTessProperty(pTess, GLU_TESS_TOLERANCE, 0);
            
            gluTessBeginPolygon(pTess, NULL);
            
            gluTessBeginContour(pTess);
            for (int i = 0; i < COAST_POINTS; ++i)
            {
                gluTessVertex(pTess, vCoast[i], vCoast[i]);
            }
            gluTessEndContour(pTess);
            
            gluTessBeginContour(pTess);
            for (int i = 0; i < LAKE_POINTS; ++i)
            {
                gluTessVertex(pTess, vLake[i], vLake[i]);
            }
            gluTessEndContour(pTess);
            gluTessEndPolygon(pTess);
            gluDeleteTess(pTess);
        }
            break;
        default:
            break;
    }
    
    glPopMatrix();
    
    glutSwapBuffers();
}


void CLake::ChangeSize(GLsizei w, GLsizei h)
{
    if (h == 0)
    {
        h = 1;
    }
    
    glViewport(0, 0, w, h);
    
    GLfloat aspect = (GLfloat)w/(GLfloat)h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluOrtho2D(-100.0, 100.0, -100.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void CLake::ProcessMenu(int value)
{
    iMode = value;
    glutPostRedisplay();
}

int CLake::run(int args, char *argv[])
{
    glutInit(&args, argv);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("florida");
    
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("LINE_LOOP", LINE_LOOP);
    glutAddMenuEntry("Tess", TESS);
    glutAddMenuEntry("Complex", COMPLEX);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    iMode = COMPLEX;
    GL_POLYGON;
    SetupRC();
    glutMainLoop();
    return 0;
}
