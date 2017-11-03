//
//  lh_freetype.cpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/30.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#include "lh_freetype.hpp"
#include <assert.h>

#define LFT_ERROR(err, cstr)  \
    if (_error)               \
    {                         \
        printf("%s\n", cstr); \
        return false;         \
    }

CLFreetype::CLFreetype()
{
}

CLFreetype::~CLFreetype()
{
    release_word();
}

bool CLFreetype::memory_face()
{
    std::ifstream fontfile(_str_ttf.c_str(), std::ios::binary);
    if (fontfile)
    {
        // Read the entire file to a memory buffer.
        fontfile.seekg(0, std::ios::end);
        std::fstream::pos_type fontfilesize = fontfile.tellg();
        fontfile.seekg(0);
        unsigned char *fontbuffer = new unsigned char[fontfilesize];
        fontfile.read((char *)fontbuffer, fontfilesize);
        _error = FT_New_Memory_Face(_library, fontbuffer, fontfilesize, 0, &_face);
        assert(!_error);

        delete[] fontbuffer;
        fontbuffer = nullptr;
        fontfile.close();
    }
    else
    {
        assert(0);
    }

    return true;
}

bool CLFreetype::analy_charater(wchar_t &wch)
{
    FT_UInt gindex = FT_Get_Char_Index(_face, wch);
    _error = FT_Load_Glyph(_face, gindex, FT_LOAD_NO_HINTING);
    assert(!_error);

    FT_GlyphSlot &slot = _face->glyph;
    _beread = process_contours(slot);
    return _beread;
}

bool CLFreetype::init_freetype()
{
    _error = FT_Init_FreeType(&_library);
    assert(!_error);
    //LFT_ERROR(_error, "an error occurred during library initialization.")

    _error = FT_New_Face(_library,
                         _str_ttf.c_str(),
                         0,
                         &_face);
    assert(!_error);
    LFT_ERROR(_error == FT_Err_Unknown_File_Format,
              "the font file could be opened and read, but it appears that its font format is unsupported.");

    _error = FT_Set_Char_Size(_face, 0L, _pointsize * 64, _window_x, _window_y);
    assert(!_error);

    _error = FT_Select_Charmap(_face, ft_encoding_unicode);
    assert(!_error);
    return true;
}

unsigned int CLFreetype::front(A_CHAEACTER &aword)
{
    return make(aword, 1.0, 1, _front_outset);
}
unsigned int CLFreetype::back(A_CHAEACTER &aword)
{
    return make(aword, -1.0, 2, _back_outset);
}
unsigned int CLFreetype::side(A_CHAEACTER_QUAD &aword)
{
    unsigned int tol = 0;
    for (size_t c = 0; c < _contour_current_num; ++c)
    {
        const FTContour *contour = _contour_list[c];
        size_t n = contour->point_count();
        if (n < 2)
        {
            continue;
        }

        std::vector<LFPoint3> quads;
        for (size_t j = 0; j <= n; ++j)
        {
            size_t cur = (j == n) ? 0 : j;
            size_t next = (cur == n - 1) ? 0 : cur + 1;

            FTPoint frontPt = contour->　front_point(cur);
            FTPoint backPt = contour->back_point(cur);
            if (outflag & ft_outline_reverse_fill)
            {
                quads.push_back(LFPoint3(backPt.xf() / 64.0f, backPt.yf() / 64.0f, 0.0f));
                quads.push_back(LFPoint3(frontPt.xf() / 64.0f, frontPt.yf() / 64.0f, -_depth));
            }
            else
            {
                quads.push_back(LFPoint3(backPt.xf() / 64.0f, backPt.yf() / 64.0f, -_depth));
                quads.push_back(LFPoint3(frontPt.xf() / 64.0f, frontPt.yf() / 64.0f, 0.0f));
            }
        }

        tol += quads.size();
        aword.push_back(quads);
        quads.clear();
    }

    return tol;
}

unsigned int CLFreetype::make(A_CHAEACTER &aword, FTGL_DOUBLE znormal, int outsettype, float outsetsize)
{
    char filename[128] = {0};
    sprintf(filename, "/Users/baidu/myfile2.txt_%d", outsettype);
    _ofile.open(filename); //作为输出文件打开
    unsigned int tol = 0;
    for (size_t c = 0; c < _contour_current_num; ++c)
    {
        switch (outsettype)
        {
        case 1:
            _contour_list[c]->　build_front_outset(outsetsize);
            break;
        case 2:
            _contour_list[c]->　build_back_outset(outsetsize);
            break;
        }

        std::vector<LFPoint> vecpoint;
        const FTContour *contour = _contour_list[c];
        for (size_t p = 0; p < contour->point_count(); ++p)
        {
            const FTGL_DOUBLE *d;
            switch (outsettype)
            {
            case 1:
                d = contour->　front_point(p);
                break;
            case 2:
                d = contour->back_point(p);
                break;
            case 0:
            default:
                d = contour->point(p);
                break;
            }
            vecpoint.push_back(LFPoint(d[0], d[1]));
            _ofile << d[0] << " " << d[1] << " " << d[2] << std::endl;
        }

        _ofile << std::endl;
        tol += vecpoint.size();
        aword.push_back(vecpoint);
        vecpoint.clear();
    }
    _ofile.close();
    return tol;
}

void CLFreetype::release_word()
{
    if (nullptr == _contour_list)
    {
        _contour_current_num = 0;
        return;
    }

    for (int i = 0; i < _contour_current_num; ++i)
    {
        if (nullptr != _contour_list[i])
        {
            delete _contour_list[i];
            _contour_list[i] = nullptr;
        }
    }

    delete[] _contour_list;
    _contour_list = nullptr;
    _contour_current_num = 0;
    _beread = false;
}

bool CLFreetype::process_contours(FT_GlyphSlot &slot)
{
    FT_Outline &outline = slot->outline;
    _contour_current_num = outline.n_contours;
    outflag = outline.flags;

    //unsigned int hscale = slot->face->size->metrics.x_ppem * 64;
    //unsigned int vscale = slot->face->size->metrics.y_ppem * 64;

    short contourlength = 0;
    short start_index = 0;
    short end_index = 0;

    _contour_list = new FTContour *[_contour_current_num];
    for (int i = 0; i < _contour_current_num; ++i)
    {
        FT_Vector *pointList = &outline.points[start_index];
        char *tagList = &outline.tags[start_index];

        end_index = outline.contours[i];
        contourlength = (end_index - start_index) + 1;
        FTContour *contour = new FTContour(pointList, tagList, contourlength);
        _contour_list[i] = contour;
        start_index = end_index + 1;
    }

    // Compute each contour's parity. FIXME: see if FT_Outline_Get_Orientation
    // can do it for us.
    for (int i = 0; i < _contour_current_num; i++)
    {
        FTContour *c1 = _contour_list[i];

        // 1. Find the leftmost point.
        FTPoint leftmost(65536.0, 0.0);
        for (size_t n = 0; n < c1->point_count(); n++)
        {
            FTPoint p = c1->point(n);
            if (p.x() < leftmost.x())
            {
                leftmost = p;
            }
        }

        // 2. Count how many other contours we cross when going further to
        // the left.
        int parity = 0;
        for (int j = 0; j < _contour_current_num; j++)
        {
            if (j == i)
            {
                continue;
            }

            FTContour *c2 = _contour_list[j];
            for (size_t n = 0; n < c2->point_count(); n++)
            {
                FTPoint p1 = c2->point(n);
                FTPoint p2 = c2->point((n + 1) % c2->point_count());

                /* FIXME: combinations of >= > <= and < do not seem stable */
                if ((p1.y() < leftmost.y() && p2.y() < leftmost.y()) || (p1.y() >= leftmost.y() && p2.y() >= leftmost.y()) || (p1.x() > leftmost.x() && p2.x() > leftmost.x()))
                {
                    continue;
                }
                else if (p1.x() < leftmost.x() && p2.x() < leftmost.x())
                {
                    parity++;
                }
                else
                {
                    FTPoint a = p1 - leftmost;
                    FTPoint b = p2 - leftmost;
                    if (b.x() * a.y() > b.y() * a.x())
                    {
                        parity++;
                    }
                }
            }
        }

        // 3. Make sure the glyph has the proper parity.
        c1->　set_parity(parity);
    }

    return true;
}

bool CLFreetype::set_fontfile(const char *fontfile)
{
    _str_ttf = fontfile;
    return true;
}
void CLFreetype::set_sizeface(float fontsize)
{
    _pointsize = fontsize;
}
void CLFreetype::set_depth(float depth)
{
    _depth = depth;
}

float CLFreetype::get_depth()
{
    return _depth;
}
void CLFreetype::set_outset(float front_outset, float back_outset)
{
    _front_outset = front_outset;
    _back_outset = back_outset;
}

unsigned int CLFreetype::get_word_front(A_CHAEACTER &fontword)
{
    unsigned int rt = 0;
    if (_beread)
    {
        rt = front(fontword);
    }
    return rt;
}
unsigned int CLFreetype::get_word_back(A_CHAEACTER &backword)
{
    unsigned int rt = 0;
    if (_beread)
    {
        rt = back(backword);
    }
    return rt;
}
unsigned int CLFreetype::get_word_side(A_CHAEACTER_QUAD &sideword)
{
    unsigned int rt = 0;
    if (_beread)
    {
        rt = side(sideword);
    }
    return rt;
}

bool CLFreetype::set_word(wchar_t &wch)
{
    bool rt = false;
    release_word();
    rt = analy_charater(wch);
    return rt;
}
