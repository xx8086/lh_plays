#include "TetrtiAI.h"

#ifndef useai
ai_tetrti::ai_tetrti(): algoGrid(PAINT_YNODE, GAME_XNODE)
{
}


ai_tetrti::~ai_tetrti()
{
}



// ����������AIģʽ
void ai_tetrti::AIHandle(HWND hWnd)
{
    // Pierre Dellacherie ��ȡ�����ŵ�
    getBestPosition();
    // �Զ��ƶ������λ�ò���������
    aiMoveTetris(hWnd);
}
// ��ȡ������ּ���λ��
void ai_tetrti::getBestPosition()
{
    bestScore = 0x80000000;
    int maxX, minX, maxY, minY;
    algoTetris = curTetris;
    Tetris temp;
    //��ȡ����ˮƽ��
    getHorizonIndex();
    // ͼ��������״̬
    for (int i = 0; i < 4; i++)
    {
        algoTetris.getMaxMin(minX, maxX, minY, maxY);
        int num = 10 - (maxY - minY); // ÿ�α任����num�����
        for (int j = 0; j < num; j++)
        {
            // ��ԭ����
            algoGrid = btCurGrid;
            temp = algoTetris;
            // ��base_tetris�ŵ�algoGrid��ָ��λ����, ������Ҫ�����ƶ�����
            int moveLeft = putTetrisToGrid(j);
            // ����
            int score = Dellacherie();
            if (score > bestScore)
            {
                bestScore = score;
                bestTetris = algoTetris;
                needRotate = i;
                needMoveLeft = moveLeft;
            }
            // ��ԭ algoTetri
            algoTetris = temp;
        }
        algoTetris.rotate();
    }
}
void CALLBACK ai_tetrti::AITimeProc(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
{
    if (needRotate)
    {
        // �ȱ���
        needRotate--;
        curTetris.rotate();
    }
    else if (needMoveLeft > 0)
    {
        // ���ƶ�
        curTetris.left();
        needMoveLeft--;
    }
    else if (needMoveLeft < 0)
    {
        // ���ƶ�
        curTetris.right();
        needMoveLeft++;
    }
    // ���»��ƽ���
    InvalidateRect(hWnd, NULL, TRUE);
    // ������ʱ��
    if (needRotate == 0 && needMoveLeft == 0)
    {
        KillTimer(hWnd, AI_TIMER_ID);
    }
}
// ���ٲ����ö�ʱ��
void CALLBACK ai_tetrti::TESTTimeProc(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
{
    // ����btCurGrid��ǰ��������
    btCurGrid.addCurTerisToGrid(bestTetris);
    // �������䷽�飬��������nextTetris
    curTetris = nextTetris;
    nextTetris.random();
    // ����
    iCurScore += btCurGrid.clearLine();
    // �ж�����λ���Ƿ�ռ��
    if (btCurGrid.isExist(curTetris))
    {
        // TODO ��Ϸ����
        KillTimer(hWnd, 3);
        MessageBox(hWnd, TEXT("��Ϸ����"), TEXT("��Ϣ"), MB_OK);
        return;
    }
    // ���ΪAIģʽ����������
    AIHandle(hWnd);
}
// �������ƶ������λ��
void ai_tetrti::aiMoveTetris(HWND hWnd)
{
    SetTimer(hWnd, AI_TIMER_ID, iInterval / 5, AITimeProc);
    // SetTimer(hWnd, 3, 0 ,TESTTimeProc); // ���ٲ���ʹ��
}
// ��ȡ����ˮƽ��
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
    int left_bottom_index = algoTetris.getLeftBottom(); // ��ȡ�����½ǵ�Ԫ���±�

    int maxDistanceRow = horizonIndex[col] - algoTetris.positions[left_bottom_index].x; // ��Ҫ�����ƶ��ľ���
    int maxDistanceCol = algoTetris.positions[left_bottom_index].y - col; // ��Ҫ�����ƶ��ľ���
                                                                          // ���������ƶ�����
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
    // ��algoTetris�ƶ���ָ��λλ��
    for (int j = 0; j < 4; j++)
    {
        algoTetris.positions[j].x += maxDistanceRow;
        algoTetris.positions[j].y -= maxDistanceCol;
        algoGrid[algoTetris.positions[j].x][algoTetris.positions[j].y] = algoTetris.color;
    }
    return maxDistanceCol;
}
// AI�����㷨
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
// �߶�����
INT ai_tetrti::getLH()
{
    int maxX, minX, maxY, minY;
    algoTetris.getMaxMin(minX, maxX, minY, maxY);
    return PAINT_YNODE - maxX;
}
// ������*���׷���������
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
            // ��
            clearline++; // ��+1
                         // ���й��׷�����
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
            // ��
            break;
        }
    }
    return clearline * cells;
}
// �б仯������
int ai_tetrti::getRT()
{
    // TODO �����Ż����������У�������ֱ��ÿ��+2
    int lineChane = 0, lastStatus;
    for (int i = PAINT_YNODE - 1; i >= 0; i--)
    {
        lastStatus = 1; // ��ǽ��״̬Ϊ��Ч����������ͼ��
        for (int j = 0; j < GAME_XNODE; j++)
        {
            // ����һ�����Ӳ�ͬ
            if (!(lastStatus * algoGrid[i][j]) && lastStatus != algoGrid[i][j])
            {
                lineChane++;
                lastStatus = algoGrid[i][j];
            }
            // ��ǰ�����һ���ڵ�Ϊ��
            if (j == (GAME_XNODE - 1) && algoGrid[i][j] == 0)
            {
                lineChane++;
            }
        }
    }
    return lineChane;
}
// �б仯������
INT ai_tetrti::getCT()
{
    int colChane = 0, lastStatus;
    for (int i = 0; i < GAME_XNODE; i++)
    {
        lastStatus = 1; // ��ǽ��״̬Ϊ��Ч����������ͼ��
        for (int j = 0; j < PAINT_YNODE; j++)
        {
            // ����һ�����Ӳ�ͬ
            if (!(lastStatus * algoGrid[j][i]) && lastStatus != algoGrid[j][i])
            {
                colChane++;
                lastStatus = algoGrid[j][i];
            }
            // ��ǰ�����һ���ڵ�Ϊ��
            if (j == (PAINT_YNODE - 1) && algoGrid[j][i] == 0)
            {
                colChane++;
            }
        }
    }
    return colChane;
}
// �ն�������
INT ai_tetrti::getNH()
{
    int holes = 0;
    bool holeFlag = false; // �Ƿ���Կ�ʼ����ն�
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
// ��������
INT ai_tetrti::getWS()
{
    // TODO: ������ⲻ��ȷ
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
            // �Ƿ�Ϊ��
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