#pragma once
#include <vector>
#include <windows.h>
#include "Grid.h"

// 定义回调函数指针类型
typedef void (*ResetCallback)();
typedef void (*GameStateCallback)(bool isMine, int remainingCells);

// 游戏棋盘类，负责管理游戏网格和逻辑
class Board {
private:
    std::vector<std::vector<Grid>> grid;  // 游戏网格
    bool isFirstClick;                    // 是否是第一次点击
    int remainingCells;                   // 剩余未揭开的格子数
    int remainingMines;                   // 剩余雷数
    ResetCallback resetCallback;          // 重置函数
    GameStateCallback stateCallback;      // 游戏状态变化回调
    bool showMinesMode;                   // 是否显示所有地雷（调试模式）
    
    // 棋盘尺寸
    int rows;                           // 棋盘行数
    int cols;                           // 棋盘列数
    int mineCount;                      // 地雷数量

public:
    // 构造函数
    Board();
    
    // 设置棋盘尺寸和地雷数量
    void setSize(int newRows, int newCols, int newMineCount);

    // 初始化游戏网格
    void initGrid();

    // 放置地雷
    void placeMines(int firstClickX, int firstClickY);

    // 计算每个格子周围的地雷数量
    void countMines();

    // 打开格子
    void openCell(int row, int col);

    // 递归打开周围空白格子
    void openAdjacentCells(int row, int col);

    // 处理鼠标点击事件
    void handleMouseClick(int x, int y, bool isLeftClick, POINT restartBtnPos);

    // 重新开始游戏
    void restart();

    // 切换格子的标记状态
    void toggleCellFlag(int row, int col);

    // 获取格子
    const Grid& getCell(int row, int col) const;

    // 获取游戏状态信息
    bool isFirstClickState() const;
    int getRemainingMines() const;
    int getRemainingCells() const;
    void setFirstClickState(bool state);
    
    // 获取棋盘尺寸信息
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    int getMineCount() const { return mineCount; }

    // 设置重置回调函数
    void setResetCallback(ResetCallback callback);
    
    // 设置游戏状态回调函数
    void setStateCallback(GameStateCallback callback);

    // 调试函数：切换显示所有雷的位置模式，返回当前是否处于调试模式
    bool debugShowAllMines();
    
    // 显示或隐藏所有地雷（游戏结束时由Game类调用）
    void showAllMines(bool isWin);

    // 获取当前是否处于调试模式
    bool isDebugMode() const { return showMinesMode; }
}; 
