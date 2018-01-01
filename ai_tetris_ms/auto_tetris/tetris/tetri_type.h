#pragma once


#define TIMER_ID 1
#define AI_TIMER_ID 2
#include "Windows.h"
/* *************ȫ�ֳ���************* */
// ��Ϸ�Ѷ�
const enum GAME_DIF
{
    SIMPLE = 1,
    GENERAL,
    DIFFICULT,
    DIFFICULTEST
};
const unsigned int GENERAL_SCORE = 5; // �е��Ѷȶ�Ӧ��������
const unsigned int DIFFICULT_SCORE = 10; // ���Ѷ�Ӧ��������
const unsigned int DIFFICULTEST_SCORE = 15; // �ر����Ѷ�Ӧ��������
const unsigned int DEFAULT_INTERVAL = 1000; // Ĭ������ʱ����

const unsigned int BLOCK_SIZE = 10; // ����
const unsigned int NODE_SIZE = 30; // ÿ���ڵ��СΪ10
const unsigned int GAME_XNODE = 10; // ��Ϸ��X��10���ڵ�
const unsigned int INFO_XNODE = 6; // ��Ϣչʾ�����5���ڵ�
const unsigned int PAINT_YNODE = 20; //Y��20���ڵ�
const unsigned int PAINT_WIDTH = ((GAME_XNODE + INFO_XNODE) * NODE_SIZE + 3 * BLOCK_SIZE); // ���ڿ��
const unsigned int PAINT_HEIGHT = (PAINT_YNODE * NODE_SIZE + 2 * BLOCK_SIZE);             // ���ڸ߶�

typedef RECT TERECT;