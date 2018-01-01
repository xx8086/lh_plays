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
    int horizonIndex[GAME_XNODE]; // ��¼����ˮƽ��
    int bestScore = 0x80000000;   // �������,��ʼֵΪ��СINTֵ
    int needRotate = 0, needMoveLeft = 0; // ÿ�ε������λ����Ҫת���������ƶ�����

    void getHorizonIndex();
    void AIHandle(HWND); // AIģʽ��ʱ��������
    void getBestPosition(); // ��ȡ������ּ���λ��
    int putTetrisToGrid(int); // ��base_tetris�ŵ�algoGrid��ָ��λ����, ������Ҫ�����ƶ�����
    int Dellacherie();    // AI�㷨�������������λ��
    int getLH(); // �߶�����
    int getRE();  // ������*���׷���������
    int getRT();  // �б仯������
    int getCT();  // �б仯������
    int getNH();  // �ն�������
    int getWS();  // ��������

    void aiMoveTetris(HWND); // �������ƶ������λ��
    void TESTTimeProc(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime);
private:
    Tetris bestTetris;
    Tetris algoTetris; // ���λ��
    Grid algoGrid;// ���ڼ����դ��
};

#else 
#endif

