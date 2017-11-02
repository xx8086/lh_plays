//
//  lh_freetype.hpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/30.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#ifndef lh_freetype_hpp
#define lh_freetype_hpp
#include <string>
#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_STROKER_H
#include "ftcontour.hpp"
#include <vector>
#include <fstream>

typedef struct LFPoint
{
    double x = 0.0;
    double y = 0.0;
    LFPoint(double a, double b):x(a), y(b){}
    LFPoint(){}
} LF_POINT;

typedef struct LFPoint3
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    LFPoint3(double a, double b, double c):
    x(a), y(b), z(c){}
    LFPoint3(){}
} LF_POINT3;

typedef std::vector< std::vector<LFPoint> >  A_CHAEACTER;
typedef std::vector< std::vector<LFPoint3> >  A_CHAEACTER_QUAD;

//CLFreetype
class CLFreetype
{
public:
    CLFreetype();
    ~CLFreetype();
public:
    bool set_word(wchar_t&);
    unsigned int get_word_front(A_CHAEACTER&);
    unsigned int get_word_back(A_CHAEACTER&);
    unsigned int get_word_side(A_CHAEACTER_QUAD&);
    
    bool init_freetype();
    bool set_fontfile(const char*);
    void set_sizeface(float);
    void set_depth(float);
    void set_outset(float, float);

    void release_word();
    
private:
    bool memory_face();
    bool analy_charater(wchar_t&);
    
    bool process_contours(FT_GlyphSlot& slot);
    unsigned int front(A_CHAEACTER&);
    unsigned int back(A_CHAEACTER&);
    unsigned int side(A_CHAEACTER_QUAD&);
    unsigned int make(A_CHAEACTER&, FTGL_DOUBLE, int, float);

private://动态的
    FTContour** _contour_list = nullptr;
    int _contour_current_num = 0;
    int outflag;
    unsigned int hscale;
    unsigned int vscale;
    
private://固定的
    FT_Error _error;
    FT_Library _library;
    FT_Face _face;
    unsigned int _window_x = 0;
    unsigned int _window_y = 0;

    std::string _str_ttf;
    unsigned int _pointsize = 400;
    unsigned int outset = 1.0;
    float _front_outset = 0.8;
    float _back_outset = 1.0;
    float _depth = 9.0;
    bool _beread = false;
    
    std::ofstream _ofile;               //定义输出文件
    
};

#endif /* lh_freetype_hpp */
