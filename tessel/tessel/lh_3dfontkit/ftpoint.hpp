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

typedef double FTGL_DOUBLE;
typedef float FTGL_FLOAT;

class FTPoint
{
public:
    inline FTPoint()
    {
        _values[0] = 0;
        _values[1] = 0;
        _values[2] = 0;
    }

    inline FTPoint(const FTGL_DOUBLE x, const FTGL_DOUBLE y,
                   const FTGL_DOUBLE z = 0)
    {
        _values[0] = x;
        _values[1] = y;
        _values[2] = z;
    }

    inline FTPoint(const FT_Vector &ft_vector)
    {
        _values[0] = ft_vector.x;
        _values[1] = ft_vector.y;
        _values[2] = 0;
    }

    FTPoint normalise();
    inline FTPoint &operator+=(const FTPoint &point)
    {
        _values[0] += point._values[0];
        _values[1] += point._values[1];
        _values[2] += point._values[2];

        return *this;
    }
    inline FTPoint operator+(const FTPoint &point) const
    {
        FTPoint temp;
        temp._values[0] = _values[0] + point._values[0];
        temp._values[1] = _values[1] + point._values[1];
        temp._values[2] = _values[2] + point._values[2];

        return temp;
    }
    inline FTPoint &operator-=(const FTPoint &point)
    {
        _values[0] -= point._values[0];
        _values[1] -= point._values[1];
        _values[2] -= point._values[2];

        return *this;
    }
    inline FTPoint operator-(const FTPoint &point) const
    {
        FTPoint temp;
        temp._values[0] = _values[0] - point._values[0];
        temp._values[1] = _values[1] - point._values[1];
        temp._values[2] = _values[2] - point._values[2];

        return temp;
    }
    inline FTPoint operator*(double multiplier) const
    {
        FTPoint temp;
        temp._values[0] = _values[0] * multiplier;
        temp._values[1] = _values[1] * multiplier;
        temp._values[2] = _values[2] * multiplier;

        return temp;
    }
    inline friend FTPoint operator*(double multiplier, FTPoint &point)
    {
        return point * multiplier;
    }
    inline friend double operator*(FTPoint &a, FTPoint &b)
    {
        return a._values[0] * b._values[0] + 
        a._values[1] * b._values[1] + 
        a._values[2] * b._values[2];
    }
    inline FTPoint operator^(const FTPoint &point)
    {
        FTPoint temp;
        temp._values[0] = _values[1] * point._values[2] - _values[2] * point._values[1];
        temp._values[1] = _values[2] * point._values[0] - _values[0] * point._values[2];
        temp._values[2] = _values[0] * point._values[1] - _values[1] * point._values[0];
        return temp;
    }
    friend bool operator==(const FTPoint &a, const FTPoint &b);
    friend bool operator!=(const FTPoint &a, const FTPoint &b);
    inline operator const FTGL_DOUBLE *() const
    {
        return _values;
    }
    inline void x(FTGL_DOUBLE x)
    {
        _values[0] = x;
    };
    inline void y(FTGL_DOUBLE y)
    {
        _values[1] = y;
    };
    inline void z(FTGL_DOUBLE z)
    {
        _values[2] = z;
    };
    inline FTGL_DOUBLE x() const
    {
        return _values[0];
    };
    inline FTGL_DOUBLE y() const
    {
        return _values[1];
    };
    inline FTGL_DOUBLE z() const
    {
        return _values[2];
    };
    inline FTGL_FLOAT xf() const
    {
        return static_cast<FTGL_FLOAT>(_values[0]);
    };
    inline FTGL_FLOAT yf() const
    {
        return static_cast<FTGL_FLOAT>(_values[1]);
    };
    inline FTGL_FLOAT zf() const
    {
        return static_cast<FTGL_FLOAT>(_values[2]);
    };

private:
    FTGL_DOUBLE _values[3];
};

#endif /* ftpoint_hpp */
