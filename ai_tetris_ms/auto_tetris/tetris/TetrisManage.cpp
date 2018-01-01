
#include "../stdafx.h"
#include "TetrisManage.h"

/* ***************AI模式**************** */
bool AI_SWITCH = false;
unsigned int AI_DEFAULT_INTERVAL = 10;
// 评分项权重
const  int LH_WEIGHT = -45;
const int RE_WEIGHT = 34;
const int RT_WEIGHT = -32;
const int CT_WEIGHT = -93;
const int NH_WEIGHT = -79;
const int WS_WEIGHT = -34;

CTeteisManage::CTeteisManage():btCurGrid(PAINT_YNODE, GAME_XNODE),
GAME_POSITION(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE + NODE_SIZE * GAME_XNODE, BLOCK_SIZE + NODE_SIZE * PAINT_YNODE),
INFO_POSITION(BLOCK_SIZE * 2 + NODE_SIZE * GAME_XNODE, BLOCK_SIZE, PAINT_WIDTH - BLOCK_SIZE, PAINT_HEIGHT - BLOCK_SIZE),
NEXT_FONT_POSITION(BLOCK_SIZE * 2 + NODE_SIZE * GAME_XNODE + BLOCK_SIZE, BLOCK_SIZE * 2, PAINT_WIDTH - BLOCK_SIZE * 2, BLOCK_SIZE * 2 + 2 * NODE_SIZE),
NEXT_TETRIS_POSITION(BLOCK_SIZE * 2 + NODE_SIZE * GAME_XNODE + NODE_SIZE, BLOCK_SIZE * 2 + 2 * NODE_SIZE, PAINT_WIDTH - BLOCK_SIZE - NODE_SIZE, BLOCK_SIZE * 2 + NODE_SIZE * 6),
LEVEL_FONT_POSITION(NODE_SIZE * GAME_XNODE + NODE_SIZE * 2, BLOCK_SIZE * 2 + 10 * NODE_SIZE, PAINT_WIDTH - BLOCK_SIZE - NODE_SIZE, BLOCK_SIZE * 2 + 11 * NODE_SIZE),
SCORE_FONT_POSITION(NODE_SIZE * GAME_XNODE + NODE_SIZE * 2, BLOCK_SIZE * 2 + 11 * NODE_SIZE, PAINT_WIDTH - BLOCK_SIZE - NODE_SIZE, BLOCK_SIZE * 2 + 12 * NODE_SIZE)
{
    ;
}

CTeteisManage::~CTeteisManage() {
    ;
}


void CALLBACK CTeteisManage::TimeProc(CTeteisManage* _this, HWND hWnd)
{
    // 定时器
    _this->checkTetris(hWnd, VK_DOWN);
}

// 函数： 初始化窗体的位置和大小
void CTeteisManage::InitWndPostionSize(HWND hWnd)
{
    RECT rect;
    int nWinX, nWinY, nClientX, nClientY;
    //获取窗口大小  
    GetWindowRect(hWnd, &rect);
    nWinX = rect.right - rect.left;
    nWinY = rect.bottom - rect.top;
    //获取客户区大小  
    GetClientRect(hWnd, &rect);
    nClientX = rect.right - rect.left;
    nClientY = rect.bottom - rect.top;

    // 获取当前屏幕的宽高，将窗口放置中心区域
    UINT iScreenX = GetSystemMetrics(SM_CXSCREEN);
    UINT iScreenY = GetSystemMetrics(SM_CYSCREEN);
    // 调整窗口位置与大小
    MoveWindow(hWnd, (iScreenX - PAINT_WIDTH) / 2, (iScreenY - PAINT_HEIGHT) / 3, PAINT_WIDTH + nWinX - nClientX, PAINT_HEIGHT + nWinY - nClientY, true);
}


// 函数： 启动游戏
void CTeteisManage::InitGame(HWND hWnd)
{
    // 初始化参数
    btCurGrid.init(PAINT_YNODE, GAME_XNODE);
    iInterval = AI_SWITCH ? AI_DEFAULT_INTERVAL : DEFAULT_INTERVAL;
    iCurDif = SIMPLE;
    iCurScore = 0;
    // 生成首个下落图形
    curTetris.random();
    // 生成下一个下落图形
    nextTetris.random();
    // 启动定时器
    _timer.Expire();
    _timer.StartTimer(iInterval, std::bind(TimeProc, this, hWnd));

}


// 函数： 绘制信息栏
void CTeteisManage::DrawInfo(HDC hdc)
{
    // 下一个下落方块
    TERECT rect;
    HFONT hFont = CreateFont(23, 0, 0, 0, FW_THIN, 0, 0, 0, UNICODE, 0, 0, 0, 0, L"微软雅黑");
    SelectObject(hdc, hFont);
    SetBkMode(hdc, TRANSPARENT);
    SetBkColor(hdc, RGB(255, 255, 0));

    NEXT_FONT_POSITION.setRect(rect);
    DrawText(hdc, TEXT("下一个"), _tcslen(TEXT("下一个")), &rect, 0);

    LEVEL_FONT_POSITION.setRect(rect);
    TCHAR tcDif[10];
    LPCTSTR lpctDif = TEXT("难度: %d");
    wsprintf(tcDif, lpctDif, iCurDif);
    DrawText(hdc, tcDif, _tcslen(tcDif), &rect, 0);

    SCORE_FONT_POSITION.setRect(rect);
    TCHAR tcScore[10];
    LPCTSTR lpctScore = TEXT("得分: %d");
    wsprintf(tcScore, lpctScore, iCurScore);
    RECT winrect;
    memcpy(&winrect, &rect, sizeof(RECT));
    DrawText(hdc, tcScore, _tcslen(tcScore), &winrect, 0);

    DeleteObject(hFont);
}


// 根据坐标获取当前节点位置
RectPosition CTeteisManage::getRectPosition(BYTE x, BYTE y)
{
    RectPosition rectPs;
    rectPs.iLeftX = BLOCK_SIZE + y*NODE_SIZE;
    rectPs.iLeftY = BLOCK_SIZE + x*NODE_SIZE;
    rectPs.iRightX = rectPs.iLeftX + NODE_SIZE;
    rectPs.iRightY = rectPs.iLeftY + NODE_SIZE;
    return rectPs;
}
// 函数：绘制方块
void CTeteisManage::DrawTetris(HDC hdc)
{
    RectPosition ps;
    HPEN hPen = (HPEN)GetStockObject(NULL_PEN);
    SelectObject(hdc, hPen);
    // 绘制Game区域
    for (int i = 0; i< PAINT_YNODE; i++)
    {
        for (int j = 0; j< GAME_XNODE; j++)
        {
            if (btCurGrid[i][j] > 0)
            {
                // 绘制当前节点
                SelectObject(hdc, getHbrush(btCurGrid[i][j]));
                ps = getRectPosition(i, j);
                Rectangle(hdc, ps.iLeftX, ps.iLeftY, ps.iRightX, ps.iRightY);
            }
        }
    }
    // 绘制正在下落图形
    SelectObject(hdc, getHbrush(curTetris.color));
    for (int i = 0; i < 4; i++)
    {
        ps = getRectPosition(curTetris.positions[i].x, curTetris.positions[i].y);
        Rectangle(hdc, ps.iLeftX, ps.iLeftY, ps.iRightX, ps.iRightY);
    }
    // 绘制下一次下落图形
    int move_x = 8 * NODE_SIZE + BLOCK_SIZE;
    int move_y = 2 * NODE_SIZE + BLOCK_SIZE;
    SelectObject(hdc, getHbrush(nextTetris.color));
    for (int i = 0; i < 4; i++)
    {
        ps = getRectPosition(nextTetris.positions[i].x, nextTetris.positions[i].y);
        Rectangle(hdc, ps.iLeftX + move_x, ps.iLeftY + move_y, ps.iRightX + move_x, ps.iRightY + move_y);
    }
}
// 调整游戏难度
void CTeteisManage::setLevel(HWND hWnd)
{
    BOOL SimpleToGeneral = iCurDif < GENERAL && iCurScore >= GENERAL_SCORE;
    BOOL GeneralToDifficult = iCurDif < DIFFICULT && iCurScore >= DIFFICULT_SCORE;
    BOOL DifficultToEst = iCurDif < DIFFICULTEST && iCurScore >= DIFFICULTEST_SCORE;

    // 设置难度
    if (SimpleToGeneral || GeneralToDifficult || DifficultToEst)
    {
        iCurDif++;
        iInterval -= 250;
        _timer.Expire();
        _timer.StartTimer(iInterval, std::bind(TimeProc, this, hWnd));
    }
}
// 下落完成
void CTeteisManage::finishDown(HWND hWnd)
{
    // 更新btCurGrid当前界面数组
    btCurGrid.addCurTerisToGrid(curTetris);
    // 更新下落方块，重新生成nextTetris
    curTetris = nextTetris;
    nextTetris.random();
    // 消除
    iCurScore += btCurGrid.clearLine();
    // 判断生成位置是否被占用
    if (btCurGrid.isExist(curTetris))
    {
        // TODO 游戏结束
        //KillTimer(hWnd, TIMER_ID);
        _timer.Expire();
        MessageBox(hWnd, TEXT("游戏结束"), TEXT("消息"), MB_OK);
        return;
    }
    // 调整游戏难度(ai模式不需要)
    if (!AI_SWITCH)
    {
        setLevel(hWnd);
        InvalidateRect(hWnd, NULL, TRUE);
    }
    else
    {
        //InvalidateRect(hWnd, NULL, TRUE);
        //// 如果为AI模式，继续计算
        //AIHandle(hWnd);
    }
}

// 检查图形下落是否越界/完成 / 消除
void CTeteisManage::checkTetris(HWND hWnd, unsigned int type)
{
    int maxX, minX, maxY, minY;
    Tetris temp = curTetris;
    switch (type)
    {
    case VK_LEFT: // 左
        temp.left();
        break;
    case VK_RIGHT: // 右
        temp.right();
        break;
    case VK_UP: // 上
        temp.rotate();
        break;
    case VK_DOWN: // 下(产生快速下降效果)
        temp.down();
        break;
    }
    temp.getMaxMin(minX, maxX, minY, maxY);

    // 判断是否越界
    if (maxX > 19)
    {
        // 触碰底部，下落结束
        finishDown(hWnd);
        return;
    }
    if (minX < 0 || maxY > 9 || minY < 0)
    {
        // 触碰左右边界
        return;
    }
    // 判断位置是否被占用
    if (btCurGrid.isExist(temp))
    {
        if (type == VK_DOWN)
        {
            // 下落到已存在节点上方，下落结束
            finishDown(hWnd);
        }
        return;
    }
    // 操作允许
    curTetris = temp;
    InvalidateRect(hWnd, NULL, TRUE);
}


// 函数： 绘制背景
void CTeteisManage::DrawBackGround(HDC hdc)
{
    HPEN hPenNull = (HPEN)GetStockObject(NULL_PEN);         // HPEN NULL
    HBRUSH hBrushGray = (HBRUSH)GetStockObject(GRAY_BRUSH); // hBRUSH GRAY
    SelectObject(hdc, hPenNull);
    SelectObject(hdc, hBrushGray); // 背景颜色	
                                   // 游戏区域大小 (HDC,左上角X,左上角Y，右下角X，右下角Y)
    Rectangle(hdc, GAME_POSITION.iLeftX, GAME_POSITION.iLeftY, GAME_POSITION.iRightX, GAME_POSITION.iRightY);
    // 信息区大小
    Rectangle(hdc, INFO_POSITION.iLeftX, INFO_POSITION.iLeftY, INFO_POSITION.iRightX, INFO_POSITION.iRightY);
    DeleteObject(hBrushGray);
    DeleteObject(hPenNull);

}