//
//  lh2tri.cpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/25.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#include "lh2tri.hpp"

#include <cstdlib>
#include <time.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <iostream>

void CLH2tri::Release()
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
}

int CLH2tri::run(int argc, char* argv[])
{
    int num_points = 0;
    double max, min;
    double zoom;
    
    zoom = 1;
    cx = 500;
    cy = 400;
    
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
            double x = StringToDouble(tokens[0]);
            double y = StringToDouble(tokens[1]);
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
    Init();
    
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
    
    MainLoop(zoom);
    
    Release();
    ShutDown(0);
    return 0;
}

void CLH2tri::Init()
{
    const int window_width = 800,
    window_height = 600;
    
    if (glfwInit() != GL_TRUE)
        ShutDown(1);
    // 800 x 600, 16 bit color, no depth, alpha or stencil buffers, windowed
    window = glfwCreateWindow(window_width, window_height, "Poly2Tri - C++", NULL, NULL);
    if (!window)
        ShutDown(1);
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void CLH2tri::ShutDown(int return_code)
{
    glfwTerminate();
    exit(return_code);
}

void CLH2tri::MainLoop(const double zoom)
{
    // the time of the previous frame
    double old_time = glfwGetTime();
    // this just loops as long as the program runs
    bool running = true;
    
    while (running) {
        glfwPollEvents();
        Draw(zoom);
        // swap back and front buffers
        glfwSwapBuffers(window);
    }
}

void CLH2tri::ResetZoom(double zoom, double cx, double cy, double width, double height)
{
    double left = -width / zoom;
    double right = width / zoom;
    double bottom = -height / zoom;
    double top = height / zoom;
    
    // Reset viewport
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Reset ortho view
    glOrtho(left, right, bottom, top, 1, -1);
    glTranslatef(-cx, -cy, 0);
    glScalef(scalae, scalae, scalae);
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_DEPTH_TEST);
    glLoadIdentity();
    
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
}

void CLH2tri::Draw(const double zoom)
{
    // reset zoom
    Point center = Point(cx, cy);
    ResetZoom(zoom, center.x, center.y, 800, 600);
    glColor3f(1, 0, 0);
    for(vector< vector<Triangle*> >::iterator iter = triangles.begin();
        iter != triangles.end();
        iter++){
        int num = iter->size();
        for (int i = 0; i < num; i++) {
            Triangle& t = *(iter->at(i));
            Point& a = *t.GetPoint(0);
            Point& b = *t.GetPoint(1);
            Point& c = *t.GetPoint(2);
            
            // Red
            glBegin(GL_TRIANGLES);
            glVertex2f(a.x, a.y);
            glVertex2f(b.x, b.y);
            glVertex2f(c.x, c.y);
            glEnd();
        }
    }
    
    
    // green
    glColor3f(0, 1, 0);
    for(int i = 0; i < polylines.size(); i++) {
        vector<Point*> poly = polylines[i];
        glBegin(GL_LINE_LOOP);
        for(int j = 0; j < poly.size(); j++) {
            glVertex2f(poly[j]->x, poly[j]->y);
        }
        glEnd();
    }
    
}

double CLH2tri::StringToDouble(const std::string& s)
{
    std::istringstream i(s);
    double x;
    if (!(i >> x))
        return 0;
    return x;
}

double CLH2tri::Fun(double x)
{
    return 2.5 + sin(10 * x) / x;
}

double CLH2tri::Random(double (*fun)(double), double xmin = 0, double xmax = 1)
{
    static double (*Fun)(double) = NULL, YMin, YMax;
    static bool First = true;
    
    // Initialises random generator for first call
    if (First)
    {
        First = false;
        srand((unsigned) time(NULL));
    }
    
    // Evaluates maximum of function
    if (fun != Fun)
    {
        Fun = fun;
        YMin = 0, YMax = Fun(xmin);
        for (int iX = 1; iX < RAND_MAX; iX++)
        {
            double X = xmin + (xmax - xmin) * iX / RAND_MAX;
            double Y = Fun(X);
            YMax = Y > YMax ? Y : YMax;
        }
    }
    
    // Gets random values for X & Y
    double X = xmin + (xmax - xmin) * rand() / RAND_MAX;
    double Y = YMin + (YMax - YMin) * rand() / RAND_MAX;
    
    // Returns if valid and try again if not valid
    return Y < fun(X) ? X : Random(Fun, xmin, xmax);
}


CLH2tri::CLH2tri()
{}

CLH2tri::~CLH2tri()
{}




