//
//  lh_generate_tri.cpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/30.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#include "lh_generate_tri.hpp"
#include <fstream>
#include <sstream>

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#define LOAD_STATIC_DATAS 1

CGenerateTri::CGenerateTri()
{}

CGenerateTri::~CGenerateTri()
{}

void CGenerateTri::insertpoint(float* buff, float x, float y, float z)
{
    *buff = x;
    *(buff + 1) = y;
    *(buff + 2) = z;
}

bool CGenerateTri::insert_characte(wchar_t wch,
                                   std::vector< std::vector<p2t::Point*> >& polylines)
{
    int fontpoints = 0;
    int sidecounts = 0;
    std::vector< std::vector<p2t::Triangle*> > triangles;
    std::vector<p2t::CDT*> cdts;
    for(std::vector< std::vector<p2t::Point*> >::iterator iter = polylines.begin();
        iter != polylines.end();
        iter++){
        
        p2t::CDT* cdt = new p2t::CDT(*iter);
        //if(0) {
        // Add head hole
        //vector<Point*> head_hole = CreateHeadHole();
        //当前轮廓iter如果有洞head_hole，需要手动AddHole进来。
        //cdt->AddHole(head_hole.size);
        //polylines.push_back(head_hole);
        //}
        
        cdt->Triangulate();
        triangles.push_back(cdt->GetTriangles());
        
        sidecounts += iter->size();
        fontpoints += triangles.at(triangles.size() - 1).size();
        cdts.push_back(cdt);
    }
    
    create_vertex_buffer(wch,
                         fontpoints,
                         sidecounts,
                         triangles,
                         polylines);
    
    for(std::vector<p2t::CDT*>::iterator iter = cdts.begin();
        iter != cdts.end();
        iter++){
        delete *iter;
    }
    cdts.clear();
    triangles.clear();
    
    return true;
}


bool CGenerateTri::create_vertex_buffer(wchar_t wch,
                                        unsigned int frontbufflen,
                                        unsigned int sidebufflen,
                                        std::vector< std::vector<p2t::Triangle*> >& triangles,
                                        std::vector< std::vector<p2t::Point*> >& polylines)
{
    TriPoint* tp = find(wch, true);
    if (nullptr == tp)
    {
        return false;
    }
    
    float** charbuffs = tp->buff;
    
    //一个三角形存3个点，一个点有x,y,z,3个float组成。
    tp->frontpointlen = frontbufflen * 9;
    charbuffs[CHARACTE_FRONT] = new float[tp->frontpointlen];
    charbuffs[CHARACTE_BACK] = new float[tp->frontpointlen];
    float* frontbuff = (float*)charbuffs[CHARACTE_FRONT];
    float* backbuff = (float*)charbuffs[CHARACTE_BACK];
    int n = 0;
    for(std::vector< std::vector<p2t::Triangle*> >::iterator iter = triangles.begin();
        iter != triangles.end();
        iter++){
        unsigned long num = iter->size();
        for (long i = 0; i < num; i++) {
            p2t::Triangle& t = *(iter->at(i));
            p2t::Point& a = *t.GetPoint(0);
            p2t::Point& b = *t.GetPoint(1);
            p2t::Point& c = *t.GetPoint(2);
            insertpoint(frontbuff + n, a.x/64.0f, a.y/64.0f, 0.0f);
            insertpoint(frontbuff + n + 3, b.x/64.0f, b.y/64.0f, 0.0f);
            insertpoint(frontbuff + n + 6, c.x/64.0f, c.y/64.0f, 0.0f);
            
            insertpoint(backbuff + n, a.x/64.0f, a.y/64.0f, -_deep);
            insertpoint(backbuff + n + 3, b.x/64.0f, b.y/64.0f, -_deep);
            insertpoint(backbuff + n + 6, c.x/64.0f, c.y/64.0f, -_deep);
            
            n += 9;
        }
    }
    
    //当前点和下一个点加上deep的四边形组成两个三角形共有6个点。一个点x,y,z，3个float。
    //最后一个点和初始点组成四边形。
    tp->sidepointlen = frontbufflen * 18;
    charbuffs[CHARACTE_SIDE] = new float[tp->sidepointlen];
    float* sidebuff = charbuffs[CHARACTE_SIDE];
    for(std::vector< std::vector<p2t::Point*> >::iterator iter = polylines.begin();
        iter != polylines.end();
        iter++){
        unsigned long nums = iter->size();
        for(long i = 0; i < nums - 1; i++)
        {
            p2t::Point* current = iter->at(i);
            p2t::Point* next = iter->at(i+1);
            que2tri(sidebuff + i*18, current->x/64.0f, current->y/64.0f, next->x/64.0f, next->y/64.0f);
        }
        
        p2t::Point* current = iter->at(nums - 1);
        p2t::Point* next = iter->at(0);
        que2tri(sidebuff + 18*(nums - 1), current->x, current->y, next->x, next->y);
    }
    return true;
}

void CGenerateTri::que2tri(float* buffer, float x, float y, float x0, float y0)
{
    float a[3] = {x, y, 0};
    float b[3] = {x, y, -_deep};
    float c[3] = {x0, y0, -_deep};
    float d[3] = {x0, y0, 0};
    insertpoint(buffer, a[0], a[1], a[2]);
    insertpoint(buffer + 3, b[0], b[1], b[2]);
    insertpoint(buffer + 6, c[0], c[1], c[2]);
    
    insertpoint(buffer + 9, a[0], a[1], a[2]);
    insertpoint(buffer + 12, c[0], c[1], c[2]);
    insertpoint(buffer + 15, d[0], d[1], d[2]);
}

TriPoint* CGenerateTri::find(wchar_t wch, bool autocreate)
{
    TriPoint* rt = nullptr;
    MAP_CHARACTERS::iterator iter = _map_charactes.find(wch);
    if (_map_charactes.end() == iter && autocreate)
    {
        TRI_POINT tp_chars;
        PAIR_CHARACTER_BOOL ret;
        ret = _map_charactes.insert(std::pair<wchar_t, TRI_POINT>(wch, tp_chars));
        if (true == ret.second)
        {
            rt = &ret.first->second;
        }
    }
    else if (_map_charactes.end() != iter)
    {
        rt = &iter->second;
        if (autocreate)
        {
            rt->release();
        }
    }
    
    return rt;
}
////////

unsigned int CGenerateTri::get_front_buff_size()
{
    unsigned int rt = 0;
#if LOAD_STATIC_DATAS
    rt = sizeof(float) * triangle_points.size();
#else
#endif
    return rt;
}
unsigned int CGenerateTri::get_back_buff_size()
{
    unsigned int rt = 0;
#if LOAD_STATIC_DATAS
    rt = sizeof(float) * back_triangle_points.size();
#else
#endif
    return rt;
}

unsigned int CGenerateTri::get_side_buff_size()
{
    unsigned int rt = 0;
#if LOAD_STATIC_DATAS
    rt = sizeof(float) * side_triangle_points.size();
#else
#endif
    return rt;
}

const char* CGenerateTri::get_front_buff()
{
#if LOAD_STATIC_DATAS
    return (const char*)side_triangle_points.data();
#else
#endif
}
const char* CGenerateTri::get_back_buff()
{
#if LOAD_STATIC_DATAS
    return (const char*)back_triangle_points.data();
#else
#endif
}
const char* CGenerateTri::get_side_buff()
{
#if LOAD_STATIC_DATAS
    return (const char*)side_triangle_points.data();
#else
#endif
}

///////////////


void CGenerateTri::create_vertex_buffer(float deep)
{
    triangle_points.clear();
    back_triangle_points.clear();
    for(std::vector< std::vector<p2t::Triangle*> >::iterator iter = triangles.begin();
        iter != triangles.end();
        iter++){
        long num = iter->size();
        for (long i = 0; i < num; i++) {
            p2t::Triangle& t = *(iter->at(i));
            p2t::Point& a = *t.GetPoint(0);
            p2t::Point& b = *t.GetPoint(1);
            p2t::Point& c = *t.GetPoint(2);
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

void CGenerateTri::release()
{
    // Cleanup
    for(std::vector<p2t::CDT*>::iterator iter = vec_cdt.begin();
        iter != vec_cdt.end();
        iter++){
        delete *iter;
    }
    vec_cdt.clear();
    
    // Free points
    for(int i = 0; i < polylines.size(); i++) {
        std::vector<p2t::Point*> poly = polylines[i];
        FreeClear(poly);
    }
}

float CGenerateTri::str2f(const std::string& s)
{
    std::istringstream i(s);
    float x;
    if (!(i >> x))
        return 0;
    return x;
}

void CGenerateTri::insertpoint(std::vector<float>& triangles, float x, float y, float z)
{
    triangles.push_back(x);
    triangles.push_back(y);
    triangles.push_back(z);
}


void CGenerateTri::que2tri(std::vector<float>& vecs, float x, float y, float x0, float y0)
{
    glm::vec3 a(x, y, 0);
    glm::vec3 b(x, y, -_deep);
    glm::vec3 c(x0, y0, -_deep);
    glm::vec3 d(x0, y0, 0);
    insertpoint(vecs, a.x, a.y, a.z);
    insertpoint(vecs, b.x, b.y, b.z);
    insertpoint(vecs, c.x, c.y, c.z);
    insertpoint(vecs, a.x, a.y, a.z);
    insertpoint(vecs, c.x, c.y, c.z);
    insertpoint(vecs, d.x, d.y, d.z);
}

void CGenerateTri::create_side()
{
    for(std::vector< std::vector<p2t::Point*> >::iterator iter = polylines.begin();
        iter != polylines.end();
        iter++){
        long nums = iter->size();
        for(long i = 0; i < nums - 1; i++)
        {
            p2t::Point* current = iter->at(i);
            p2t::Point* next = iter->at(i+1);
            que2tri(side_triangle_points, current->x/64.0f, current->y/64.0f, next->x/64.0f, next->y/64.0f);
        }
        p2t::Point* current = iter->at(nums - 1);
        p2t::Point* next = iter->at(0);
        que2tri(side_triangle_points, current->x, current->y, next->x, next->y);
    }
}

void CGenerateTri::loaddatas()
{
    std::vector<p2t::Point*> polyline;
    std::string line;
    std::ifstream myfile("/Users/baidu/lh_plays/tessel/a_pos.txt");
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
            std::istringstream iss(line);
            std::vector<std::string> tokens;
            copy(std::istream_iterator<std::string>(iss),
                 std::istream_iterator<std::string>(),
                 std::back_inserter<std::vector<std::string> >(tokens));
            double x = str2f(tokens[0]);
            double y = str2f(tokens[1]);
            polyline.push_back(new p2t::Point(x, y));
        }
        myfile.close();
    } else {
        printf("File not opened\n");
    }
    
    if (polyline.size() > 0)
    {
        polylines.push_back(polyline);
        polyline.clear();
    }
    
    for(std::vector< std::vector<p2t::Point*> >::iterator iter = polylines.begin();
        iter != polylines.end();
        iter++){
        
        p2t::CDT* cdt = new p2t::CDT(*iter);
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
    
    create_vertex_buffer(_deep);
    create_side();
}
