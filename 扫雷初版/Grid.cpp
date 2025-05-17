#define _CRT_SECURE_NO_WARNINGS 1
#include "Grid.h"
#include "Game.h"
#include <cstdio>

// ��ʼ����̬��Ա����
IMAGE Grid::s_imgInitial;
IMAGE Grid::s_imgFlag;
IMAGE Grid::s_imgBombed;
IMAGE Grid::s_imgUnbombed;
IMAGE Grid::s_imgNumbers[9];
bool Grid::s_imagesLoaded = false;

// ���ظ���ͼƬ��Դ
bool Grid::LoadImages(int cellSize) {
    if (s_imagesLoaded) return true; // �Ѽ��ع������ظ�����
    
    try {
        // ����δ�ҿ�����ͼƬ
        loadimage(&s_imgInitial, _T("images/Initial.png"), cellSize, cellSize);
        
        // ���ر��ͼƬ
        loadimage(&s_imgFlag, _T("images/Marked.png"), cellSize, cellSize);
        
        // ���ص���ͼƬ
        loadimage(&s_imgBombed, _T("images/Bombed.png"), cellSize, cellSize);
        loadimage(&s_imgUnbombed, _T("images/Unbombed.png"), cellSize, cellSize);
        
        // ��������ͼƬ��0-8��
        for (int i = 0; i <= 8; i++) {
            TCHAR filename[50];
            _stprintf(filename, _T("images/grid%d.png"), i);
            loadimage(&s_imgNumbers[i], filename, cellSize, cellSize);
        }
        
        s_imagesLoaded = true;
        return true;
    } catch (...) {
        // �������ʧ�ܣ���ʾ������Ϣ
        MessageBox(NULL, _T("���ظ���ͼƬ��Դʧ�ܣ���ȷ��images�ļ����а������б�Ҫ��ͼƬ��"), _T("����"), MB_OK | MB_ICONERROR);
        return false;
    }
}

// Ĭ�Ϲ��캯�� - ����Ҫ������ʹ��Ĭ��ֵ��ʼ�����г�Ա����
Grid::Grid() : row(0), col(0), hasMine(false), adjacentMines(0), state(GridState::UNOPENED) {
}

// �������Ĺ��캯�� - ����ָ������λ�ã�������Աʹ��Ĭ��ֵ��ʼ��
Grid::Grid(int row, int col) : row(row), col(col), hasMine(false), adjacentMines(0), state(GridState::UNOPENED) {
}

// ��ȡ���ӵ�������
int Grid::getRow() const {
    return row;
}

// ��ȡ���ӵ�������
int Grid::getCol() const {
    return col;
}

// ���ø����Ƿ��е���
void Grid::setMine(bool value) {
    hasMine = value;
}

// �жϸ����Ƿ��е���
bool Grid::isMine() const {
    return hasMine;
}

// ������Χ��������
void Grid::setAdjacentMines(int count) {
    adjacentMines = count;
}

// ��ȡ��Χ��������
int Grid::getAdjacentMines() const {
    return adjacentMines;
}

// ���ø���״̬
void Grid::setState(GridState newState) {
    state = newState;
}

// ��ȡ����״̬
GridState Grid::getState() const {
    return state;
}

// �жϸ����Ƿ��Ѵ�
bool Grid::isOpened() const {
    return state == GridState::OPENED;
}

// �жϸ����Ƿ񱻱��
bool Grid::isFlagged() const {
    return state == GridState::FLAGGED;
}

// �򿪸���
void Grid::open() {
    state = GridState::OPENED;
}

// �л����״̬
void Grid::toggleFlag() {
    if (state == GridState::UNOPENED) {
        state = GridState::FLAGGED;
    }
    else if (state == GridState::FLAGGED) {
        state = GridState::UNOPENED;
    }
}

// ���Ƹ���
void Grid::draw(int x, int y, bool isGameOver, bool isWin, bool showMine) const {
    // ȷ��ͼƬ�Ѽ���
    if (!s_imagesLoaded) {
        return;
    }
    
    // ���ݸ���״̬���Ʋ�ͬͼ��
    if (isOpened()) {
        if (isMine()) {
            // ����ǵ��ף�������Ϸ״̬��ʾ��ͬͼƬ
            if (isWin) {
                putimage(x, y, &s_imgUnbombed);
            } else {
                putimage(x, y, &s_imgBombed);
            }
        } else {
            // ��ʾ��Χ��������
            putimage(x, y, &s_imgNumbers[adjacentMines]);
        }
    } else if (isFlagged()) {
        // ��ʾ���
        putimage(x, y, &s_imgFlag);
    } else {
        // ��ʾδ�򿪵ĸ���
        putimage(x, y, &s_imgInitial);
        
        // ����ģʽ�£���ʾ����λ��
        if (showMine && isMine()) {
            // ����͸�����ģʽ
            setbkmode(TRANSPARENT);
            settextcolor(RGB(255, 0, 0));
            settextstyle(20, 0, _T("����"), 0, 0, FW_NORMAL, false, false, false);
            
            // �ڵ���λ�û��ƺ�ɫ"��"�֣�������ʾ��
            RECT r = {x, y, x + CELL_SIZE, y + CELL_SIZE};
            drawtext(_T("��"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }
} 
