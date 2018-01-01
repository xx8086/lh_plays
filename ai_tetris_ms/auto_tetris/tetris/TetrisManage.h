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

                                       /* *************函数定义************* */
    void InitWndPostionSize(HWND);  // 初始化窗体的位置和大小
    void InitGame(HWND);			  // 启动游戏
    void DrawBackGround(HDC);		  // 绘制背景
    void DrawInfo(HDC);				  // 绘制信息栏
    void DrawTetris(HDC);			  // 绘制下落图形/信息
    void checkTetris(HWND, unsigned int);		      // 检查图形移动是否越界 -> 下落结束
    void finishDown(HWND);			  // 下落结束: 更换当前图形，生成新图形 ， 消除
    void setLevel(HWND);				  // 调整游戏难度

private:
                                          // 根据坐标位置获取节点像素位置
    RectPosition getRectPosition(BYTE, BYTE);
private:
    Timer _timer;
    unsigned int iCurDif = SIMPLE; // 当前游戏难度
    unsigned int iCurScore = 0;	   // 当前得分
                                   /* 游戏区域栅栏 ,值表示对应颜色枚举类型*/
                                   // BYTE btCurGrid[PAINT_YNODE][GAME_XNODE];
    Grid btCurGrid;
    /*
    * 当前下落图形 (x,y) 关于(a,b)顺时针旋转90度后为 (y-b+a,a+b-x)
    * 在20 * 10的网格中 定义图形类，包含四个点坐标与颜色信息
    */
    Tetris curTetris;
    Tetris nextTetris;
    unsigned int iInterval = DEFAULT_INTERVAL; // 下落时间间隔

    // 游戏区坐标
    const RectPosition GAME_POSITION;
    // 信息区坐标
    const RectPosition INFO_POSITION;
    // 信息区 - 下一个文字坐标
    const RectPosition NEXT_FONT_POSITION;
    // 信息区 - 下一个图形坐标
    const RectPosition NEXT_TETRIS_POSITION;
    // 信息区 - 难度文字坐标
    const RectPosition LEVEL_FONT_POSITION;
    // 信息区 - 得分文字坐标
    const RectPosition SCORE_FONT_POSITION;

};