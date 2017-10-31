//
//  ftpoint.hpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/31.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#ifndef ftpoint_hpp
#define ftpoint_hpp

#include "freetype/ftimage.h"

typedef double   FTGL_DOUBLE;
typedef float    FTGL_FLOAT;

class FTPoint
{
public:
    inline FTPoint()
    {
        values[0] = 0;
        values[1] = 0;
        values[2] = 0;
    }
    
    inline FTPoint(const FTGL_DOUBLE x, const FTGL_DOUBLE y,
                   const FTGL_DOUBLE z = 0)
    {
        values[0] = x;
        values[1] = y;
        values[2] = z;
    }
    
    inline FTPoint(const FT_Vector& ft_vector)
    {
        values[0] = ft_vector.x;
        values[1] = ft_vector.y;
        values[2] = 0;
    }
    
    FTPoint Normalise();
    inline FTPoint& operator += (const FTPoint& point)
    {
        values[0] += point.values[0];
        values[1] += point.values[1];
        values[2] += point.values[2];
        
        return *this;
    }
    inline FTPoint operator + (const FTPoint& point) const
    {
        FTPoint temp;
        temp.values[0] = values[0] + point.values[0];
        temp.values[1] = values[1] + point.values[1];
        temp.values[2] = values[2] + point.values[2];
        
        return temp;
    }
    inline FTPoint& operator -= (const FTPoint& point)
    {
        values[0] -= point.values[0];
        values[1] -= point.values[1];
        values[2] -= point.values[2];
        
        return *this;
    }
    inline FTPoint operator - (const FTPoint& point) const
    {
        FTPoint temp;
        temp.values[0] = values[0] - point.values[0];
        temp.values[1] = values[1] - point.values[1];
        temp.values[2] = values[2] - point.values[2];
        
        return temp;
    }
    inline FTPoint operator * (double multiplier) const
    {
        FTPoint temp;
        temp.values[0] = values[0] * multiplier;
        temp.values[1] = values[1] * multiplier;
        temp.values[2] = values[2] * multiplier;
        
        return temp;
    }
    inline friend FTPoint operator * (double multiplier, FTPoint& point)
    {
        return point * multiplier;
    }
    inline friend double operator * (FTPoint &a, FTPoint& b)
    {
        return a.values[0] * b.values[0]
        + a.values[1] * b.values[1]
        + a.values[2] * b.values[2];
    }
    inline FTPoint operator ^ (const FTPoint& point)
    {
        FTPoint temp;
        temp.values[0] = values[1] * point.values[2]
        - values[2] * point.values[1];
        temp.values[1] = values[2] * point.values[0]
        - values[0] * point.values[2];
        temp.values[2] = values[0] * point.values[1]
        - values[1] * point.values[0];
        return temp;
    }
    friend bool operator == (const FTPoint &a, const FTPoint &b);
    friend bool operator != (const FTPoint &a, const FTPoint &b);
    inline operator const FTGL_DOUBLE*() const
    {
        return values;
    }
    inline void X(FTGL_DOUBLE x) { values[0] = x; };
    inline void Y(FTGL_DOUBLE y) { values[1] = y; };
    inline void Z(FTGL_DOUBLE z) { values[2] = z; };
    inline FTGL_DOUBLE X() const { return values[0]; };
    inline FTGL_DOUBLE Y() const { return values[1]; };
    inline FTGL_DOUBLE Z() const { return values[2]; };
    inline FTGL_FLOAT Xf() const { return static_cast<FTGL_FLOAT>(values[0]); };
    inline FTGL_FLOAT Yf() const { return static_cast<FTGL_FLOAT>(values[1]); };
    inline FTGL_FLOAT Zf() const { return static_cast<FTGL_FLOAT>(values[2]); };
    
private:
    FTGL_DOUBLE values[3];
};

#endif /* ftpoint_hpp */
