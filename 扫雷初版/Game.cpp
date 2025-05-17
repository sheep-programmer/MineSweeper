#define _CRT_SECURE_NO_WARNINGS 1
#include "Game.h"
#include <cstdio>

// 全局变量 - 原始窗口过程函数指针
static WNDPROC g_oldWndProc = NULL;
static Game* g_gameInstance = NULL;  // 全局游戏实例指针，用于静态回调

// 窗口消息处理函数
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // 调用原始窗口过程
    return g_oldWndProc ? CallWindowProc(g_oldWndProc, hwnd, uMsg, wParam, lParam) : DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//----------------------------------------------------------------
// WindowManager 类实现
//----------------------------------------------------------------

// 构造函数
WindowManager::WindowManager() : hwnd(NULL), hIcon(NULL), gameInstance(NULL) {
}

// 析构函数
WindowManager::~WindowManager() {
    // 释放图标资源
    if (hIcon) {
        DestroyIcon(hIcon);
        hIcon = NULL;
    }
}

// 初始化窗口
void WindowManager::initWindow(int width, int height, Game* game) {
    gameInstance = game;
    
    // 居中显示窗口
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    // 创建指定位置和大小的窗口
    initgraph(width, height);
    hwnd = GetHWnd(); // 获取窗口句柄
    SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    
    // 设置窗口标题
    SetWindowText(hwnd, _T("扫雷游戏"));
    
    // 设置背景色
    setbkcolor(RGB(240, 240, 240));
    cleardevice();
}

// 设置窗口图标
void WindowManager::setIcon(const char* iconPath) {
    // 加载应用程序图标
    int len = MultiByteToWideChar(CP_ACP, 0, iconPath, -1, NULL, 0);
    wchar_t* wIconPath = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, iconPath, -1, wIconPath, len);
    
    // 使用宽字符版本的LoadImage (LoadImageW)
    hIcon = (HICON)LoadImageW(NULL, wIconPath, IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    if (hIcon) {
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    }
    
    delete[] wIconPath;
}

// 获取窗口句柄
HWND WindowManager::getHWnd() const {
    return hwnd;
}

// 注册窗口过程
void WindowManager::registerWindowProcedure() {
    // 注册窗口消息处理函数
    g_oldWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WindowProcedure);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)gameInstance);
}

// 关闭窗口
void WindowManager::closeWindow() {
    // 关闭图形窗口
    closegraph();
}

// 调整窗口大小
void WindowManager::resizeWindow(int width, int height) {
    // 关闭当前图形环境
    closegraph();
    
    // 居中显示窗口
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    // 创建指定位置和大小的窗口
    initgraph(width, height);
    hwnd = GetHWnd();
    SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    
    // 设置背景色并清屏
    setbkcolor(RGB(240, 240, 240));
    cleardevice();
    
    // 重新注册窗口过程
    g_oldWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WindowProcedure);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)gameInstance);
    
    // 设置窗口标题
    SetWindowText(hwnd, _T("扫雷游戏"));
}

//----------------------------------------------------------------
// GameUI 类实现
//----------------------------------------------------------------

// 构造函数
GameUI::GameUI() {
    // 计算信息栏位置
    int panelWidth = COLS * CELL_SIZE;
    int panelHeight = INFO_HEIGHT;
    int panelX = MARGIN;
    int panelY = MARGIN - INFO_HEIGHT - INFO_MARGIN;  // 就在雷区的上方
    
    // 设置信息栏区域
    infoRect.left = panelX;
    infoRect.top = panelY;
    infoRect.right = panelX + panelWidth;
    infoRect.bottom = panelY + panelHeight;
    
    // 设置重新开始按钮位置
    restartBtnPos.x = panelX + panelWidth / 2;
    restartBtnPos.y = panelY + panelHeight / 2;
}

// 初始化UI
void GameUI::init() {
    // 加载图片资源
    loadImages();
}

// 加载图片资源
void GameUI::loadImages() {
    try {
        // 加载格子相关图片已移至Grid类

        // 加载重新开始按钮图片
        loadimage(&imgRestart[0], _T("images/ReStart.png"), 30, 30);
        loadimage(&imgRestart[1], _T("images/ReStartWin.png"), 30, 30);
        loadimage(&imgRestart[2], _T("images/ReStartLose.png"), 30, 30);
        
        // 加载计数器数字图片（num0-num9）
        for (int i = 0; i <= 9; i++) {
            TCHAR filename[50];
            _stprintf(filename, _T("images/num%d.png"), i);
            loadimage(&digitsImg[i], filename, 15, 30);
        }
        
        // 加载Grid类所需的图片资源
        Grid::LoadImages(CELL_SIZE);
    } catch (...) {
        // 如果加载失败，显示错误消息
        MessageBox(NULL, _T("加载图片资源失败！请确保images文件夹中包含所有必要的图片。"), _T("错误"), MB_OK | MB_ICONERROR);
    }
}

// 绘制游戏界面
void GameUI::drawUI(const Board& board, int gameTime, bool debugMode, bool isGameOver, bool isWin) {
    // 清空屏幕
    cleardevice();
    
    // 绘制信息栏
    drawInfoPanel(board, gameTime, isGameOver, isWin);
    
    // 绘制游戏网格
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            const Grid& cell = board.getCell(i, j);
            int x = MARGIN + j * CELL_SIZE;
            int y = MARGIN + i * CELL_SIZE;
            
            // 使用Grid类中的draw方法绘制格子
            cell.draw(x, y, isGameOver, isWin, debugMode);
        }
    }
    
    // 刷新屏幕
    FlushBatchDraw();
}

// 绘制信息栏
void GameUI::drawInfoPanel(const Board& board, int gameTime, bool isGameOver, bool isWin) {
    // 绘制信息栏背景（黑框）
    setlinecolor(BLACK);
    setfillcolor(RGB(220, 220, 220));  // 浅灰色背景
    fillrectangle(infoRect.left, infoRect.top, infoRect.right, infoRect.bottom);
    
    // 计算信息栏内部元素位置
    int distanceFromCenter = (infoRect.right - infoRect.left) / 3;  // 均匀分布三个元素
    int centerY = (infoRect.top + infoRect.bottom) / 2;
    
    // 绘制游戏状态图标（笑脸）
    int imgIndex = 0;
    if (isGameOver) {
        imgIndex = isWin ? 1 : 2;
    }
    putimage(restartBtnPos.x - 15, centerY - 15, &imgRestart[imgIndex]);
    
    // 显示剩余雷数（左侧）
    int minesLeft = board.getRemainingMines();
    drawDigits(infoRect.left + distanceFromCenter / 2 - 22, centerY - 15, minesLeft, 3);
    
    // 显示游戏时间（右侧）
    drawDigits(infoRect.right - distanceFromCenter / 2 - 22, centerY - 15, gameTime, 3);
}

// 绘制数字（用于显示雷数和时间）
void GameUI::drawDigits(int x, int y, int number, int digits) {
    // 处理负数
    if (number < 0) {
        // 如果是负数，绘制负号并取绝对值
        settextcolor(RGB(255, 0, 0));
        outtextxy(x - 15, y + 7, _T("-"));
        number = abs(number);
    }
    
    // 确保数字不超过指定位数
    int maxValue = 1;
    for (int i = 0; i < digits; i++) maxValue *= 10;
    if (number >= maxValue) number = maxValue - 1;
    
    // 从高位到低位显示数字
    for (int i = digits - 1; i >= 0; i--) {
        int divisor = 1;
        for (int j = 0; j < i; j++) divisor *= 10;
        
        int digit = (number / divisor) % 10;
        putimage(x + (digits - 1 - i) * 15, y, &digitsImg[digit]);
    }
}

// 获取重新开始按钮位置
POINT GameUI::getRestartBtnPos() const {
    return restartBtnPos;
}

// 获取信息栏区域
RECT GameUI::getInfoRect() const {
    return infoRect;
}

//----------------------------------------------------------------
// Game 类实现
//----------------------------------------------------------------

// 游戏状态变化回调函数实现
void Game::GameStateChangeCallback(bool isMine, int remainingCells) {
    if (g_gameInstance) {
        if (isMine) {
            // 踩到地雷，游戏结束
            g_gameInstance->gameOver(false);
        } else {
            // 检查是否获胜
            g_gameInstance->checkWin(remainingCells);
        }
    }
}

// 静态回调函数，重置游戏时间
static void ResetGameCallback() {
    if (g_gameInstance) {
        g_gameInstance->restart();
    }
}

// 构造函数
Game::Game() : isRunning(false), startTime(0), gameTime(0), isDebugMode(false),
               isGameOver(false), isWin(false), remainingMines(MINE_COUNT) {
    // 设置全局实例，用于静态回调
    g_gameInstance = this;
}

// 析构函数
Game::~Game() {
}

// 游戏初始化
void Game::init() {
    // 初始化游戏棋盘
    board.initGrid();
    
    // 初始化游戏UI
    ui.init();
    
    // 重置游戏时间和状态
    gameTime = 0;
    isGameOver = false;
    isWin = false;
    remainingMines = MINE_COUNT;
    
    // 设置回调函数
    board.setResetCallback(ResetGameCallback);
    board.setStateCallback(GameStateChangeCallback);
}

// 游戏开始
void Game::start() {
    // 计算窗口尺寸，增加信息栏的高度
    int windowWidth = MARGIN * 2 + COLS * CELL_SIZE;
    int windowHeight = MARGIN * 2 + ROWS * CELL_SIZE + INFO_MARGIN;
    
    // 初始化窗口
    windowManager.initWindow(windowWidth, windowHeight, this);
    
    // 设置窗口图标
    windowManager.setIcon("images/logo.ico");
    
    // 注册窗口过程
    windowManager.registerWindowProcedure();
    
    // 初始化游戏
    init();
    
    // 启用批量绘制，提高性能
    BeginBatchDraw();
    
    // 设置游戏运行状态
    isRunning = true;
    
    // 进入游戏主循环
    gameLoop();
    
    // 关闭批量绘制
    EndBatchDraw();
    
    // 关闭窗口
    windowManager.closeWindow();
}

// 重新开始游戏
void Game::restart() {
    // 重置游戏棋盘
    board.restart();
    
    // 重置游戏时间和状态
    gameTime = 0;
    isGameOver = false;
    isWin = false;
    remainingMines = MINE_COUNT;
    
    // 重绘UI
    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin);
}

// 游戏结束处理
void Game::gameOver(bool win) {
    isGameOver = true;
    isWin = win;
    
    // 如果获胜，更新剩余雷数为0
    if (win) {
        remainingMines = 0;
    }
    
    // 显示所有地雷
    showAllMines();
    
    // 更新UI显示
    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin);
}

// 检查是否获胜
void Game::checkWin(int remainingCells) {
    if (remainingCells == 0) {
        // 剩余格子数为0，游戏获胜
        gameOver(true);
    }
}

// 显示所有地雷
void Game::showAllMines() {
    board.showAllMines(isWin);
}

// 更新剩余地雷数量
void Game::updateRemainingMines(int mines) {
    remainingMines = mines;
}

// 处理鼠标点击事件
void Game::handleMouseClick(int x, int y, bool isLeftClick) {
    // 如果游戏已结束，只允许点击重置按钮
    if (isGameOver) {
        // 检查是否点击了重置按钮
        POINT btnPos = ui.getRestartBtnPos();
        if (abs(x - btnPos.x) <= 15 && abs(y - btnPos.y) <= 15) {
            restart();
        }
        return;
    }
    
    // 检查是否点击了重置按钮
    POINT btnPos = ui.getRestartBtnPos();
    if (abs(x - btnPos.x) <= 15 && abs(y - btnPos.y) <= 15) {
        restart();
        return;
    }
    
    // 检查是否是第一次点击
    bool wasFirstClick = board.isFirstClickState();
    
    // 处理点击事件
    board.handleMouseClick(x, y, isLeftClick, ui.getRestartBtnPos());
    
    // 如果是第一次点击(从true变为false)，则开始计时
    if (wasFirstClick && !board.isFirstClickState()) {
        startTime = time(NULL);
    }
    
    // 更新界面
    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin);
}

// 游戏主循环
void Game::gameLoop() {
    // 绘制初始界面
    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin);
    
    // 自定义消息循环
    MSG msg;
    int lastSecond = 0;
    
    while (isRunning) {
        // 更新游戏时间
        updateGameTime();
        
        // 如果游戏已开始(非首次点击)且未结束，且时间变化了或处于调试模式，重绘UI
        if ((!board.isFirstClickState() && !isGameOver && gameTime != lastSecond) || isDebugMode) {
            lastSecond = gameTime;
            ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin);
        }
        
        // 处理Windows消息
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                isRunning = false;
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        
        // 处理鼠标消息
        if (MouseHit()) {
            MOUSEMSG mouseMsg = GetMouseMsg();
            
            switch (mouseMsg.uMsg) {
                case WM_LBUTTONDOWN:
                    handleMouseClick(mouseMsg.x, mouseMsg.y, true);
                    break;
                    
                case WM_RBUTTONDOWN:
                    handleMouseClick(mouseMsg.x, mouseMsg.y, false);
                    break;
            }
        }
        
        // 检测F9键是否按下（用于调试）
        if (GetAsyncKeyState(VK_F9) & 0x8000) {
            // 调用调试函数
            debugShowAllMines();
            // 延迟一段时间，避免重复触发
            Sleep(500);
        }
        
        // 添加一些延时，避免CPU占用过高
        Sleep(10);
    }
}

// 更新游戏时间
void Game::updateGameTime() {
    // 只有游戏已开始且未结束时才更新时间
    if (!board.isFirstClickState() && !isGameOver) {
        gameTime = static_cast<int>(time(NULL) - startTime);
    }
}

// 获取游戏时间
int Game::getGameTime() const {
    return gameTime;
}

// 调试函数：切换显示所有雷的位置
void Game::debugShowAllMines() {
    // 调用Board的调试函数并更新调试状态
    isDebugMode = board.debugShowAllMines();
    
    // 更新UI，显示变更后的界面
    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin);
} 
