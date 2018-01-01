#pragma once
#include "tetri_type.h"
#include"RectPosition.h"
#include"GridPosition.h"
#include"Tetris.h"
#include"Grid.h"
#include "TetrtiAI.h"
#include "../common/Timer.h"

class CTeteisManage {
public:
    CTeteisManage();
    ~CTeteisManage();
private:

public:
    static void CALLBACK TimeProc(CTeteisManage* _this, HWND hWnd);

                                       /* *************��������************* */
    void InitWndPostionSize(HWND);  // ��ʼ�������λ�úʹ�С
    void InitGame(HWND);			  // ������Ϸ
    void DrawBackGround(HDC);		  // ���Ʊ���
    void DrawInfo(HDC);				  // ������Ϣ��
    void DrawTetris(HDC);			  // ��������ͼ��/��Ϣ
    void checkTetris(HWND, unsigned int);		      // ���ͼ���ƶ��Ƿ�Խ�� -> �������
    void finishDown(HWND);			  // �������: ������ǰͼ�Σ�������ͼ�� �� ����
    void setLevel(HWND);				  // ������Ϸ�Ѷ�

private:
                                          // ��������λ�û�ȡ�ڵ�����λ��
    RectPosition getRectPosition(BYTE, BYTE);
private:
    Timer _timer;
    unsigned int iCurDif = SIMPLE; // ��ǰ��Ϸ�Ѷ�
    unsigned int iCurScore = 0;	   // ��ǰ�÷�
                                   /* ��Ϸ����դ�� ,ֵ��ʾ��Ӧ��ɫö������*/
                                   // BYTE btCurGrid[PAINT_YNODE][GAME_XNODE];
    Grid btCurGrid;
    /*
    * ��ǰ����ͼ�� (x,y) ����(a,b)˳ʱ����ת90�Ⱥ�Ϊ (y-b+a,a+b-x)
    * ��20 * 10�������� ����ͼ���࣬�����ĸ�����������ɫ��Ϣ
    */
    Tetris curTetris;
    Tetris nextTetris;
    unsigned int iInterval = DEFAULT_INTERVAL; // ����ʱ����

    // ��Ϸ������
    const RectPosition GAME_POSITION;
    // ��Ϣ������
    const RectPosition INFO_POSITION;
    // ��Ϣ�� - ��һ����������
    const RectPosition NEXT_FONT_POSITION;
    // ��Ϣ�� - ��һ��ͼ������
    const RectPosition NEXT_TETRIS_POSITION;
    // ��Ϣ�� - �Ѷ���������
    const RectPosition LEVEL_FONT_POSITION;
    // ��Ϣ�� - �÷���������
    const RectPosition SCORE_FONT_POSITION;

};