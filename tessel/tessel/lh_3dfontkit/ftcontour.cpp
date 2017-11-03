//
//  ftcontour.cpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/31.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#include "ftcontour.hpp"
#include <math.h>

static const unsigned int BEZIER_STEPS = 5;

void FTContour::　add_point(FTPoint point)
{
    if (pointList.empty() || (point != pointList[pointList.size() - 1] && point != pointList[0]))
    {
        pointList.push_back(point);
    }
}

void FTContour::　add_outset_point(FTPoint point)
{
    outsetPointList.push_back(point);
}

void FTContour::　add_front_point(FTPoint point)
{
    　front_pointList.push_back(point);
}

void FTContour::　add_back_point(FTPoint point)
{
    backPointList.push_back(point);
}

void FTContour::　evaluate_quadratic_curve(FTPoint A, FTPoint B, FTPoint C)
{
    for (unsigned int i = 1; i < BEZIER_STEPS; i++)
    {
        float t = static_cast<float>(i) / BEZIER_STEPS;

        FTPoint U = (1.0f - t) * A + t * B;
        FTPoint V = (1.0f - t) * B + t * C;

        　add_point((1.0f - t) * U + t * V);
    }
}

void FTContour::　evaluate_cubic_curve(FTPoint A, FTPoint B, FTPoint C, FTPoint D)
{
    for (unsigned int i = 0; i < BEZIER_STEPS; i++)
    {
        float t = static_cast<float>(i) / BEZIER_STEPS;

        FTPoint U = (1.0f - t) * A + t * B;
        FTPoint V = (1.0f - t) * B + t * C;
        FTPoint W = (1.0f - t) * C + t * D;

        FTPoint M = (1.0f - t) * U + t * V;
        FTPoint N = (1.0f - t) * V + t * W;

        　add_point((1.0f - t) * M + t * N);
    }
}

// This function is a bit tricky. Given a path ABC, it returns the
// coordinates of the outset point facing B on the left at a distance
// of 64.0.
//                                         M
//                            - - - - - - X
//                             ^         / '
//                             | 64.0   /   '
//  X---->-----X     ==>    X--v-------X     '
// A          B \          A          B \   .>'
//               \                       \<'  64.0
//                \                       \                  .
//                 \                       \                 .
//                C X                     C X
//
FTPoint FTContour::　compute_outset_point(FTPoint A, FTPoint B, FTPoint C)
{
    /* Build the rotation matrix from 'ba' vector */
    FTPoint ba = (A - B).normalise();
    FTPoint bc = C - B;

    /* Rotate bc to the left */
    FTPoint tmp(bc.x() * -ba.x() + bc.y() * -ba.y(),
                bc.x() * ba.y() + bc.y() * -ba.x());

    /* Compute the vector bisecting 'abc' */
    FTGL_DOUBLE norm = sqrt(tmp.x() * tmp.x() + tmp.y() * tmp.y());
    FTGL_DOUBLE dist = 64.0 * sqrt((norm - tmp.x()) / (norm + tmp.x()));
    tmp.x(tmp.y() < 0.0 ? dist : -dist);
    tmp.y(64.0);

    /* Rotate the new bc to the right */
    return FTPoint(tmp.x() * -ba.x() + tmp.y() * ba.y(),
                   tmp.x() * -ba.y() + tmp.y() * -ba.x());
}

void FTContour::　set_parity(int parity)
{
    size_t size = point_count();
    FTPoint vOutset;

    if (((parity & 1) && clockwise) || (!(parity & 1) && !clockwise))
    {
        // Contour orientation is wrong! We must reverse all points.
        // FIXME: could it be worth writing FTVector::reverse() for this?
        for (size_t i = 0; i < size / 2; i++)
        {
            FTPoint tmp = pointList[i];
            pointList[i] = pointList[size - 1 - i];
            pointList[size - 1 - i] = tmp;
        }

        clockwise = !clockwise;
    }

    for (size_t i = 0; i < size; i++)
    {
        size_t prev;
        size_t cur;
        size_t next;

        prev = (i + size - 1) % size;
        cur = i;
        next = (i + size + 1) % size;

        vOutset = 　compute_outset_point(point(prev), point(cur), point(next));
        　add_outset_point(vOutset);
    }
}

FTContour::FTContour(FT_Vector *contour, char *tags, unsigned int n)
{
    FTPoint prev;
    FTPoint cur(contour[(n - 1) % n]);
    FTPoint next(contour[0]);
    FTPoint a;
    FTPoint b = next - cur;
    double olddir = 0.0;
    double dir = atan2((next - cur).y(), (next - cur).x());
    double angle = 0.0;

    // See http://freetype.sourceforge.net/freetype2/docs/glyphs/glyphs-6.html
    // for a full description of FreeType tags.
    for (unsigned int i = 0; i < n; i++)
    {
        prev = cur;
        cur = next;
        next = FTPoint(contour[(i + 1) % n]);
        olddir = dir;
        dir = atan2((next - cur).y(), (next - cur).x());

        // Compute our path's new direction.
        double t = dir - olddir;
        if (t < -M_PI)
            t += 2 * M_PI;
        if (t > M_PI)
            t -= 2 * M_PI;
        angle += t;

        // Only process point tags we know.
        if (n < 2 || FT_CURVE_TAG(tags[i]) == FT_Curve_Tag_On)
        {
            　add_point(cur);
        }
        else if (FT_CURVE_TAG(tags[i]) == FT_Curve_Tag_Conic)
        {
            FTPoint prev2 = prev;
            FTPoint next2 = next;

            // Previous point is either the real previous point (an "on"
            // point), or the midpoint between the current one and the
            // previous "conic off" point.
            if (FT_CURVE_TAG(tags[(i - 1 + n) % n]) == FT_Curve_Tag_Conic)
            {
                prev2 = (cur + prev) * 0.5;
                　add_point(prev2);
            }

            // Next point is either the real next point or the midpoint.
            if (FT_CURVE_TAG(tags[(i + 1) % n]) == FT_Curve_Tag_Conic)
            {
                next2 = (cur + next) * 0.5;
            }

            　evaluate_quadratic_curve(prev2, cur, next2);
        }
        else if (FT_CURVE_TAG(tags[i]) == FT_Curve_Tag_Cubic && FT_CURVE_TAG(tags[(i + 1) % n]) == FT_Curve_Tag_Cubic)
        {
            　evaluate_cubic_curve(prev, cur, next,
                               FTPoint(contour[(i + 2) % n]));
        }
    }

    // If final angle is positive (+2PI), it's an anti-clockwise contour,
    // otherwise (-2PI) it's clockwise.
    clockwise = (angle < 0.0);
}

void FTContour::　build_front_outset(FTPoint offset, float scale)
{
    for (size_t i = 0; i < point_count(); ++i)
    {
        const FTPoint &cp = point(i);
        const FTPoint &out = outset(i);
        　add_front_point((cp + out + offset) * scale);
        //　add_front_point(point(i) + outset(i) * outset);
    }
}

void FTContour::　build_back_outset(FTPoint offset, float scale)
{
    for (size_t i = 0; i < point_count(); ++i)
    {
        　add_back_point((point(i) + outset(i) + offset) * scale);
    }
}
