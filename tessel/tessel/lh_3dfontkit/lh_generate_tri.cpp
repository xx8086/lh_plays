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

#define LOAD_STATIC_DATAS 0

CGenerateTri::CGenerateTri()
{
}

CGenerateTri::~CGenerateTri()
{
    release();
}

bool CGenerateTri::insert_words(wchar_t *pwch, int len)
{
    if (len < 1 || nullptr == pwch)
    {
        return false;
    }

    for (int i = 0; i < len; i++)
    {
        insert_aword(pwch[i]);
    }
    return true;
}

void CGenerateTri::insertpoint(float *buff, float x, float y, float z)
{
    *buff = x;
    *(buff + 1) = y;
    *(buff + 2) = z;
}
void CGenerateTri::que2tri(float* buffer, LFPoint3& a, LFPoint3& b, LFPoint3& c, LFPoint3& d)
{
    insertpoint(buffer, a.x, a.y, a.z);
    insertpoint(buffer + 3, b.x, b.y, b.z);
    insertpoint(buffer + 6, c.x, c.y, c.z);

    insertpoint(buffer + 9, a.x, a.y, a.z);
    insertpoint(buffer + 12, c.x, c.y, c.z);
    insertpoint(buffer + 15, d.x, d.y, d.z);
}

bool CGenerateTri::insert_map(wchar_t wch, TRI_POINT* tri)
{
    PAIR_CHARACTER_BOOL ret;
    ret = _map_charactes.insert(std::pair<wchar_t, TRI_POINT*>(wch, tri));
    return ret.second;
}

TRI_POINT* CGenerateTri::find_map(wchar_t wch)
{
    TRI_POINT *rt = nullptr;
    MAP_CHARACTERS::iterator iter = _map_charactes.find(wch);
    if (_map_charactes.end() != iter)
    {
        rt = iter->second;
    }

    return rt;
}
////////

unsigned int CGenerateTri::get_front_buff_size()
{
    unsigned int rt = 0;
    rt = _vec_charactes.at(0)->bufflen[CHARACTE_FRONT];
    rt *= sizeof(float);
    return rt;
}
unsigned int CGenerateTri::get_back_buff_size()
{
    unsigned int rt = 0;
    rt = _vec_charactes.at(0)->bufflen[CHARACTE_BACK];
    rt *= sizeof(float);
    return rt;
}

unsigned int CGenerateTri::get_side_buff_size()
{
    unsigned int rt = 0;
    rt = _vec_charactes.at(0)->bufflen[CHARACTE_SIDE];
    rt *= sizeof(float);
    return rt;
}

const char *CGenerateTri::get_front_buff()
{
    return (const char *)_vec_charactes.at(0)->buff[CHARACTE_FRONT];
}
const char *CGenerateTri::get_back_buff()
{
    return (const char *)_vec_charactes.at(0)->buff[CHARACTE_BACK];
}
const char *CGenerateTri::get_side_buff()
{
    return (const char *)_vec_charactes.at(0)->buff[CHARACTE_SIDE];
}


void CGenerateTri::release()
{
    clear();
}

float CGenerateTri::str2f(const std::string &s)
{
    std::istringstream i(s);
    float x;
    if (!(i >> x))
        return 0;
    return x;
}


void CGenerateTri::set_sizeface(float fontsize)
{
    _lh_freetype.set_sizeface(fontsize);
}
void CGenerateTri::set_depth(float depth)
{
    _depth = depth;
    _lh_freetype.set_depth(depth);
}
void CGenerateTri::set_outset(float front_outset, float back_outset)
{
    _lh_freetype.set_outset(front_outset, back_outset);
}

bool CGenerateTri::insert_aword(wchar_t wch)
{
    TRI_POINT* word_tri = find_map(wch);
    if (nullptr == word_tri)
    {
        word_tri = insert_characte(wch);
        insert_map(wch, word_tri);
    }
    _vec_charactes.push_back(word_tri);
    return true;
}

float* CGenerateTri::create_thri(float depth, A_CHAEACTER& points, unsigned int& counts)
{
    int triangle_counts = 0;
    std::vector<p2t::CDT *> cdts;
    std::vector<std::vector<p2t::Point *> > vecpolys;
    std::vector<std::vector<p2t::Triangle *> > triangles;
    for (A_CHAEACTER::iterator iter = points.begin();
         iter != points.end();
         iter++)
    {
        std::vector<p2t::Point *> vecpoly;
        vecpoly.clear();
        for(std::vector<LFPoint>::iterator iterpoint = iter->begin();
        iter->end() != iterpoint;
        iterpoint++){
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
    counts = triangle_counts * 9;//一个三角行三个点，一个点三个float
    float* buff = new float[counts];
    for (std::vector<std::vector<p2t::Triangle *> >::iterator iter = triangles.begin();
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
    for (std::vector<std::vector<p2t::Point*> >::iterator iter = vecpolys.begin();
         iter != vecpolys.end();
         iter++)
    {
        for(std::vector<p2t::Point*>::iterator iterpoint = iter->begin();
        iter->end() != iterpoint;
        iterpoint++){
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

TRI_POINT* CGenerateTri::insert_characte(wchar_t wch)
{
    A_CHAEACTER fronts;
    A_CHAEACTER backs;
    A_CHAEACTER_QUAD sides;
    _lh_freetype.set_word(wch);
    _lh_freetype.get_word_front(fronts);
    _lh_freetype.get_word_back(backs);
    _lh_freetype.get_word_side(sides);
    unsigned int frontpointsum = 0;
    unsigned int backpointsum = 0;
    unsigned int sidepointsum = 0;

    TRI_POINT* word_tri = new TRI_POINT;
    word_tri->buff[CHARACTE_FRONT] = create_thri(0, fronts, frontpointsum);
    word_tri->buff[CHARACTE_BACK] = create_thri(-_depth, backs, backpointsum);
    word_tri->bufflen[CHARACTE_FRONT] = frontpointsum;
    word_tri->bufflen[CHARACTE_BACK] = backpointsum;
    
    word_tri->buff[CHARACTE_SIDE] = create_side(sides, sidepointsum);
    word_tri->bufflen[CHARACTE_SIDE] = sidepointsum;

    return word_tri;
}
float* CGenerateTri::create_side(A_CHAEACTER_QUAD& quad, unsigned int& counts)
{
    //typedef std::vector< std::vector<LFPoint3> >  A_CHAEACTER_QUAD;
    counts = 0;
    for (A_CHAEACTER_QUAD::iterator iter = quad.begin();
    iter != quad.end();
    iter++){
        counts += iter->size();
    }
    //相邻4个点组成的四边形，得到两个三角形6个点，每个点3个float
    counts /= 2;
    counts *= 18;//6*3
    
    int n = 0;
    float* buff = new float[counts];
    for (A_CHAEACTER_QUAD::iterator iter = quad.begin();
         iter != quad.end();
         iter++)
    {
        unsigned long nums = iter->size();
        for (long i = 0; i <= nums - 4; )
        {
            LFPoint3& a = iter->at(i);
            LFPoint3& b = iter->at(i+1);
            LFPoint3& d = iter->at(i+2);
            LFPoint3& c = iter->at(i+3);
            que2tri(buff + n, a, b, c, d);
            i += 2;
            n += 18;
        }

        LFPoint3& a = iter->at(nums - 2);
        LFPoint3& b = iter->at(nums - 1);
        LFPoint3& d = iter->at(0);
        LFPoint3& c = iter->at(1);
        que2tri(buff + n, a, b, c, d);
        n += 18;
    }
    return buff;
}

void CGenerateTri::show()
{
    _show = true;
}

void CGenerateTri::hide()
{
    _show = false;
}

void CGenerateTri::clear()
{
    _show = false;
    for (MAP_CHARACTERS::iterator iter = _map_charactes.begin();
         iter != _map_charactes.end();
         iter++)
    {
        iter->second->release();
    }
    _map_charactes.clear();
    _vec_charactes.clear();
}

bool CGenerateTri::set_fontfile(const char *path)
{
    return _lh_freetype.set_fontfile(path);
}

void CGenerateTri::load_freetype()
{
    _lh_freetype.init_freetype();
}
