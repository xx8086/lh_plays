//
//  lh_generate_tri.hpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/30.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#ifndef lh_generate_tri_hpp
#define lh_generate_tri_hpp

#include <stdio.h>
#include "../poly2tri/poly2tri.h"
#include "lh_freetype.hpp"
#include <map>
#include <vector>

typedef enum CharactePostion
{
    CHARACTE_FRONT = 0,
    CHARACTE_BACK = 1,
    CHARACTE_SIDE = 2,
    CHARACTE_COUNTS,
} CHARACTE_POSTION;

typedef struct TriPoint{
public:
    unsigned int frontpointlen;
    unsigned int sidepointlen;
    float* buff[CHARACTE_COUNTS];
    
    TriPoint(){
        release();
    }
    ~TriPoint(){
        release();
    }
    void release(){
        for(int i = 0; i < CHARACTE_COUNTS; i++)
        {
            if(nullptr != buff[i])
            {
                delete [] buff[i];
                buff[i] = nullptr;
            }
        }
        frontpointlen = 0;
        sidepointlen = 0;
    }
} TRI_POINT;

class CGenerateTri
{
public:
    CGenerateTri();
    ~CGenerateTri();
public:
    unsigned int get_front_buff_size();
    unsigned int get_back_buff_size();
    unsigned int get_side_buff_size();
    const char* get_front_buff();
    const char* get_back_buff();
    const char* get_side_buff();
    
    void set_typeface(const char*);
    void set_sizeface(float);
    void set_depth(float);
    void set_outset(float, float);
    
public:
    void loaddatas();
private:
    //读取本地到处的字数据文件
    typedef struct Vector3f{
        Vector3f(){};
        float x[3];
        float y[3];
        float z[3];
    } VECTOR_3F;
    std::vector<float>   triangle_points;
    std::vector<float>   back_triangle_points;
    std::vector<float>   side_triangle_points;
    std::vector< std::vector<p2t::Triangle*> > triangles;
    std::vector<p2t::CDT*> vec_cdt;
    std::vector< std::vector<p2t::Point*> > polylines;
    
    
    void que2tri(std::vector<float>&, float x, float y, float x0, float y0);
    void insertpoint(std::vector<float>& triangles, float x, float y, float z);
    void create_side();
    void create_vertex_buffer(float deep);
    void release();
    float str2f(const std::string& s);
    template <class C> void FreeClear( C & cntr ) {
        for ( typename C::iterator it = cntr.begin();
             it != cntr.end(); ++it ) {
            delete * it;
        }
        cntr.clear();
    }
private:
    //自己解析字体库，得到字的数据内容
    CLFreetype _lh_freetype;
private:
    TriPoint* find(wchar_t, bool = false);
    bool insert_characte(wchar_t,
                         std::vector< std::vector<p2t::Point*> >&);
    void insertpoint(float* buff, float x, float y, float z);
    void que2tri(float* buffer, float x, float y, float x0, float y0);
    bool create_vertex_buffer(wchar_t,
                              unsigned int,
                              unsigned int,
                              std::vector< std::vector<p2t::Triangle*> >&,
                              std::vector< std::vector<p2t::Point*> >&);
private:
    typedef std::pair<std::map<wchar_t, TRI_POINT>::iterator,bool> PAIR_CHARACTER_BOOL;
    typedef std::pair<wchar_t, TRI_POINT> PAIR_CHARACTER;
    typedef std::map<wchar_t, TRI_POINT> MAP_CHARACTERS;
    MAP_CHARACTERS _map_charactes;
    
    float _deep = 15.0f;
};

#endif /* lh_generate_tri_hpp */
