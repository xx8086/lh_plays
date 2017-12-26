#pragma once

#include "resource.h"
#include "class.h"


/* *************�궨��************* */
#define BLOCK_SIZE 10 // ����
#define NODE_SIZE 30 // ÿ���ڵ��СΪ10
#define GAME_XNODE 10 // ��Ϸ��X��10���ڵ�
#define INFO_XNODE 6 // ��Ϣչʾ�����5���ڵ�
#define PAINT_YNODE 20 //Y��20���ڵ�
#define PAINT_WIDTH ((GAME_XNODE+INFO_XNODE) * NODE_SIZE +  3 * BLOCK_SIZE) // ���ڿ��
#define PAINT_HEIGHT (PAINT_YNODE * NODE_SIZE + 2 * BLOCK_SIZE)             // ���ڸ߶�

#define TIMER_ID 1
#define AI_TIMER_ID 2

/* *************ȫ�ֳ���************* */
// ��Ϸ�Ѷ�
CONST enum GAME_DIF
{
	SIMPLE = 1,
	GENERAL,
	DIFFICULT,
	DIFFICULTEST
};
CONST UINT GENERAL_SCORE = 5; // �е��Ѷȶ�Ӧ��������
CONST UINT DIFFICULT_SCORE = 10; // ���Ѷ�Ӧ��������
CONST UINT DIFFICULTEST_SCORE = 15; // �ر����Ѷ�Ӧ��������
// ��Ϸ������
CONST RectPosition GAME_POSITION(BLOCK_SIZE, BLOCK_SIZE,BLOCK_SIZE + NODE_SIZE * GAME_XNODE, BLOCK_SIZE + NODE_SIZE * PAINT_YNODE);
// ��Ϣ������
CONST RectPosition INFO_POSITION(BLOCK_SIZE*2 + NODE_SIZE * GAME_XNODE, BLOCK_SIZE,PAINT_WIDTH - BLOCK_SIZE, PAINT_HEIGHT - BLOCK_SIZE);
// ��Ϣ�� - ��һ����������
CONST RectPosition NEXT_FONT_POSITION(BLOCK_SIZE*2 + NODE_SIZE * GAME_XNODE + BLOCK_SIZE, BLOCK_SIZE * 2,PAINT_WIDTH - BLOCK_SIZE*2, BLOCK_SIZE * 2 + 2 * NODE_SIZE);
// ��Ϣ�� - ��һ��ͼ������
CONST RectPosition NEXT_TETRIS_POSITION(BLOCK_SIZE*2 + NODE_SIZE * GAME_XNODE + NODE_SIZE , BLOCK_SIZE * 2 + 2 * NODE_SIZE, PAINT_WIDTH - BLOCK_SIZE - NODE_SIZE , BLOCK_SIZE * 2 + NODE_SIZE * 6);
// ��Ϣ�� - �Ѷ���������
CONST RectPosition LEVEL_FONT_POSITION(NODE_SIZE * GAME_XNODE + NODE_SIZE * 2, BLOCK_SIZE * 2 + 10 * NODE_SIZE, PAINT_WIDTH - BLOCK_SIZE - NODE_SIZE, BLOCK_SIZE * 2 + 11 * NODE_SIZE);
// ��Ϣ�� - �÷���������
CONST RectPosition SCORE_FONT_POSITION(NODE_SIZE * GAME_XNODE + NODE_SIZE * 2, BLOCK_SIZE * 2 + 11 * NODE_SIZE,PAINT_WIDTH - BLOCK_SIZE - NODE_SIZE, BLOCK_SIZE * 2 + 12 * NODE_SIZE);

CONST UINT DEFAULT_INTERVAL = 1000; // Ĭ������ʱ����

/* *************ȫ�ֱ���************* */
UINT iCurDif = SIMPLE; // ��ǰ��Ϸ�Ѷ�
UINT iCurScore = 0;	   // ��ǰ�÷�
/* ��Ϸ����դ�� ,ֵ��ʾ��Ӧ��ɫö������*/
// BYTE btCurGrid[PAINT_YNODE][GAME_XNODE];
Grid btCurGrid(PAINT_YNODE, GAME_XNODE);
/*  
* ��ǰ����ͼ�� (x,y) ����(a,b)˳ʱ����ת90�Ⱥ�Ϊ (y-b+a,a+b-x) 
* ��20 * 10�������� ����ͼ���࣬�����ĸ�����������ɫ��Ϣ
*/
Tetris curTetris, nextTetris;
UINT iInterval = DEFAULT_INTERVAL; // ����ʱ����

/* *************��������************* */
VOID InitWndPostionSize(HWND);  // ��ʼ�������λ�úʹ�С
VOID InitGame(HWND);			  // ������Ϸ
VOID DrawBackGround(HDC);		  // ���Ʊ���
VOID DrawInfo(HDC);				  // ������Ϣ��
VOID DrawTetris(HDC);			  // ��������ͼ��/��Ϣ
VOID checkTetris(HWND,INT);		      // ���ͼ���ƶ��Ƿ�Խ�� -> �������
VOID finishDown(HWND);			  // �������: ������ǰͼ�Σ�������ͼ�� �� ����
VOID setLevel(HWND);				  // ������Ϸ�Ѷ�
// ��������λ�û�ȡ�ڵ�����λ��
RectPosition getRectPosition(BYTE , BYTE);


/* ***************AIģʽ**************** */
BOOL AI_SWITCH = false;
UINT AI_DEFAULT_INTERVAL = 10;
// ������Ȩ��
CONST INT LH_WEIGHT = -45;
CONST INT RE_WEIGHT = 34;
CONST INT RT_WEIGHT = -32;
CONST INT CT_WEIGHT = -93;
CONST INT NH_WEIGHT = -79;
CONST INT WS_WEIGHT = -34;

Tetris bestTetris, algoTetris; // ���λ��
Grid algoGrid(PAINT_YNODE, GAME_XNODE);// ���ڼ����դ��
INT horizonIndex[GAME_XNODE]; // ��¼����ˮƽ��
INT bestScore = 0x80000000;   // �������,��ʼֵΪ��СINTֵ
INT needRotate = 0, needMoveLeft = 0; // ÿ�ε������λ����Ҫת���������ƶ�����

VOID getHorizonIndex();
VOID AIHandle(HWND); // AIģʽ��ʱ��������
VOID getBestPosition(); // ��ȡ������ּ���λ��
INT putTetrisToGrid(INT); // ��base_tetris�ŵ�algoGrid��ָ��λ����, ������Ҫ�����ƶ�����
INT Dellacherie();    // AI�㷨�������������λ��
INT getLH(); // �߶�����
INT getRE();  // ������*���׷���������
INT getRT();  // �б仯������
INT getCT();  // �б仯������
INT getNH();  // �ն�������
INT getWS();  // ��������

VOID aiMoveTetris(HWND); // �������ƶ������λ��