#pragma once
#include <easyx.h>
#include <graphics.h>
#include <vector>

// 全局常量
#define CELL_SIZE 30   // 格子大小
#define MARGIN 100     // 边距

// 格子状态枚举
enum class GridState {
    UNOPENED,    // 未打开
    OPENED,      // 已打开
    FLAGGED      // 标记
};

// 表示扫雷游戏中的一个格子
class Grid {
private:
    int row;            // 行坐标
    int col;            // 列坐标
    bool hasMine;       // 是否有地雷
    int adjacentMines;  // 周围地雷数量
    GridState state;    // 格子状态
    
    // 格子图片资源（静态成员，所有格子共享）
    static IMAGE s_imgInitial;     // 未揭开
    static IMAGE s_imgFlag;        // 标记
    static IMAGE s_imgBombed;      // 踩到地雷
    static IMAGE s_imgUnbombed;    // 未踩到的地雷（胜利时）
    static IMAGE s_imgNumbers[9];  // 数字0-8（揭开后显示）
    static bool s_imagesLoaded;    // 图片是否已加载
    static bool s_textSettingsInitialized; // 文本设置是否已初始化
    
public:
    // 构造函数
    Grid();
    Grid(int row, int col);
    
    // 静态方法：加载格子图片资源
    static bool LoadImages(int cellSize);
    
    // 获取格子的行坐标
    int getRow() const;
    
    // 获取格子的列坐标
    int getCol() const;
    
    // 设置/获取格子是否有地雷
    void setMine(bool value);
    bool isMine() const;
    
    // 设置/获取周围地雷数量
    void setAdjacentMines(int count);
    int getAdjacentMines() const;
    
    // 设置/获取格子状态
    void setState(GridState newState);
    GridState getState() const;
    
    // 判断格子是否已打开
    bool isOpened() const;
    
    // 判断格子是否被标记
    bool isFlagged() const;
    
    // 打开格子
    void open();
    
    // 切换标记状态
    void toggleFlag();
    
    // 绘制格子
    void draw(int x, int y, bool isGameOver, bool isWin, bool showMine) const;
}; 
