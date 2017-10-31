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
    void get_word_front(wchar_t&, A_CHAEACTER&);
    void get_word_side(wchar_t&, A_CHAEACTER&);
private:
    bool init_freetype();
    bool memory_face();
    bool analy_charater(wchar_t&);
    
    void process_contours(FT_GlyphSlot& slot);
    void front(A_CHAEACTER&);
    void back(A_CHAEACTER&);
    void side(A_CHAEACTER_QUAD&);
    void make(A_CHAEACTER&, FTGL_DOUBLE, int, float);
    void release_word();
private://动态的
    FTContour** _contour_list;
    int _contour_current_num = 0;
    int outflag;
    unsigned int hscale;
    unsigned int vscale;
    
private://固定的
    float _depth;
    FT_Error _error;
    FT_Library _library;
    FT_Face _face;
    unsigned int _window_x = 0;
    unsigned int _window_y = 0;
    unsigned int _pointsize;
    std::string _str_ttf;
    
    unsigned int outset;
    float _front_outset;
    float _back_outset;
};

#endif /* lh_freetype_hpp */
