#pragma once
#ifndef GRID_CLASS
#define GRID_CLASS

#include"Tetris.h"
#include <vector>
using std::vector;
class Tetris;
// ������
typedef std::vector<char> ROWSPAN; 
typedef std::vector<ROWSPAN> GRID;
class Grid
{
public:
	GRID grid;

	Grid();
	Grid(unsigned int row, unsigned int col) ;
	~Grid();
	// �����±�����
	ROWSPAN & operator [](unsigned int index);
	Grid & operator = (const Grid &);
	// ����ͼ��λ���жϵ�ǰλ���Ƿ�ռ��
	bool isExist(Tetris tetris);
	// ����ǰͼ��������ӽ�����
	void addCurTerisToGrid(Tetris tetris);
	// ������ �� return ����
    unsigned int clearLine();
	// �жϸ���״̬, -1���� 0����� 1����
    unsigned int lineStatus(unsigned int);
	// ��ʼ��
	void init(unsigned int , unsigned int);
};

#endif // !GRID_CLASS

