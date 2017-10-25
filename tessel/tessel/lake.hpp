//
//  lake.hpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/24.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#ifndef lake_hpp
#define lake_hpp

#include <stdio.h>
#include <GLUT/glut.h>

class CLake
{
public:
    CLake();
    ~CLake();
    
public:
    int run(int args, char *argv[]);
    
private:
    void SetupRC();
    static void tessError(GLenum code);
    static void RenderScene();
    static void ChangeSize(GLsizei w, GLsizei h);
    static void ProcessMenu(int value);
private:
    
#define COAST_POINTS 24
#define LAKE_POINTS 4
    static  GLdouble vCoast[COAST_POINTS][3];
    static  GLdouble vLake[LAKE_POINTS][3];
    
#define LINE_LOOP 1
#define TESS 2
#define COMPLEX 3
    
    static int iMode;
};

#endif /* lake_hpp */
