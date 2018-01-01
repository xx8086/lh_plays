#include "TetrtiAI.h"

#ifndef useai
ai_tetrti::ai_tetrti(): algoGrid(PAINT_YNODE, GAME_XNODE)
{
}


ai_tetrti::~ai_tetrti()
{
}



// 函数：启动AI模式
void ai_tetrti::AIHandle(HWND hWnd)
{
    // Pierre Dellacherie 获取最佳落脚点
    getBestPosition();
    // 自动移动到最佳位置并结束下落
    aiMoveTetris(hWnd);
}
// 获取最佳评分及其位置
void ai_tetrti::getBestPosition()
{
    bestScore = 0x80000000;
    int maxX, minX, maxY, minY;
    algoTetris = curTetris;
    Tetris temp;
    //获取网格水平面
    getHorizonIndex();
    // 图形有四种状态
    for (int i = 0; i < 4; i++)
    {
        algoTetris.getMaxMin(minX, maxX, minY, maxY);
        int num = 10 - (maxY - minY); // 每次变换产生num种情况
        for (int j = 0; j < num; j++)
        {
            // 还原网格
            algoGrid = btCurGrid;
            temp = algoTetris;
            // 将base_tetris放到algoGrid的指定位置中, 返回需要向左移动次数
            int moveLeft = putTetrisToGrid(j);
            // 评分
            int score = Dellacherie();
            if (score > bestScore)
            {
                bestScore = score;
                bestTetris = algoTetris;
                needRotate = i;
                needMoveLeft = moveLeft;
            }
            // 还原 algoTetri
            algoTetris = temp;
        }
        algoTetris.rotate();
    }
}
void CALLBACK ai_tetrti::AITimeProc(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
{
    if (needRotate)
    {
        // 先变形
        needRotate--;
        curTetris.rotate();
    }
    else if (needMoveLeft > 0)
    {
        // 左移动
        curTetris.left();
        needMoveLeft--;
    }
    else if (needMoveLeft < 0)
    {
        // 右移动
        curTetris.right();
        needMoveLeft++;
    }
    // 重新绘制界面
    InvalidateRect(hWnd, NULL, TRUE);
    // 结束定时器
    if (needRotate == 0 && needMoveLeft == 0)
    {
        KillTimer(hWnd, AI_TIMER_ID);
    }
}
// 快速测试用定时器
void CALLBACK ai_tetrti::TESTTimeProc(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
{
    // 更新btCurGrid当前界面数组
    btCurGrid.addCurTerisToGrid(bestTetris);
    // 更新下落方块，重新生成nextTetris
    curTetris = nextTetris;
    nextTetris.random();
    // 消除
    iCurScore += btCurGrid.clearLine();
    // 判断生成位置是否被占用
    if (btCurGrid.isExist(curTetris))
    {
        // TODO 游戏结束
        KillTimer(hWnd, 3);
        MessageBox(hWnd, TEXT("游戏结束"), TEXT("消息"), MB_OK);
        return;
    }
    // 如果为AI模式，继续计算
    AIHandle(hWnd);
}
// 将方块移动到最佳位置
void ai_tetrti::aiMoveTetris(HWND hWnd)
{
    SetTimer(hWnd, AI_TIMER_ID, iInterval / 5, AITimeProc);
    // SetTimer(hWnd, 3, 0 ,TESTTimeProc); // 快速测试使用
}
// 获取网格水平线
void ai_tetrti::getHorizonIndex()
{
    for (int i = 0; i < GAME_XNODE; i++)
    {
        for (int j = 0; j < PAINT_YNODE - 1; j++)
        {
            if (btCurGrid[j + 1][i])
            {
                horizonIndex[i] = j;
                break;
            }
            if (j == (PAINT_YNODE - 2))
            {
                horizonIndex[i] = j + 1;
            }
        }
    }

}

int ai_tetrti::putTetrisToGrid(int col)
{
    int left_bottom_index = algoTetris.getLeftBottom(); // 获取最左下角的元素下标

    int maxDistanceRow = horizonIndex[col] - algoTetris.positions[left_bottom_index].x; // 需要向下移动的距离
    int maxDistanceCol = algoTetris.positions[left_bottom_index].y - col; // 需要向左移动的距离
                                                                          // 调整向下移动距离
    for (int j = 0; j < 4; j++)
    {
        if (j != left_bottom_index)
        {
            int colDistance = algoTetris.positions[j].y - algoTetris.positions[left_bottom_index].y;
            int rowDistance = horizonIndex[col + colDistance] - algoTetris.positions[j].x;
            if (rowDistance < maxDistanceRow)
            {
                maxDistanceRow = rowDistance;
            }
        }
    }
    // 将algoTetris移动到指定位位置
    for (int j = 0; j < 4; j++)
    {
        algoTetris.positions[j].x += maxDistanceRow;
        algoTetris.positions[j].y -= maxDistanceCol;
        algoGrid[algoTetris.positions[j].x][algoTetris.positions[j].y] = algoTetris.color;
    }
    return maxDistanceCol;
}
// AI评分算法
int ai_tetrti::Dellacherie()
{
    INT lh_score, re_score, rt_score, ct_score, nh_score, ws_score;
    lh_score = getLH();
    re_score = getRE();
    rt_score = getRT();
    ct_score = getCT();
    nh_score = getNH();
    ws_score = getWS();
    return lh_score * LH_WEIGHT + re_score * RE_WEIGHT + rt_score * RT_WEIGHT + ct_score * CT_WEIGHT + nh_score * NH_WEIGHT + ws_score * WS_WEIGHT;
}
// 高度评分
INT ai_tetrti::getLH()
{
    int maxX, minX, maxY, minY;
    algoTetris.getMaxMin(minX, maxX, minY, maxY);
    return PAINT_YNODE - maxX;
}
// 消除行*贡献方块数评分
int ai_tetrti::getRE()
{
    int  status, clearline = 0, cells = 0;

    for (int i = PAINT_YNODE - 1; i >= 0; i--)
    {
        status = algoGrid.lineStatus(i);
        if (status == 0)
        {
            continue;
        }
        if (status == 1)
        {
            // 满
            clearline++; // 行+1
                         // 此行贡献方块数
            for (int j = 0; j < 4; j++)
            {
                if (algoTetris.positions[j].x == i)
                {
                    cells++;
                }
            }
        }
        else
        {
            // 空
            break;
        }
    }
    return clearline * cells;
}
// 行变化率评分
int ai_tetrti::getRT()
{
    // TODO 可以优化，遇到空行，以上行直接每行+2
    int lineChane = 0, lastStatus;
    for (int i = PAINT_YNODE - 1; i >= 0; i--)
    {
        lastStatus = 1; // 左墙壁状态为有效，当作存在图形
        for (int j = 0; j < GAME_XNODE; j++)
        {
            // 与上一个格子不同
            if (!(lastStatus * algoGrid[i][j]) && lastStatus != algoGrid[i][j])
            {
                lineChane++;
                lastStatus = algoGrid[i][j];
            }
            // 当前行最后一个节点为空
            if (j == (GAME_XNODE - 1) && algoGrid[i][j] == 0)
            {
                lineChane++;
            }
        }
    }
    return lineChane;
}
// 列变化率评分
INT ai_tetrti::getCT()
{
    int colChane = 0, lastStatus;
    for (int i = 0; i < GAME_XNODE; i++)
    {
        lastStatus = 1; // 左墙壁状态为有效，当作存在图形
        for (int j = 0; j < PAINT_YNODE; j++)
        {
            // 与上一个格子不同
            if (!(lastStatus * algoGrid[j][i]) && lastStatus != algoGrid[j][i])
            {
                colChane++;
                lastStatus = algoGrid[j][i];
            }
            // 当前行最后一个节点为空
            if (j == (PAINT_YNODE - 1) && algoGrid[j][i] == 0)
            {
                colChane++;
            }
        }
    }
    return colChane;
}
// 空洞数评分
INT ai_tetrti::getNH()
{
    int holes = 0;
    bool holeFlag = false; // 是否可以开始计算空洞
    for (int i = 0; i < GAME_XNODE; i++)
    {
        holeFlag = false;
        for (int j = 0; j < PAINT_YNODE; j++)
        {
            if (!holeFlag)
            {
                if (algoGrid[j][i])
                {
                    holeFlag = true;
                }
            }
            else
            {
                if (algoGrid[j][i] == 0)
                {
                    holes++;
                }
            }
        }
    }
    return holes;
}
// 井深评分
INT ai_tetrti::getWS()
{
    // TODO: 井深求解不精确
    int wells = 0;
    int center, left, right;
    for (int i = 0; i < PAINT_YNODE; i++)
    {
        for (int j = 0; j < GAME_XNODE; j++)
        {

            center = algoGrid[i][j];
            if (j > 0)
            {
                left = algoGrid[i][j - 1];
            }
            else
            {
                left = 1;
            }
            if (j < GAME_XNODE - 1)
            {
                right = algoGrid[i][j + 1];
            }
            else
            {
                right = 1;
            }
            // 是否为井
            if (left && right && !center)
            {
                for (int k = i; k < PAINT_YNODE; k++)
                {
                    if (algoGrid[k][j] == 0)
                    {
                        wells++;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
    return wells;
}

#endif