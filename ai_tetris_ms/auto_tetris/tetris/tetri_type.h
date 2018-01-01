#pragma once


#define TIMER_ID 1
#define AI_TIMER_ID 2
#include "Windows.h"
/* *************全局常量************* */
// 游戏难度
const enum GAME_DIF
{
    SIMPLE = 1,
    GENERAL,
    DIFFICULT,
    DIFFICULTEST
};
const unsigned int GENERAL_SCORE = 5; // 中等难度对应分数限制
const unsigned int DIFFICULT_SCORE = 10; // 困难对应分数限制
const unsigned int DIFFICULTEST_SCORE = 15; // 特别困难对应分数限制
const unsigned int DEFAULT_INTERVAL = 1000; // 默认下落时间间隔

const unsigned int BLOCK_SIZE = 10; // 留白
const unsigned int NODE_SIZE = 30; // 每个节点大小为10
const unsigned int GAME_XNODE = 10; // 游戏区X轴10个节点
const unsigned int INFO_XNODE = 6; // 信息展示区宽度5个节点
const unsigned int PAINT_YNODE = 20; //Y轴20个节点
const unsigned int PAINT_WIDTH = ((GAME_XNODE + INFO_XNODE) * NODE_SIZE + 3 * BLOCK_SIZE); // 窗口宽度
const unsigned int PAINT_HEIGHT = (PAINT_YNODE * NODE_SIZE + 2 * BLOCK_SIZE);             // 窗口高度

typedef RECT TERECT;