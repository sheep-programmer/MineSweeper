#pragma once
#include <easyx.h>     // EasyX图形库
#include <graphics.h>  // EasyX图形库扩展
#include <ctime>
#include <windows.h>
#include <mmsystem.h>  // 增加mmsystem.h头文件用于播放音效
#include "Board.h"

// 游戏难度枚举
enum class GameDifficulty {
    BEGINNER,    // 初级：9x9, 10个雷
    INTERMEDIATE, // 中级：16x16, 40个雷
    EXPERT       // 专家：16x30, 99个雷
};

// 游戏配置常量
// 基础难度配置
#define BEGINNER_ROWS 9        // 初级行数
#define BEGINNER_COLS 9        // 初级列数
#define BEGINNER_MINES 10      // 初级地雷数

// 中级难度配置
#define INTERMEDIATE_ROWS 16   // 中级行数
#define INTERMEDIATE_COLS 16   // 中级列数
#define INTERMEDIATE_MINES 40  // 中级地雷数

// 专家难度配置
#define EXPERT_ROWS 16         // 专家行数
#define EXPERT_COLS 30         // 专家列数
#define EXPERT_MINES 99        // 专家地雷数

// 默认使用初级难度
#define ROWS BEGINNER_ROWS
#define COLS BEGINNER_COLS
#define MINE_COUNT BEGINNER_MINES

#define CELL_SIZE 30   // 格子大小
#define MARGIN 100     // 边距
#define INFO_HEIGHT 40 // 信息栏高度
#define INFO_MARGIN 10 // 信息栏内边距
#define DIFFICULTY_HEIGHT 30 // 难度选择栏高度

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
    RECT difficultyRect;          // 难度选择区域
    RECT beginnerRect;            // 初级难度文字区域
    RECT intermediateRect;        // 中级难度文字区域
    RECT expertRect;              // 专家难度文字区域
    
public:
    // 构造函数
    GameUI();
    
    // 初始化UI
    void init();
    
    // 加载图片资源
    void loadImages();
    
    // 更新UI布局
    void updateUILayout(int cols);
    
    // 绘制游戏界面
    void drawUI(const Board& board, int gameTime = 0, bool debugMode = false, bool isGameOver = false, bool isWin = false, GameDifficulty difficulty = GameDifficulty::BEGINNER);
    
    // 绘制信息栏（包含雷数、笑脸和计时器）
    void drawInfoPanel(const Board& board, int gameTime, bool isGameOver, bool isWin);
    
    // 绘制难度选择栏
    void drawDifficultyPanel(GameDifficulty currentDifficulty);
    
    // 绘制数字显示（用于雷数和时间）
    void drawDigits(int x, int y, int number, int digits);
    
    // 获取重新开始按钮位置
    POINT getRestartBtnPos() const;
    
    // 获取信息栏区域
    RECT getInfoRect() const;
    
    // 获取难度选择区域相关信息
    RECT getDifficultyRect() const;
    RECT getBeginnerRect() const;
    RECT getIntermediateRect() const;
    RECT getExpertRect() const;
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
    
    // 难度相关
    GameDifficulty currentDifficulty; // 当前游戏难度
    int rows;                // 当前行数
    int cols;                // 当前列数
    int mineCount;           // 当前地雷数
    
    // 音效相关
    bool soundEnabled;       // 是否启用音效
    
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
    
    // 切换游戏难度
    void setDifficulty(GameDifficulty difficulty);
    
    // 处理鼠标点击事件
    void handleMouseClick(int x, int y, bool isLeftClick);
    
    // 处理难度选择区域的点击
    bool handleDifficultyClick(int x, int y);
    
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
    
    // 音效相关方法
    void initSound();         // 初始化音效
    void playClickSound();    // 播放点击音效
    void playBoomSound();     // 播放爆炸音效
    void closeSound();        // 关闭所有音效
}; 
