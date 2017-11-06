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
    
    _offset = 0;
    for (int i = 0; i < len; i++)
    {
        insert_aword(pwch[i]);
    }
    
    update_buff_datas();
    _offset = 0;
    return true;
}

bool CGenerateTri::insert_map(wchar_t wch, TRI_POINT *tri)
{
    PAIR_CHARACTER_BOOL ret;
    ret = _map_charactes.insert(std::pair<wchar_t, TRI_POINT *>(wch, tri));
    return ret.second;
}

TRI_POINT *CGenerateTri::find_map(wchar_t wch)
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
    rt = sizeof(float) * _tol_bufflen[CHARACTE_FRONT];
    return rt;
}
unsigned int CGenerateTri::get_back_buff_size()
{
    unsigned int rt = 0;
    rt = sizeof(float) * _tol_bufflen[CHARACTE_BACK];
    return rt;
}

unsigned int CGenerateTri::get_side_buff_size()
{
    unsigned int rt = 0;
    rt = sizeof(float) * _tol_bufflen[CHARACTE_SIDE];
    return rt;
}

const char *CGenerateTri::get_front_buff()
{
    return (const char *)_tol_buff[CHARACTE_FRONT];
}
const char *CGenerateTri::get_back_buff()
{
    return (const char *)_tol_buff[CHARACTE_BACK];
}
const char *CGenerateTri::get_side_buff()
{
    return (const char *)_tol_buff[CHARACTE_SIDE];
}

void CGenerateTri::release()
{
    clear();
    release_buff_datas();
}

float CGenerateTri::str2f(const std::string &s)
{
    std::istringstream i(s);
    float x = 0;
    if (!(i >> x))
        return 0;
    return x;
}

float CGenerateTri::get_sizeface_scalae(float fontsize)
{
    float size = _lh_freetype.get_sizeface();
    return fontsize/size;
}
void CGenerateTri::set_depth(float depth)
{
    _lh_freetype.set_depth(depth);
}
void CGenerateTri::set_outset(float front_outset, float back_outset)
{
    _lh_freetype.set_outset(front_outset, back_outset);
}

bool CGenerateTri::insert_aword(wchar_t wch)
{
    TRI_POINT *word_tri ;//= find_map(wch);
    //if (nullptr == word_tri)
    {
        word_tri = insert_characte(wch);
        //insert_map(wch, word_tri);
    }
    _vec_charactes.push_back(word_tri);
    return true;
}

void CGenerateTri::release_buff_datas()
{
    for (int  iter = CHARACTE_FRONT;
        iter < CHARACTE_COUNTS;
        iter++){
        _tol_bufflen[iter] = 0;
        if (nullptr != _tol_buff[iter]){
            delete [] _tol_buff[iter];
            _tol_buff[iter] = nullptr;
        }
    }
}

void CGenerateTri::update_buff_datas()
{
    release_buff_datas();
    VEC_CHARACTERS::iterator iter_words = _vec_charactes.begin();
    for (;
        iter_words != _vec_charactes.end();
         iter_words++){
        _tol_bufflen[CHARACTE_FRONT] += (*iter_words)->bufflen[CHARACTE_FRONT];
        _tol_bufflen[CHARACTE_BACK] += (*iter_words)->bufflen[CHARACTE_BACK];
        _tol_bufflen[CHARACTE_SIDE] += (*iter_words)->bufflen[CHARACTE_SIDE];
    }
    
    for (int  iter = CHARACTE_FRONT;
         iter < CHARACTE_COUNTS;
         iter++){
        _tol_buff[iter] = new float[_tol_bufflen[iter]];
    }
    
    //FTPoint fpoint(0.0, 0.0);
    //int index_point = 0;
    unsigned int len[CHARACTE_COUNTS] = {0};
    iter_words = _vec_charactes.begin();
    for (;
         iter_words != _vec_charactes.end();
         iter_words++){
        //fpoint += get_offset(index_point++);
        for (int  iter = CHARACTE_FRONT;
             iter < CHARACTE_COUNTS;
             iter++){
            memcpy(_tol_buff[iter] + len[iter],
                   (*iter_words)->buff[iter],
                   sizeof(float)*(*iter_words)->bufflen[iter]);
            len[iter] += (*iter_words)->bufflen[iter];
        }
    }
}

TRI_POINT *CGenerateTri::insert_characte(wchar_t wch)
{
    A_CHAEACTER fronts;
    A_CHAEACTER backs;
    A_CHAEACTER_QUAD sides;
    
    float advance =_lh_freetype.set_word(wch);
    _offset += advance;
    FTPoint offset(_offset - advance, 0);
    _lh_freetype.get_word_front(offset, fronts);
    _lh_freetype.get_word_back(offset, backs);
    _lh_freetype.get_word_side(sides);
    unsigned int 　front_pointsum = 0;
    unsigned int backpointsum = 0;
    unsigned int sidepointsum = 0;

    TRI_POINT *word_tri = new TRI_POINT;
    word_tri->buff[CHARACTE_FRONT] = _lh_poly2tri.create_thri(0, fronts, front_pointsum);
    word_tri->buff[CHARACTE_BACK] = _lh_poly2tri.create_thri(-_lh_freetype.get_depth(),
                                                backs, backpointsum);
    word_tri->bufflen[CHARACTE_FRONT] = 　front_pointsum;
    word_tri->bufflen[CHARACTE_BACK] = backpointsum;

    word_tri->buff[CHARACTE_SIDE] = _lh_poly2tri.create_side(sides, sidepointsum);
    word_tri->bufflen[CHARACTE_SIDE] = sidepointsum;

    return word_tri;
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
    _vec_charactes.clear();
    _map_charactes.clear();
    
}

bool CGenerateTri::set_fontfile(const char *path)
{
    return _lh_freetype.set_fontfile(path);
}

void CGenerateTri::load_freetype()
{
    _lh_freetype.init_freetype();
}

