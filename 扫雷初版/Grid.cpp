#define _CRT_SECURE_NO_WARNINGS 1
#include "Grid.h"
#include "Game.h"
#include <cstdio>

// 初始化静态成员变量
IMAGE Grid::s_imgInitial;
IMAGE Grid::s_imgFlag;
IMAGE Grid::s_imgBombed;
IMAGE Grid::s_imgUnbombed;
IMAGE Grid::s_imgNumbers[9];
bool Grid::s_imagesLoaded = false;

// 加载格子图片资源
bool Grid::LoadImages(int cellSize) {
    if (s_imagesLoaded) return true; // 已加载过，不重复加载
    
    try {
        // 加载未揭开格子图片
        loadimage(&s_imgInitial, _T("images/Initial.png"), cellSize, cellSize);
        
        // 加载标记图片
        loadimage(&s_imgFlag, _T("images/Marked.png"), cellSize, cellSize);
        
        // 加载地雷图片
        loadimage(&s_imgBombed, _T("images/Bombed.png"), cellSize, cellSize);
        loadimage(&s_imgUnbombed, _T("images/Unbombed.png"), cellSize, cellSize);
        
        // 加载数字图片（0-8）
        for (int i = 0; i <= 8; i++) {
            TCHAR filename[50];
            _stprintf(filename, _T("images/grid%d.png"), i);
            loadimage(&s_imgNumbers[i], filename, cellSize, cellSize);
        }
        
        s_imagesLoaded = true;
        return true;
    } catch (...) {
        // 如果加载失败，显示错误消息
        MessageBox(NULL, _T("加载格子图片资源失败！请确保images文件夹中包含所有必要的图片。"), _T("错误"), MB_OK | MB_ICONERROR);
        return false;
    }
}

// 默认构造函数 - 不需要参数，使用默认值初始化所有成员变量
Grid::Grid() : row(0), col(0), hasMine(false), adjacentMines(0), state(GridState::UNOPENED) {
}

// 带参数的构造函数 - 允许指定行列位置，其他成员使用默认值初始化
Grid::Grid(int row, int col) : row(row), col(col), hasMine(false), adjacentMines(0), state(GridState::UNOPENED) {
}

// 获取格子的行坐标
int Grid::getRow() const {
    return row;
}

// 获取格子的列坐标
int Grid::getCol() const {
    return col;
}

// 设置格子是否有地雷
void Grid::setMine(bool value) {
    hasMine = value;
}

// 判断格子是否有地雷
bool Grid::isMine() const {
    return hasMine;
}

// 设置周围地雷数量
void Grid::setAdjacentMines(int count) {
    adjacentMines = count;
}

// 获取周围地雷数量
int Grid::getAdjacentMines() const {
    return adjacentMines;
}

// 设置格子状态
void Grid::setState(GridState newState) {
    state = newState;
}

// 获取格子状态
GridState Grid::getState() const {
    return state;
}

// 判断格子是否已打开
bool Grid::isOpened() const {
    return state == GridState::OPENED;
}

// 判断格子是否被标记
bool Grid::isFlagged() const {
    return state == GridState::FLAGGED;
}

// 打开格子
void Grid::open() {
    state = GridState::OPENED;
}

// 切换标记状态
void Grid::toggleFlag() {
    if (state == GridState::UNOPENED) {
        state = GridState::FLAGGED;
    }
    else if (state == GridState::FLAGGED) {
        state = GridState::UNOPENED;
    }
}

// 绘制格子
void Grid::draw(int x, int y, bool isGameOver, bool isWin, bool showMine) const {
    // 确保图片已加载
    if (!s_imagesLoaded) {
        return;
    }
    
    // 根据格子状态绘制不同图像
    if (isOpened()) {
        if (isMine()) {
            // 如果是地雷，根据游戏状态显示不同图片
            if (isWin) {
                putimage(x, y, &s_imgUnbombed);
            } else {
                putimage(x, y, &s_imgBombed);
            }
        } else {
            // 显示周围地雷数量
            putimage(x, y, &s_imgNumbers[adjacentMines]);
        }
    } else if (isFlagged()) {
        // 显示标记
        putimage(x, y, &s_imgFlag);
    } else {
        // 显示未打开的格子
        putimage(x, y, &s_imgInitial);
        
        // 调试模式下，显示地雷位置
        if (showMine && isMine()) {
            // 设置透明混合模式
            setbkmode(TRANSPARENT);
            settextcolor(RGB(255, 0, 0));
            settextstyle(20, 0, _T("宋体"), 0, 0, FW_NORMAL, false, false, false);
            
            // 在地雷位置绘制红色"雷"字（居中显示）
            RECT r = {x, y, x + CELL_SIZE, y + CELL_SIZE};
            drawtext(_T("雷"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }
} 
