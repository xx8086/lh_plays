#pragma once


#define useai
#ifndef useai
useai
#include "tetri_type.h"
#include"RectPosition.h"
#include"GridPosition.h"
#include"Tetris.h"
#include"Grid.h"



class ai_tetrti
{
public:
    ai_tetrti();
    ~ai_tetrti();
public:
    int horizonIndex[GAME_XNODE]; // 记录网格水平面
    int bestScore = 0x80000000;   // 最佳评分,初始值为最小INT值
    int needRotate = 0, needMoveLeft = 0; // 每次到达最佳位置需要转动、向左移动次数

    void getHorizonIndex();
    void AIHandle(HWND); // AI模式定时器处理函数
    void getBestPosition(); // 获取最佳评分及其位置
    int putTetrisToGrid(int); // 将base_tetris放到algoGrid的指定位置中, 返回需要向左移动次数
    int Dellacherie();    // AI算法：计算最佳评分位置
    int getLH(); // 高度评分
    int getRE();  // 消除行*贡献方块数评分
    int getRT();  // 行变化率评分
    int getCT();  // 列变化率评分
    int getNH();  // 空洞数评分
    int getWS();  // 井深评分

    void aiMoveTetris(HWND); // 将方块移动到最佳位置
    void TESTTimeProc(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime);
private:
    Tetris bestTetris;
    Tetris algoTetris; // 最佳位置
    Grid algoGrid;// 用于计算的栅栏
};

#else 
#endif

