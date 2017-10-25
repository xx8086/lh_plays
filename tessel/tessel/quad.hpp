//
//  quad.hpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/24.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#ifndef quad_hpp
#define quad_hpp

#include "glbase.hpp"
class CQuad :public CGL
{
public:
    CQuad();
    ~CQuad();
public:
    int run(int args, char *argv[]);
private:
    static void OnDraw();
    static void OnInit();
    static void OnExit();
    static void OnReshape(int w, int h);
private:
    static void myIdle(void);
    static void CALLBACK vertexCallback(GLvoid *vertex);
    static void CALLBACK beginCallback(GLenum which);
    static void CALLBACK endCallback();
    static void CALLBACK errorCallback(GLenum errorCode);
    static void CALLBACK combineCallback(GLdouble coords[3],
                                  GLdouble *vertex_data[4],
                                  GLfloat weight[4], GLdouble **dataOut);
private:
    static GLdouble quad[12][3];
    
};

#endif /* quad_hpp */
