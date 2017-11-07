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
#include <map>
#include <fstream>
#include "lh_type.h"

//CLFreetype
class CLFreetype
{
public:
    CLFreetype();
    ~CLFreetype();
public:
    float set_word(wchar_t&);
    unsigned int get_word_front(FTPoint, A_CHAEACTER&);
    unsigned int get_word_back(FTPoint, A_CHAEACTER&);
    unsigned int get_word_side(A_CHAEACTER_QUAD&);
    
    bool init_freetype();
    bool set_fontfile(const char*);
    float get_sizeface();
    void set_depth(float);
    void set_outset(float, float);
    void release_word();
    float get_depth();
    unsigned int get_index(wchar_t&);
    FTPoint kern_advance(unsigned int index1, unsigned int index2);
    float get_advance();
private:
    bool memory_face();
    float analy_charater(wchar_t&);
    
    bool process_contours(FT_GlyphSlot& slot);
    unsigned int front(FTPoint, A_CHAEACTER&);
    unsigned int back(FTPoint, A_CHAEACTER&);
    unsigned int side(A_CHAEACTER_QUAD&);
    unsigned int side_simple(A_CHAEACTER_QUAD&);
    unsigned int make(FTPoint offset, A_CHAEACTER&, FTGL_DOUBLE, int, float);
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
    const unsigned int _pointsize = 10000;
    unsigned int outset = 1.0;
    float _front_outset = 0.8;
    float _back_outset = 1.0;
    float _depth = 9.0;
    bool _beread = false;
    std::map<wchar_t, unsigned int> _map_chacter_index;
    std::ofstream _ofile;               //定义输出文件
    
};

#endif /* lh_freetype_hpp */
