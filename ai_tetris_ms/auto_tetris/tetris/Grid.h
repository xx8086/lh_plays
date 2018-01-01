#pragma once
#ifndef GRID_CLASS
#define GRID_CLASS

#include"Tetris.h"
#include <vector>
using std::vector;
class Tetris;
// 网格类
typedef std::vector<char> ROWSPAN; 
typedef std::vector<ROWSPAN> GRID;
class Grid
{
public:
	GRID grid;

	Grid();
	Grid(unsigned int row, unsigned int col) ;
	~Grid();
	// 重载下标运算
	ROWSPAN & operator [](unsigned int index);
	Grid & operator = (const Grid &);
	// 根据图形位置判断当前位置是否被占用
	bool isExist(Tetris tetris);
	// 将当前图形坐标添加进网格
	void addCurTerisToGrid(Tetris tetris);
	// 消除行 ， return 行数
    unsigned int clearLine();
	// 判断该行状态, -1：空 0：混合 1：满
    unsigned int lineStatus(unsigned int);
	// 初始化
	void init(unsigned int , unsigned int);
};

#endif // !GRID_CLASS

