//
//  lh_ptri.cpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/11/6.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#include "lh_ptri.hpp"
#include "../poly2tri/poly2tri.h"

CLhPTri::CLhPTri()
{
    ;
}

CLhPTri::~CLhPTri()
{
    ;
}


float *CLhPTri::create_side(A_CHAEACTER_QUAD &quad, unsigned int &counts)
{
    //typedef std::vector< std::vector<LFPoint3> >  A_CHAEACTER_QUAD;
    counts = 0;
    for (A_CHAEACTER_QUAD::iterator iter = quad.begin();
         iter != quad.end();
         iter++)
    {
        counts += iter->size();
    }
    //相邻4个点组成的四边形，得到两个三角形6个点，每个点3个float
    counts /= 2;
    counts *= 18; //6*3
    
    int n = 0;
    float *buff = new float[counts];
    for (A_CHAEACTER_QUAD::iterator iter = quad.begin();
         iter != quad.end();
         iter++)
    {
        unsigned long nums = iter->size();
        for (long i = 0; i <= nums - 4;)
        {
            LFPoint3 &a = iter->at(i);
            LFPoint3 &b = iter->at(i + 1);
            LFPoint3 &d = iter->at(i + 2);
            LFPoint3 &c = iter->at(i + 3);
            que2tri(buff + n, a, b, c, d);
            i += 2;
            n += 18;
        }
        
        LFPoint3 &a = iter->at(nums - 2);
        LFPoint3 &b = iter->at(nums - 1);
        LFPoint3 &d = iter->at(0);
        LFPoint3 &c = iter->at(1);
        que2tri(buff + n, a, b, c, d);
        n += 18;
    }
    return buff;
}

void CLhPTri::insertpoint(float *buff, float x, float y, float z)
{
    *buff = x;
    *(buff + 1) = y;
    *(buff + 2) = z;
}
 

void CLhPTri::que2tri(float *buffer, LFPoint3 &a, LFPoint3 &b, LFPoint3 &c, LFPoint3 &d)
{
    insertpoint(buffer, a.x, a.y, a.z);
    insertpoint(buffer + 3, b.x, b.y, b.z);
    insertpoint(buffer + 6, c.x, c.y, c.z);
    
    insertpoint(buffer + 9, a.x, a.y, a.z);
    insertpoint(buffer + 12, c.x, c.y, c.z);
    insertpoint(buffer + 15, d.x, d.y, d.z);
}

float* CLhPTri::create_thri(float depth, A_CHAEACTER &points, unsigned int &counts)
{
    int triangle_counts = 0;
    std::vector<p2t::CDT *> cdts;
    std::vector<std::vector<p2t::Point *>> vecpolys;
    std::vector<std::vector<p2t::Triangle *>> triangles;
    for (A_CHAEACTER::iterator iter = points.begin();
         iter != points.end();
         iter++)
    {
        std::vector<p2t::Point *> vecpoly;
        vecpoly.clear();
        for (std::vector<LFPoint>::iterator iterpoint = iter->begin();
             iter->end() != iterpoint;
             iterpoint++)
        {
            vecpoly.push_back(new p2t::Point(iterpoint->x, iterpoint->y));
        }
        vecpolys.push_back(vecpoly);
        p2t::CDT *cdt = new p2t::CDT(vecpoly);
        //if(0) {
        // Add head hole
        //vector<Point*> head_hole = CreateHeadHole();
        //当前轮廓iter如果有洞head_hole，需要手动AddHole进来。
        //cdt->AddHole(head_hole.size);
        //polylines.push_back(head_hole);
        //}
        
        cdt->Triangulate();
        triangles.push_back(cdt->GetTriangles());
        triangle_counts += triangles.at(triangles.size() - 1).size();
        cdts.push_back(cdt);
    }
    
    int n = 0;
    counts = triangle_counts * 9; //一个三角行三个点，一个点三个float
    float *buff = new float[counts];
    for (std::vector<std::vector<p2t::Triangle *>>::iterator iter = triangles.begin();
         iter != triangles.end();
         iter++)
    {
        unsigned long num = iter->size();
        for (long i = 0; i < num; i++)
        {
            p2t::Triangle &t = *(iter->at(i));
            p2t::Point &a = *t.GetPoint(0);
            p2t::Point &b = *t.GetPoint(1);
            p2t::Point &c = *t.GetPoint(2);
            insertpoint(buff + n, a.x / 64.0f, a.y / 64.0f, depth);
            insertpoint(buff + n + 3, b.x / 64.0f, b.y / 64.0f, depth);
            insertpoint(buff + n + 6, c.x / 64.0f, c.y / 64.0f, depth);
            n += 9;
        }
    }
    
    //清除
    for (std::vector<std::vector<p2t::Point *>>::iterator iter = vecpolys.begin();
         iter != vecpolys.end();
         iter++)
    {
        for (std::vector<p2t::Point *>::iterator iterpoint = iter->begin();
             iter->end() != iterpoint;
             iterpoint++)
        {
            delete *iterpoint;
        }
        iter->clear();
    }
    vecpolys.clear();
    
    for (std::vector<p2t::CDT *>::iterator iter = cdts.begin();
         iter != cdts.end();
         iter++)
    {
        delete *iter;
    }
    cdts.clear();
    triangles.clear();
    
    return buff;
}
