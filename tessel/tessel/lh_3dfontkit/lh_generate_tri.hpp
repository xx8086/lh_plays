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
    unsigned int bufflen[CHARACTE_COUNTS] = {0};
    float* buff[CHARACTE_COUNTS] = {nullptr};
    
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
            bufflen[i] = 0;
        }
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

    bool insert_words(wchar_t*, int);
    void set_sizeface(float);
    void set_depth(float);
    void set_outset(float, float);
    void show();
    void hide();
    void clear();

    bool set_fontfile(const char*);
    void load_freetype();
public:
    float str2f(const std::string &s);
    template <class C>
    void FreeClear(C &cntr)
    {
        for (typename C::iterator it = cntr.begin();
             it != cntr.end(); ++it)
        {
            delete *it;
        }
        cntr.clear();
    }
private:
    void release();
    bool insert_aword(wchar_t);
    TRI_POINT* find_map(wchar_t);
    bool insert_map(wchar_t, TRI_POINT*);
    TRI_POINT* insert_characte(wchar_t);
    void insertpoint(float* buff, float x, float y, float z);
    void que2tri(float* buffer, LFPoint3& a, LFPoint3& b, LFPoint3& c, LFPoint3& d);
    float* create_thri(float, A_CHAEACTER&, unsigned int&);
    float* create_side(A_CHAEACTER_QUAD&, unsigned int&);
    void update_buff_datas();
    void release_buff_datas();
private:
    CLFreetype _lh_freetype;
    typedef std::pair<std::map<wchar_t, TRI_POINT*>::iterator,bool> PAIR_CHARACTER_BOOL;
    typedef std::pair<wchar_t, TRI_POINT*> PAIR_CHARACTER;
    typedef std::map<wchar_t, TRI_POINT*> MAP_CHARACTERS;
    MAP_CHARACTERS _map_charactes;

    typedef std::vector<TRI_POINT*> VEC_CHARACTERS;
    VEC_CHARACTERS _vec_charactes;
    unsigned int _tol_bufflen[CHARACTE_COUNTS] = {0};
    float* _tol_buff[CHARACTE_COUNTS] = {nullptr};
    float _offset;
    //const unsigned int _add_buffs = 1024;
    bool _show = false;
};

#endif /* lh_generate_tri_hpp */
