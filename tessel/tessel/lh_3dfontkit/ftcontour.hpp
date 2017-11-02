//
//  ftcontour.hpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/31.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#ifndef ftcontour_hpp
#define ftcontour_hpp

#include <stdio.h>
#include "ftvector.h"
#include "ftpoint.hpp"


class FTContour
{
public:
    FTContour(FT_Vector* contour, char* pointTags, unsigned int numberOfPoints);
    ~FTContour()
    {
        pointList.clear();
        outsetPointList.clear();
        frontPointList.clear();
        backPointList.clear();
    }
    
    const FTPoint& Point(size_t index) const { return pointList[index]; }
    const FTPoint& Outset(size_t index) const { return outsetPointList[index]; }
    const FTPoint& FrontPoint(size_t index) const
    {
        if(frontPointList.size() == 0)
            return Point(index);
        return frontPointList[index];
    }
    const FTPoint& BackPoint(size_t index) const
    {
        if(backPointList.size() == 0)
            return Point(index);
        return backPointList[index];
    }
    size_t PointCount() const { return pointList.size(); }
    void SetParity(int parity);
    
    // FIXME: this should probably go away.
    void buildFrontOutset(float outset);
    void buildBackOutset(float outset);
    
private:
    inline void AddPoint(FTPoint point);
    inline void AddOutsetPoint(FTPoint point);
    inline void AddFrontPoint(FTPoint point);
    inline void AddBackPoint(FTPoint point);
    inline void evaluateQuadraticCurve(FTPoint, FTPoint, FTPoint);
    inline void evaluateCubicCurve(FTPoint, FTPoint, FTPoint, FTPoint);
    inline FTPoint ComputeOutsetPoint(FTPoint a, FTPoint b, FTPoint c);
private:
    typedef FTVector<FTPoint> PointVector;
    PointVector pointList;
    PointVector outsetPointList;
    PointVector frontPointList;
    PointVector backPointList;
    bool clockwise;
};

#endif /* ftcontour_hpp */
