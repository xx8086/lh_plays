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
#include "lh_freetype.hpp"
#include "lh_ptri.hpp"
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
    float get_sizeface_scalae(float size);
    //获取字体大小为size需要缩放的倍数(size较小时freetype获取点有问题，内部为固定个1000size，由opengl缩放来控制字体大小)。
    void set_depth(float);
    void set_outset(float, float);
    void show();
    void hide();
    void clear();

    bool set_fontfile(const char*);
    void load_freetype();
    
    void font_rend(CHARACTE_POSTION postion);
    
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
    TRI_POINT* insert_characte(wchar_t);
    
    void update_buff_datas();
    void release_buff_datas();
    void bind_vertex_buffer();
private://render
    
    void bindshader(unsigned int vao,
                    unsigned int vbo,
                    unsigned int size,
                    const char* datas);
    void drawshader(unsigned int vao,
                    unsigned int vbo,
                    unsigned int size);
private:
    struct v_x_o{
        unsigned int vxo[CHARACTE_COUNTS];
    };
    typedef std::vector<v_x_o> VEC_VXO;
    VEC_VXO _vec_vbos;
    VEC_VXO _vec_vaos;
private:
    CLFreetype _lh_freetype;
    CLhPTri _lh_poly2tri;

    typedef std::vector<TRI_POINT*> VEC_CHARACTERS;
    VEC_CHARACTERS _vec_charactes;
    unsigned int _tol_bufflen[CHARACTE_COUNTS] = {0};
    float* _tol_buff[CHARACTE_COUNTS] = {nullptr};
    std::vector<float> _vec_tol[CHARACTE_COUNTS];
    float _offset;
    bool _show = false;
};

#endif /* lh_generate_tri_hpp */
