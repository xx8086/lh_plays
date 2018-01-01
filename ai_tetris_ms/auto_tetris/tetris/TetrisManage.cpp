
#include "../stdafx.h"
#include "TetrisManage.h"

/* ***************AIģʽ**************** */
bool AI_SWITCH = false;
unsigned int AI_DEFAULT_INTERVAL = 10;
// ������Ȩ��
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
    // ��ʱ��
    _this->checkTetris(hWnd, VK_DOWN);
}

// ������ ��ʼ�������λ�úʹ�С
void CTeteisManage::InitWndPostionSize(HWND hWnd)
{
    RECT rect;
    int nWinX, nWinY, nClientX, nClientY;
    //��ȡ���ڴ�С  
    GetWindowRect(hWnd, &rect);
    nWinX = rect.right - rect.left;
    nWinY = rect.bottom - rect.top;
    //��ȡ�ͻ�����С  
    GetClientRect(hWnd, &rect);
    nClientX = rect.right - rect.left;
    nClientY = rect.bottom - rect.top;

    // ��ȡ��ǰ��Ļ�Ŀ�ߣ������ڷ�����������
    UINT iScreenX = GetSystemMetrics(SM_CXSCREEN);
    UINT iScreenY = GetSystemMetrics(SM_CYSCREEN);
    // ��������λ�����С
    MoveWindow(hWnd, (iScreenX - PAINT_WIDTH) / 2, (iScreenY - PAINT_HEIGHT) / 3, PAINT_WIDTH + nWinX - nClientX, PAINT_HEIGHT + nWinY - nClientY, true);
}


// ������ ������Ϸ
void CTeteisManage::InitGame(HWND hWnd)
{
    // ��ʼ������
    btCurGrid.init(PAINT_YNODE, GAME_XNODE);
    iInterval = AI_SWITCH ? AI_DEFAULT_INTERVAL : DEFAULT_INTERVAL;
    iCurDif = SIMPLE;
    iCurScore = 0;
    // �����׸�����ͼ��
    curTetris.random();
    // ������һ������ͼ��
    nextTetris.random();
    // ������ʱ��
    _timer.Expire();
    _timer.StartTimer(iInterval, std::bind(TimeProc, this, hWnd));

}


// ������ ������Ϣ��
void CTeteisManage::DrawInfo(HDC hdc)
{
    // ��һ�����䷽��
    TERECT rect;
    HFONT hFont = CreateFont(23, 0, 0, 0, FW_THIN, 0, 0, 0, UNICODE, 0, 0, 0, 0, L"΢���ź�");
    SelectObject(hdc, hFont);
    SetBkMode(hdc, TRANSPARENT);
    SetBkColor(hdc, RGB(255, 255, 0));

    NEXT_FONT_POSITION.setRect(rect);
    DrawText(hdc, TEXT("��һ��"), _tcslen(TEXT("��һ��")), &rect, 0);

    LEVEL_FONT_POSITION.setRect(rect);
    TCHAR tcDif[10];
    LPCTSTR lpctDif = TEXT("�Ѷ�: %d");
    wsprintf(tcDif, lpctDif, iCurDif);
    DrawText(hdc, tcDif, _tcslen(tcDif), &rect, 0);

    SCORE_FONT_POSITION.setRect(rect);
    TCHAR tcScore[10];
    LPCTSTR lpctScore = TEXT("�÷�: %d");
    wsprintf(tcScore, lpctScore, iCurScore);
    RECT winrect;
    memcpy(&winrect, &rect, sizeof(RECT));
    DrawText(hdc, tcScore, _tcslen(tcScore), &winrect, 0);

    DeleteObject(hFont);
}


// ���������ȡ��ǰ�ڵ�λ��
RectPosition CTeteisManage::getRectPosition(BYTE x, BYTE y)
{
    RectPosition rectPs;
    rectPs.iLeftX = BLOCK_SIZE + y*NODE_SIZE;
    rectPs.iLeftY = BLOCK_SIZE + x*NODE_SIZE;
    rectPs.iRightX = rectPs.iLeftX + NODE_SIZE;
    rectPs.iRightY = rectPs.iLeftY + NODE_SIZE;
    return rectPs;
}
// ���������Ʒ���
void CTeteisManage::DrawTetris(HDC hdc)
{
    RectPosition ps;
    HPEN hPen = (HPEN)GetStockObject(NULL_PEN);
    SelectObject(hdc, hPen);
    // ����Game����
    for (int i = 0; i< PAINT_YNODE; i++)
    {
        for (int j = 0; j< GAME_XNODE; j++)
        {
            if (btCurGrid[i][j] > 0)
            {
                // ���Ƶ�ǰ�ڵ�
                SelectObject(hdc, getHbrush(btCurGrid[i][j]));
                ps = getRectPosition(i, j);
                Rectangle(hdc, ps.iLeftX, ps.iLeftY, ps.iRightX, ps.iRightY);
            }
        }
    }
    // ������������ͼ��
    SelectObject(hdc, getHbrush(curTetris.color));
    for (int i = 0; i < 4; i++)
    {
        ps = getRectPosition(curTetris.positions[i].x, curTetris.positions[i].y);
        Rectangle(hdc, ps.iLeftX, ps.iLeftY, ps.iRightX, ps.iRightY);
    }
    // ������һ������ͼ��
    int move_x = 8 * NODE_SIZE + BLOCK_SIZE;
    int move_y = 2 * NODE_SIZE + BLOCK_SIZE;
    SelectObject(hdc, getHbrush(nextTetris.color));
    for (int i = 0; i < 4; i++)
    {
        ps = getRectPosition(nextTetris.positions[i].x, nextTetris.positions[i].y);
        Rectangle(hdc, ps.iLeftX + move_x, ps.iLeftY + move_y, ps.iRightX + move_x, ps.iRightY + move_y);
    }
}
// ������Ϸ�Ѷ�
void CTeteisManage::setLevel(HWND hWnd)
{
    BOOL SimpleToGeneral = iCurDif < GENERAL && iCurScore >= GENERAL_SCORE;
    BOOL GeneralToDifficult = iCurDif < DIFFICULT && iCurScore >= DIFFICULT_SCORE;
    BOOL DifficultToEst = iCurDif < DIFFICULTEST && iCurScore >= DIFFICULTEST_SCORE;

    // �����Ѷ�
    if (SimpleToGeneral || GeneralToDifficult || DifficultToEst)
    {
        iCurDif++;
        iInterval -= 250;
        _timer.Expire();
        _timer.StartTimer(iInterval, std::bind(TimeProc, this, hWnd));
    }
}
// �������
void CTeteisManage::finishDown(HWND hWnd)
{
    // ����btCurGrid��ǰ��������
    btCurGrid.addCurTerisToGrid(curTetris);
    // �������䷽�飬��������nextTetris
    curTetris = nextTetris;
    nextTetris.random();
    // ����
    iCurScore += btCurGrid.clearLine();
    // �ж�����λ���Ƿ�ռ��
    if (btCurGrid.isExist(curTetris))
    {
        // TODO ��Ϸ����
        //KillTimer(hWnd, TIMER_ID);
        _timer.Expire();
        MessageBox(hWnd, TEXT("��Ϸ����"), TEXT("��Ϣ"), MB_OK);
        return;
    }
    // ������Ϸ�Ѷ�(aiģʽ����Ҫ)
    if (!AI_SWITCH)
    {
        setLevel(hWnd);
        InvalidateRect(hWnd, NULL, TRUE);
    }
    else
    {
        //InvalidateRect(hWnd, NULL, TRUE);
        //// ���ΪAIģʽ����������
        //AIHandle(hWnd);
    }
}

// ���ͼ�������Ƿ�Խ��/��� / ����
void CTeteisManage::checkTetris(HWND hWnd, unsigned int type)
{
    int maxX, minX, maxY, minY;
    Tetris temp = curTetris;
    switch (type)
    {
    case VK_LEFT: // ��
        temp.left();
        break;
    case VK_RIGHT: // ��
        temp.right();
        break;
    case VK_UP: // ��
        temp.rotate();
        break;
    case VK_DOWN: // ��(���������½�Ч��)
        temp.down();
        break;
    }
    temp.getMaxMin(minX, maxX, minY, maxY);

    // �ж��Ƿ�Խ��
    if (maxX > 19)
    {
        // �����ײ����������
        finishDown(hWnd);
        return;
    }
    if (minX < 0 || maxY > 9 || minY < 0)
    {
        // �������ұ߽�
        return;
    }
    // �ж�λ���Ƿ�ռ��
    if (btCurGrid.isExist(temp))
    {
        if (type == VK_DOWN)
        {
            // ���䵽�Ѵ��ڽڵ��Ϸ����������
            finishDown(hWnd);
        }
        return;
    }
    // ��������
    curTetris = temp;
    InvalidateRect(hWnd, NULL, TRUE);
}


// ������ ���Ʊ���
void CTeteisManage::DrawBackGround(HDC hdc)
{
    HPEN hPenNull = (HPEN)GetStockObject(NULL_PEN);         // HPEN NULL
    HBRUSH hBrushGray = (HBRUSH)GetStockObject(GRAY_BRUSH); // hBRUSH GRAY
    SelectObject(hdc, hPenNull);
    SelectObject(hdc, hBrushGray); // ������ɫ	
                                   // ��Ϸ�����С (HDC,���Ͻ�X,���Ͻ�Y�����½�X�����½�Y)
    Rectangle(hdc, GAME_POSITION.iLeftX, GAME_POSITION.iLeftY, GAME_POSITION.iRightX, GAME_POSITION.iRightY);
    // ��Ϣ����С
    Rectangle(hdc, INFO_POSITION.iLeftX, INFO_POSITION.iLeftY, INFO_POSITION.iRightX, INFO_POSITION.iRightY);
    DeleteObject(hBrushGray);
    DeleteObject(hPenNull);

}