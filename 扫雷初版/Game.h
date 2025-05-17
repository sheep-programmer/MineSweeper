#pragma once
#include <easyx.h>     // EasyX图形库
#include <graphics.h>  // EasyX图形库扩展
#include <ctime>
#include <windows.h>
#include "Board.h"

// 游戏配置常量
#define ROWS 9        // 行数
#define COLS 9         // 列数
#define MINE_COUNT 10  // 地雷数量，增加一些地雷使游戏更有挑战性
#define CELL_SIZE 30   // 格子大小
#define MARGIN 100     // 边距
#define INFO_HEIGHT 40 // 信息栏高度
#define INFO_MARGIN 10 // 信息栏内边距

// 前向声明
class Game;

// 游戏状态回调函数
typedef void (*GameStateCallback)(bool isMine, int remainingCells);

// 窗口管理类，负责创建和管理窗口
class WindowManager {
private:
    HWND hwnd;             // 窗口句柄
    HICON hIcon;           // 图标句柄
    Game* gameInstance;    // 游戏实例指针

public:
    // 构造函数
    WindowManager();
    
    // 析构函数
    ~WindowManager();
    
    // 初始化窗口
    void initWindow(int width, int height, Game* game);
    
    // 设置窗口图标
    void setIcon(const char* iconPath);
    
    // 获取窗口句柄
    HWND getHWnd() const;
    
    // 注册窗口过程
    void registerWindowProcedure();
    
    // 关闭窗口
    void closeWindow();
    
    // 调整窗口大小
    void resizeWindow(int width, int height);
};

// 游戏UI类，负责绘制游戏界面
class GameUI {
private:
    // 图片资源
    IMAGE imgRestart[3];          // 游戏状态图标（正常、获胜、失败）
    IMAGE digitsImg[10];          // 数字显示（计数器和计时器）
    
    POINT restartBtnPos;          // 重新开始按钮位置
    RECT infoRect;                // 信息栏区域
    
public:
    // 构造函数
    GameUI();
    
    // 初始化UI
    void init();
    
    // 加载图片资源
    void loadImages();
    
    // 绘制游戏界面
    void drawUI(const Board& board, int gameTime = 0, bool debugMode = false, bool isGameOver = false, bool isWin = false);
    
    // 绘制信息栏（包含雷数、笑脸和计时器）
    void drawInfoPanel(const Board& board, int gameTime, bool isGameOver, bool isWin);
    
    // 绘制数字显示（用于雷数和时间）
    void drawDigits(int x, int y, int number, int digits);
    
    // 获取重新开始按钮位置
    POINT getRestartBtnPos() const;
    
    // 获取信息栏区域
    RECT getInfoRect() const;
};

// 扫雷游戏主类
class Game {
private:
    Board board;             // 游戏棋盘
    GameUI ui;               // 游戏UI
    WindowManager windowManager; // 窗口管理器
    bool isRunning;          // 游戏是否运行中
    time_t startTime;        // 游戏开始时间
    int gameTime;            // 游戏已进行时间（秒）
    bool isDebugMode;        // 是否处于调试模式
    bool isGameOver;         // 游戏是否结束
    bool isWin;              // 是否获胜
    int remainingMines;      // 剩余地雷数量
    
    // 游戏状态回调方法，供Board类调用通知游戏状态变化
    static void GameStateChangeCallback(bool isMine, int remainingCells);
    
public:
    // 构造函数
    Game();
    
    // 析构函数
    ~Game();
    
    // 游戏初始化
    void init();
    
    // 游戏开始
    void start();
    
    // 重新开始游戏
    void restart();
    
    // 处理鼠标点击事件
    void handleMouseClick(int x, int y, bool isLeftClick);
    
    // 游戏主循环
    void gameLoop();
    
    // 更新游戏时间
    void updateGameTime();
    
    // 获取游戏时间
    int getGameTime() const;
    
    // 调试函数：切换显示所有雷的位置
    void debugShowAllMines();
    
    // 游戏结束处理
    void gameOver(bool win);
    
    // 检查是否获胜
    void checkWin(int remainingCells);
    
    // 显示所有地雷
    void showAllMines();
    
    // 更新剩余地雷数量
    void updateRemainingMines(int mines);
    
    // 获取游戏状态
    bool isGameOverState() const { return isGameOver; }
    bool isWinState() const { return isWin; }
    
    // 获取游戏棋盘对象
    Board& getBoard() { return board; }
    
    // 获取当前是否处于调试模式
    bool isInDebugMode() const { return isDebugMode; }
}; 
