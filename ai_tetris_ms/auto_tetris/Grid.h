#pragma once
#ifndef GRID_CLASS
#define GRID_CLASS

#include"Tetris.h"
#include <vector>
using std::vector;
class Tetris;
// ������
typedef vector<BYTE> ROWSPAN; 
typedef vector<ROWSPAN> GRID;
class Grid
{
public:
	GRID grid;

	Grid();
	Grid(int row, int col) ;
	~Grid();
	// �����±�����
	ROWSPAN & operator [](int index);
	Grid & operator = (const Grid &);
	// ����ͼ��λ���жϵ�ǰλ���Ƿ�ռ��
	BOOL isExist(Tetris tetris);
	// ����ǰͼ��������ӽ�����
	void addCurTerisToGrid(Tetris tetris);
	// ������ �� return ����
	int clearLine();
	// �жϸ���״̬, -1���� 0����� 1����
	int lineStatus(int);
	// ��ʼ��
	void init(int ,int);
};

#endif // !GRID_CLASS

