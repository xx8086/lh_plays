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
        FTContour(FT_Vector *contour, char *pointTags, unsigned int numberOfPoints);
        ~FTContour()
        {
            pointList.clear();
            outsetPointList.clear();
            front_pointList.clear();
            backPointList.clear();
        }

        const FTPoint &point(size_t index) const { return pointList[index]; }
        const FTPoint &outset(size_t index) const { return outsetPointList[index]; }
        const FTPoint &　front_point(size_t index) const
        {
            if (　front_pointList.size() == 0)
            {
                return point(index);
            }
            return 　front_pointList[index];
        }
        const FTPoint &back_point(size_t index) const
        {
            if (backPointList.size() == 0)
            {
                return point(index);
            }
            return backPointList[index];
        }
        size_t point_count() const
        {
            return pointList.size();
        }
        void 　set_parity(int parity);

        // FIXME: this should probably go away.
        void 　build_front_outset(float);
        void 　build_back_outset(float);

    private:
        inline void 　add_point(FTPoint point);
        inline void 　add_outset_point(FTPoint point);
        inline void 　add_front_point(FTPoint point);
        inline void 　add_back_point(FTPoint point);
        inline void 　evaluate_quadratic_curve(FTPoint, FTPoint, FTPoint);
        inline void 　evaluate_cubic_curve(FTPoint, FTPoint, FTPoint, FTPoint);
        inline FTPoint 　compute_outset_point(FTPoint a, FTPoint b, FTPoint c);

    private:
        typedef FTVector<FTPoint> PointVector;
        PointVector pointList;
        PointVector outsetPointList;
        PointVector 　front_pointList;
        PointVector backPointList;
        bool clockwise;
    };

#endif /* ftcontour_hpp */
