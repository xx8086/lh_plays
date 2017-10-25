//
//  quad.cpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/24.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#include "quad.hpp"

int veccount = 0;
GLdouble CQuad::quad[12][3] = {
    {-2,3,0},   {-2,0,0},   {2,0,0},    { 2,3,0},
    {-1,2,0},   {-1,1,0},   {1,1,0},    { 1,2,0},
    {-0.5,1,0}, {-0.5,2,0}, {0.5,2,0},  { 0.5,1,0} };

CQuad::CQuad()
{}

CQuad::~CQuad()
{}

void myIdle(void)
{
    glutPostRedisplay();
}

//------------------------------------------------------------    OnDraw()
//
void CALLBACK CQuad::vertexCallback(GLvoid *vertex)
{
    const GLdouble *pointer;
    pointer = (GLdouble *) vertex;
    glColor3dv(pointer+3);//在此设置颜色
    glVertex3dv(pointer);
    printf("\t%d Vertex:(\t%.2f\t%.2f\t%.2f), Color:(\t%.2f\t%.2f\t%.2f)\n",
           veccount++,
           pointer[0], pointer[1], pointer[2],
           (pointer+3)[0],(pointer+3)[1], (pointer+3)[2]);
}

void CALLBACK CQuad::beginCallback(GLenum which){
    glBegin(which);
    printf("glBegin:%d\n", which);
    veccount = 0;
}

void CALLBACK CQuad::endCallback(){
    glEnd();
    printf("glEnd\n");
}

void CALLBACK CQuad::errorCallback(GLenum errorCode){
    const GLubyte *estring;
    estring = gluErrorString(errorCode);
    fprintf(stderr, "Tessellation Error: %s\n", estring);
    exit(0);
}
void CALLBACK CQuad::combineCallback(GLdouble coords[3],
                              GLdouble *vertex_data[4],
                              GLfloat weight[4], GLdouble **dataOut )
{
    GLdouble *vertex;
    int i;
    vertex = (GLdouble *) malloc(6 * sizeof(GLdouble));
    vertex[0] = coords[0];
    vertex[1] = coords[1];
    vertex[2] = coords[2];
    for (i = 3; i < 7; i++)
        vertex[i] = weight[0] * vertex_data[0][i]
        + weight[1] * vertex_data[1][i]
        + weight[2] * vertex_data[2][i]
        + weight[3] * vertex_data[3][i];
    *dataOut = vertex;
}

void CQuad::OnDraw() {
    
    // clear the screen & depth buffer
    glClear(GL_COLOR_BUFFER_BIT);
    
    // clear the previous transform
    glLoadIdentity();
    
    GLUtesselator* tobj = gluNewTess();
    if (!tobj) return;
    
    gluTessCallback(tobj, GLU_TESS_VERTEX, (void (CALLBACK *)())vertexCallback);
    gluTessCallback(tobj, GLU_TESS_BEGIN, (void (CALLBACK *)())beginCallback);
    gluTessCallback(tobj, GLU_TESS_END, (void (CALLBACK *)())endCallback);
    gluTessCallback(tobj, GLU_TESS_ERROR, (void (CALLBACK *)())errorCallback);
    gluTessCallback(tobj, GLU_TESS_COMBINE, (void (CALLBACK *)())combineCallback);
    
    //glShadeModel(GL_FLAT);
    //gluTessProperty(tobj,GLU_TESS_WINDING_RULE,GLU_TESS_WINDING_POSITIVE); //GLU_TESS_WINDING_ODD
    gluTessBeginPolygon(tobj,NULL);
    
    gluTessBeginContour(tobj);
    gluTessVertex(tobj, quad[0], quad[0]);
    gluTessVertex(tobj, quad[1], quad[1]);
    gluTessVertex(tobj, quad[2], quad[2]);
    gluTessVertex(tobj, quad[3], quad[3]);
    gluTessEndContour(tobj);
    
    gluTessBeginContour(tobj);                      // inner quad (hole)
    gluTessVertex(tobj, quad[4], quad[4]);
    gluTessVertex(tobj, quad[5], quad[5]);
    gluTessVertex(tobj, quad[6], quad[6]);
    gluTessVertex(tobj, quad[7], quad[7]);
    gluTessEndContour(tobj);
     
    gluTessBeginContour(tobj);                      // inner quad (hole)
    gluTessVertex(tobj, quad[8], quad[8]);
    gluTessVertex(tobj, quad[9], quad[9]);
    gluTessVertex(tobj, quad[10], quad[10]);
    gluTessVertex(tobj, quad[11], quad[11]);
    gluTessEndContour(tobj);
    
    
    gluTessEndPolygon(tobj);
    
    gluDeleteTess(tobj);
    glutSwapBuffers();
}

//------------------------------------------------------------    OnInit()
//
void CQuad::OnInit()
{
    //glClearColor(1,1,1,0);
}

//------------------------------------------------------------    OnExit()
//
void CQuad::OnExit() {
}


//------------------------------------------------------------    OnReshape()
//
void CQuad::OnReshape(int w, int h)
{
    // prevents division by zero when minimising window
    if (h==0)
        h=1;
    
    // set the drawable region of the window
    glViewport(0,0,w,h);
    
    // set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // just use a perspective projection
    //gluPerspective(45,(float)w/h,0.1,100);
    if(w<=h)
        glOrtho(-4.0,4.0,-4.0*(GLfloat)h/(GLfloat)w,4.0*(GLfloat)h/(GLfloat)w,0.0,100.0);
    else
        glOrtho(-4.0,4.0,-4.0*(GLfloat)h/(GLfloat)w,4.0*(GLfloat)h/(GLfloat)w,0.0,100.0);
    
    // go back to modelview matrix so we can move the objects about
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int CQuad::run(int args, char *argv[])
{
    // initialise glut
    glutInit(&args, argv);
    
    // request a depth buffer, RGBA display mode, and we want double buffering
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
    
    // set the initial window size
    glutInitWindowSize(480,480);
    
    // create the window
    glutCreateWindow("filling");
    
    // run our custom initialisation
    OnInit();
    
    // set the function to use to draw our scene
    glutDisplayFunc(OnDraw);
    
    // set the function to handle changes in screen size
    glutReshapeFunc(OnReshape);
    //glutIdleFunc(&myIdle);
    
    // set the function to be called when we exit
    atexit(OnExit);
    
    // this function runs a while loop to keep the program running.
    glutMainLoop();
    
    return 0;
}
