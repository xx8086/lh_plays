#pragma once
#ifndef RECTPOSITION_CLASS
#define RECTPOSITION_CLASS
/*
* �ࣺ ���ε����ϽǺ����½�����
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
	// ��RECT��ֵ
	void setRect(TERECT &) const;
};
#endif // !RECTPOSITION_CLASS



