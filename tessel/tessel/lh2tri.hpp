//
//  lh2tri.hpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/25.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#ifndef lh2tri_hpp
#define lh2tri_hpp

#include "glfw/glfw3.h"
#include "glbase.hpp"
#include "../poly2tri/poly2tri.h"
#include <vector>
#include <string>

using namespace std;
using namespace p2t;

class CLH2tri :public CGL
{
public:
    CLH2tri();
    ~CLH2tri();
public:
    int run(int args, char *argv[]);
private:
    template <class C> void FreeClear( C & cntr ) {
        for ( typename C::iterator it = cntr.begin();
             it != cntr.end(); ++it ) {
            delete * it;
        }
        cntr.clear();
    }
private:
    void Release();
    void Init();
    void ShutDown(int return_code);
    void MainLoop(const double zoom);
    void Draw(const double zoom);
    double StringToDouble(const std::string& s);
    double Random(double (*fun)(double), double xmin, double xmax);
    double Fun(double x);
    void ResetZoom(double zoom, double cx, double cy, double width, double height);
private:
    float scalae = 0.04;
    float rotate_y = 0;
    float rotate_z = 45;
    const float rotations_per_tick = .2;
    
    /// Screen center x
    double cx = 0.0;
    /// Screen center y
    double cy = 0.0;
    
    /// Constrained triangles
    vector< vector<Triangle*> > triangles;
    vector<CDT*> vec_cdt;
    /// Polylines
    vector< vector<Point*> > polylines;
    GLFWwindow* window = NULL;
};

#endif /* lh2tri_hpp */
