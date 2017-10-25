//
//  lh2tri.cpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/25.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#include "lh2tri.hpp"

#include <cstdlib>
#include "glfw/glfw3.h"
#include <time.h>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <iostream>
using namespace std;

using namespace p2t;


#define COAST_POINTS 24
#define LAKE_POINTS 4
//外围轮廓线
GLdouble vCoast[COAST_POINTS][3] = {{-70.0, 30.0, 0.0 },
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
GLdouble vLake[LAKE_POINTS][3] = {{ 10.0, -20.0, 0.0 },
    { 15.0, -25.0, 0.0 },
    { 10.0, -30.0, 0.0 },
    {  5.0, -25.0, 0.0 }};
void Init();
void ShutDown(int return_code);
void MainLoop(const double zoom);
void Draw(const double zoom);
void DrawMap(const double zoom);
void ConstrainedColor(bool constrain);
double StringToDouble(const std::string& s);
double Random(double (*fun)(double), double xmin, double xmax);
double Fun(double x);

/// Dude hole examples
vector<Point*> CreateHeadHole();
vector<Point*> CreateChestHole();

float rotate_y = 0,
rotate_z = 0;
const float rotations_per_tick = .2;

/// Screen center x
double cx = 0.0;
/// Screen center y
double cy = 0.0;

/// Constrained triangles
vector<Triangle*> triangles;
/// Triangle map
list<Triangle*> map;
/// Polylines
vector< vector<Point*> > polylines;

/// Draw the entire triangle map?
bool draw_map = false;
/// Create a random distribution of points?
bool random_distribution = false;

GLFWwindow* window = NULL;

template <class C> void FreeClear( C & cntr ) {
    for ( typename C::iterator it = cntr.begin();
         it != cntr.end(); ++it ) {
        delete * it;
    }
    cntr.clear();
}

int CLH2tri::run(int argc, char* argv[])
{
    
    int num_points = 0;
    double max, min;
    double zoom;
    
    zoom = 1;
    cx = 0;
    cy = 0;
    
    vector<p2t::Point*> polyline;
    for(int i =0 ;i < COAST_POINTS; i++)
    {
        polyline.push_back(new Point(vCoast[i][0], vCoast[i][1]));
        num_points++;
    }
    
    cout << "Number of constrained edges = " << polyline.size() << endl;
    polylines.push_back(polyline);
    
    Init();
    
    /*
     * Perform triangulation!
     */
    
    double init_time = glfwGetTime();
    
    /*
     * STEP 1: Create CDT and add primary polyline
     * NOTE: polyline must be a simple polygon. The polyline's points
     * constitute constrained edges. No repeat points!!!
     */
    CDT* cdt = new CDT(polyline);
    
    /*
     * STEP 2: Add holes or Steiner points if necessary
     */
    if(1) {
        // Add head hole
        vector<Point*> head_hole = CreateHeadHole();
        num_points += head_hole.size();
        cdt->AddHole(head_hole);
        //// Add chest hole
        //vector<Point*> chest_hole = CreateChestHole();
        //num_points += chest_hole.size();
        //cdt->AddHole(chest_hole);
        polylines.push_back(head_hole);
        //polylines.push_back(chest_hole);
    }
    
    /*
     * STEP 3: Triangulate!
     */
    cdt->Triangulate();
    double dt = glfwGetTime() - init_time;
    
    triangles = cdt->GetTriangles();
    map = cdt->GetMap();
    
    cout << "Number of points = " << num_points << endl;
    cout << "Number of triangles = " << triangles.size() << endl;
    cout << "Elapsed time (ms) = " << dt*1000.0 << endl;
    
    MainLoop(zoom);
    
    // Cleanup
    
    delete cdt;
    
    // Free points
    for(int i = 0; i < polylines.size(); i++) {
        vector<Point*> poly = polylines[i];
        FreeClear(poly);
    }
    
    ShutDown(0);
    return 0;
}

void Init()
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

void ShutDown(int return_code)
{
    glfwTerminate();
    exit(return_code);
}

void MainLoop(const double zoom)
{
    // the time of the previous frame
    double old_time = glfwGetTime();
    // this just loops as long as the program runs
    bool running = true;
    
    while (running) {
        glfwPollEvents();
        
        // calculate time elapsed, and the amount by which stuff rotates
        double current_time = glfwGetTime(),
        delta_rotate = (current_time - old_time) * rotations_per_tick * 360;
        old_time = current_time;
        
        // escape to quit, arrow keys to rotate view
        // Check if ESC key was pressed or window was closed
        running = !glfwGetKey(window, GLFW_KEY_ESCAPE) && !glfwWindowShouldClose(window);
        
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            rotate_y += delta_rotate;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            rotate_y -= delta_rotate;
        // z axis always rotates
        rotate_z += delta_rotate;
        
        // Draw the scene
        if (draw_map) {
            DrawMap(zoom);
        } else {
            Draw(zoom);
        }
        
        // swap back and front buffers
        glfwSwapBuffers(window);
    }
}

void ResetZoom(double zoom, double cx, double cy, double width, double height)
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
    glScalef(10, 10, 10);
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_DEPTH_TEST);
    glLoadIdentity();
    
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
}

void Draw(const double zoom)
{
    // reset zoom
    Point center = Point(cx, cy);
    ResetZoom(zoom, center.x, center.y, 800, 600);
    for (int i = 0; i < triangles.size(); i++) {
        Triangle& t = *triangles[i];
        Point& a = *t.GetPoint(0);
        Point& b = *t.GetPoint(1);
        Point& c = *t.GetPoint(2);
        
        // Red
        glColor3f(1, 0, 0);
        
        glBegin(GL_TRIANGLES);
        glVertex2f(a.x, a.y);
        glVertex2f(b.x, b.y);
        glVertex2f(c.x, c.y);
        glEnd();
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

void DrawMap(const double zoom)
{
    // reset zoom
    Point center = Point(cx, cy);
    
    ResetZoom(zoom, center.x, center.y, 800, 600);
    
    list<Triangle*>::iterator it;
    for (it = map.begin(); it != map.end(); it++) {
        Triangle& t = **it;
        Point& a = *t.GetPoint(0);
        Point& b = *t.GetPoint(1);
        Point& c = *t.GetPoint(2);
        
        ConstrainedColor(t.constrained_edge[2]);
        glBegin(GL_LINES);
        glVertex2f(a.x, a.y);
        glVertex2f(b.x, b.y);
        glEnd( );
        
        ConstrainedColor(t.constrained_edge[0]);
        glBegin(GL_LINES);
        glVertex2f(b.x, b.y);
        glVertex2f(c.x, c.y);
        glEnd( );
        
        ConstrainedColor(t.constrained_edge[1]);
        glBegin(GL_LINES);
        glVertex2f(c.x, c.y);
        glVertex2f(a.x, a.y);
        glEnd( );
    }
}

void ConstrainedColor(bool constrain)
{
    if (constrain) {
        // Green
        glColor3f(0, 1, 0);
    } else {
        // Red
        glColor3f(1, 0, 0);
    }
}

vector<Point*> CreateHeadHole() {
    
    vector<Point*> head_hole;
    for(int i =0 ;i < LAKE_POINTS; i++)
    {
        head_hole.push_back(new Point(vLake[i][0], vLake[i][1]));
        //num_points++;
    }
    return head_hole;
}

vector<Point*> CreateChestHole() {
    
    vector<Point*> chest_hole;
    chest_hole.push_back(new Point(320.72342,480));
    chest_hole.push_back(new Point(338.90617,465.96863));
    chest_hole.push_back(new Point(347.99754,480.61584));
    chest_hole.push_back(new Point(329.8148,510.41534));
    chest_hole.push_back(new Point(339.91632,480.11077));
    chest_hole.push_back(new Point(334.86556,478.09046));
    
    return chest_hole;
    
}



double StringToDouble(const std::string& s)
{
    std::istringstream i(s);
    double x;
    if (!(i >> x))
        return 0;
    return x;
}

double Fun(double x)
{
    return 2.5 + sin(10 * x) / x;
}

double Random(double (*fun)(double), double xmin = 0, double xmax = 1)
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




