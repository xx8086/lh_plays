#pragma once
#ifndef RECTPOSITION_CLASS
#define RECTPOSITION_CLASS
/*
* 类： 矩形的左上角和右下角坐标
*/
#include "tetri_type.h"

class RectPosition {
public:
	long iLeftX;
	long iLeftY;
	long iRightX;
	long iRightY;

	RectPosition();
	RectPosition(long, long, long, long);
	~RectPosition();
	// 给RECT赋值
	void setRect(TERECT &) const;
};
#endif // !RECTPOSITION_CLASS



