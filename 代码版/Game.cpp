#define _CRT_SECURE_NO_WARNINGS 1
#include "Game.h"
#include <cstdio>

// 使用winmm库播放声音
#pragma comment(lib, "winmm.lib")

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
    // 将多字节字符串转换为TCHAR类型
    TCHAR tIconPath[MAX_PATH];
    #ifdef UNICODE
        // 如果是Unicode版本，需要将路径转换为宽字符
        MultiByteToWideChar(CP_ACP, 0, iconPath, -1, tIconPath, MAX_PATH);
    #else
        // ANSI版本直接复制
        strcpy(tIconPath, iconPath);
    #endif
    
    // 使用LoadImage函数加载图标
    hIcon = (HICON)LoadImage(
        NULL,                // 实例句柄为NULL
        tIconPath,           // 使用传入的图标路径
        IMAGE_ICON,          // 加载的是图标
        32,                  // 图标宽度
        32,                  // 图标高度
        LR_LOADFROMFILE      // 从文件加载
    );
    
    if (hIcon) {
        // 设置大图标和小图标
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    } else {
        // 图标加载失败，显示错误信息
        DWORD error = GetLastError();
        TCHAR errorMsg[256];
        _stprintf(errorMsg, _T("图标加载失败! 错误代码: %lu\n路径: %s"), error, tIconPath);
        MessageBox(hwnd, errorMsg, _T("图标加载错误"), MB_OK | MB_ICONERROR);
    }
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
    // 初始化使用默认尺寸，在drawUI时会更新
    updateUILayout(BEGINNER_COLS);
}

// 更新UI布局
void GameUI::updateUILayout(int cols) {
    // 计算信息栏位置
    int panelWidth = cols * CELL_SIZE;
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
    
    // 设置难度选择区域
    difficultyRect.left = panelX;
    difficultyRect.top = panelY - DIFFICULTY_HEIGHT - 5;  // 在信息栏上方
    difficultyRect.right = panelX + panelWidth;
    difficultyRect.bottom = panelY - 5;
    
    // 设置各难度文字区域
    int diffWidth = panelWidth / 3;
    
    beginnerRect.left = difficultyRect.left;
    beginnerRect.top = difficultyRect.top;
    beginnerRect.right = beginnerRect.left + diffWidth;
    beginnerRect.bottom = difficultyRect.bottom;
    
    intermediateRect.left = beginnerRect.right;
    intermediateRect.top = difficultyRect.top;
    intermediateRect.right = intermediateRect.left + diffWidth;
    intermediateRect.bottom = difficultyRect.bottom;
    
    expertRect.left = intermediateRect.right;
    expertRect.top = difficultyRect.top;
    expertRect.right = difficultyRect.right;
    expertRect.bottom = difficultyRect.bottom;
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
void GameUI::drawUI(const Board& board, int gameTime, bool debugMode, bool isGameOver, bool isWin, GameDifficulty difficulty) {
    // 更新UI布局以适应当前棋盘大小
    updateUILayout(board.getCols());
    
    // 清空屏幕
    cleardevice();
    
    // 绘制难度选择栏
    drawDifficultyPanel(difficulty);
    
    // 绘制信息栏
    drawInfoPanel(board, gameTime, isGameOver, isWin);
    
    // 绘制游戏网格
    int rows = board.getRows();
    int cols = board.getCols();
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
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

// 绘制难度选择栏
void GameUI::drawDifficultyPanel(GameDifficulty currentDifficulty) {
    // 绘制难度选择栏背景
    setlinecolor(BLACK);
    setfillcolor(RGB(220, 220, 220));  // 浅灰色背景
    fillrectangle(difficultyRect.left, difficultyRect.top, difficultyRect.right, difficultyRect.bottom);
    
    // 设置字体和颜色
    settextcolor(RGB(0, 0, 255));  // 蓝色文字
    settextstyle(18, 0, _T("宋体"));
    setbkmode(TRANSPARENT);
    
    // 绘制三个难度选项
    const TCHAR* difficulty_names[3] = { _T("基础"), _T("中级"), _T("专家") };
    RECT textRects[3] = { beginnerRect, intermediateRect, expertRect };
    
    // 绘制每个难度的文字
    for (int i = 0; i < 3; i++) {
        // 如果是当前选中的难度，使用加粗字体
        if ((i == 0 && currentDifficulty == GameDifficulty::BEGINNER) ||
            (i == 1 && currentDifficulty == GameDifficulty::INTERMEDIATE) ||
            (i == 2 && currentDifficulty == GameDifficulty::EXPERT)) {
            // 当前选中的难度，加粗显示
            settextstyle(18, 0, _T("宋体"), 0, 0, FW_BOLD, false, false, false);  // 加粗，无下划线
        } else {
            // 未选中的难度，普通显示
            settextstyle(18, 0, _T("宋体"), 0, 0, FW_NORMAL, false, false, false);  // 无下划线
        }
        
        // 绘制文字
        drawtext(difficulty_names[i], &textRects[i], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
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

// 获取难度选择区域
RECT GameUI::getDifficultyRect() const {
    return difficultyRect;
}

// 获取各难度区域
RECT GameUI::getBeginnerRect() const {
    return beginnerRect;
}

RECT GameUI::getIntermediateRect() const {
    return intermediateRect;
}

RECT GameUI::getExpertRect() const {
    return expertRect;
}

//----------------------------------------------------------------
// Game 类实现
//----------------------------------------------------------------

// 游戏状态变化回调函数实现
void Game::GameStateChangeCallback(bool isMine, int remainingCells) {
    if (g_gameInstance) {
        if (isMine) {
            // 踩到地雷，游戏结束
            g_gameInstance->playBoomSound(); // 播放爆炸音效
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
               isGameOver(false), isWin(false), remainingMines(BEGINNER_MINES), 
               currentDifficulty(GameDifficulty::BEGINNER),
               rows(BEGINNER_ROWS), cols(BEGINNER_COLS), 
               mineCount(BEGINNER_MINES), soundEnabled(true) {
    // 设置全局实例，用于静态回调
    g_gameInstance = this;
    
    // 初始化音效
    initSound();
}

// 析构函数
Game::~Game() {
    // 关闭音效
    closeSound();
}

// 初始化音效
void Game::initSound() {
    // 预加载音效文件
    mciSendString(_T("open sounds/click.wav alias clicksound"), NULL, 0, NULL);
    mciSendString(_T("open sounds/boom.wav alias boomsound"), NULL, 0, NULL);
}
// 播放点击音效
void Game::playClickSound() {
    if (soundEnabled) {
        mciSendString(_T("seek clicksound to start"), NULL, 0, NULL);
        mciSendString(_T("play clicksound"), NULL, 0, NULL);
    }
}

// 播放爆炸音效
void Game::playBoomSound() {
    if (soundEnabled) {
        mciSendString(_T("seek boomsound to start"), NULL, 0, NULL);
        mciSendString(_T("play boomsound"), NULL, 0, NULL);
    }
}

// 关闭所有音效
void Game::closeSound() {
    mciSendString(_T("close clicksound"), NULL, 0, NULL);
    mciSendString(_T("close boomsound"), NULL, 0, NULL);
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
    remainingMines = mineCount;
    
    // 设置回调函数
    board.setResetCallback(ResetGameCallback);
    board.setStateCallback(GameStateChangeCallback);
}

// 游戏开始
void Game::start() {
    // 计算窗口尺寸，增加信息栏和难度选择栏的高度
    int windowWidth = MARGIN * 2 + cols * CELL_SIZE;
    int windowHeight = MARGIN * 2 + rows * CELL_SIZE + INFO_MARGIN + DIFFICULTY_HEIGHT + 5; // 额外加5像素空间
    
    // 初始化窗口
    windowManager.initWindow(windowWidth, windowHeight, this);
    
    // 设置窗口图标
    windowManager.setIcon("images/Logo.ico");
    
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
    remainingMines = mineCount;
    
    // 重绘UI
    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin, currentDifficulty);
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
    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin, currentDifficulty);
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
    // 检查是否点击在难度选择区域
    if (isLeftClick && handleDifficultyClick(x, y)) {
        // 如果点击了难度选择区域并且切换了难度，不播放点击音效
        return;
    }
    
    // 如果游戏已结束，只允许点击重置按钮
    if (isGameOver) {
        // 检查是否点击了重置按钮
        POINT btnPos = ui.getRestartBtnPos();
        if (abs(x - btnPos.x) <= 15 && abs(y - btnPos.y) <= 15) {
            // 重置游戏（不播放音效）
            restart();
        }
        return;
    }
    
    // 检查是否点击了重置按钮
    POINT btnPos = ui.getRestartBtnPos();
    if (abs(x - btnPos.x) <= 15 && abs(y - btnPos.y) <= 15) {
        // 重置游戏（不播放音效）
        restart();
        return;
    }
    
    // 检查是否是第一次点击
    bool wasFirstClick = board.isFirstClickState();
    
    // 检查点击位置是否在有效范围内且是未揭开的格子
    bool canPlaySound = false;
    if (isLeftClick && x >= MARGIN && x < MARGIN + cols * CELL_SIZE && 
        y >= MARGIN && y < MARGIN + rows * CELL_SIZE) {
        
        int row = (y - MARGIN) / CELL_SIZE;
        int col = (x - MARGIN) / CELL_SIZE;
        
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            const Grid& cell = board.getCell(row, col);
            // 只有在格子未打开且未标记时才能播放音效
            if (!cell.isOpened() && !cell.isFlagged() && !cell.isMine()) {
                canPlaySound = true;
            }
        }
    }
    
    // 处理点击事件
    board.handleMouseClick(x, y, isLeftClick, ui.getRestartBtnPos());
    
    // 如果是第一次点击(从true变为false)，则开始计时
    if (wasFirstClick && !board.isFirstClickState()) {
        startTime = time(NULL);
        playClickSound(); // 只在第一次点击时播放音效
    }
    else if (isLeftClick && !isGameOver && !board.isFirstClickState() && canPlaySound) {
        // 只有在点击未揭开的非地雷格子时才播放点击音效
        playClickSound();
    }
}

// 游戏主循环
void Game::gameLoop() {
    // 绘制初始界面
    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin, currentDifficulty);
    
    // 自定义消息循环
    MSG msg;
    int lastSecond = 0;
    
    // 性能优化变量
    DWORD lastDrawTime = GetTickCount();
    const DWORD FRAME_DELAY = 50;  // 限制为每秒约20帧
    
    while (isRunning) {
        // 更新游戏时间
        updateGameTime();
        
        DWORD currentTime = GetTickCount();
        bool needRedraw = false;
        
        // 如果游戏已开始(非首次点击)且未结束，且时间变化了，标记需要重绘
        if (!board.isFirstClickState() && !isGameOver && gameTime != lastSecond) {
            lastSecond = gameTime;
            needRedraw = true;
        }
        
        // 调试模式下，控制重绘频率，不要每次循环都重绘
        if (isDebugMode && (currentTime - lastDrawTime >= FRAME_DELAY)) {
            needRedraw = true;
        }
        
        // 如果需要重绘且已经过了足够时间，则重绘UI
        if (needRedraw && (currentTime - lastDrawTime >= FRAME_DELAY)) {
            ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin, currentDifficulty);
            lastDrawTime = currentTime;
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
                    // 点击后强制重绘
                    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin, currentDifficulty);
                    lastDrawTime = GetTickCount();
                    break;
                    
                case WM_RBUTTONDOWN:
                    handleMouseClick(mouseMsg.x, mouseMsg.y, false);
                    // 点击后强制重绘
                    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin, currentDifficulty);
                    lastDrawTime = GetTickCount();
                    break;
            }
        }
        
        // 检测F9键是否按下（用于调试）
        if (GetAsyncKeyState(VK_F9) & 0x8000) {
            // 调用调试函数
            debugShowAllMines();
            // 强制重绘
            ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin, currentDifficulty);
            lastDrawTime = GetTickCount();
            // 延迟一段时间，避免重复触发
            Sleep(500);
        }
        
        // 添加一些延时，避免CPU占用过高
        Sleep(5);  // 减少Sleep时间，使游戏更流畅
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
    // 获取当前调试模式状态
    bool currentDebugMode = board.isDebugMode();
    
    // 调用Board的调试函数并更新调试状态
    isDebugMode = board.debugShowAllMines();
    
    // 只有在状态确实发生变化时才重绘UI
    if (currentDebugMode != isDebugMode) {
        ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin, currentDifficulty);
    }
}

// 切换游戏难度
void Game::setDifficulty(GameDifficulty difficulty) {
    // 记录当前难度
    currentDifficulty = difficulty;
    
    // 根据难度设置棋盘大小和地雷数量
    switch (difficulty) {
        case GameDifficulty::BEGINNER:
            rows = BEGINNER_ROWS;
            cols = BEGINNER_COLS;
            mineCount = BEGINNER_MINES;
            break;
            
        case GameDifficulty::INTERMEDIATE:
            rows = INTERMEDIATE_ROWS;
            cols = INTERMEDIATE_COLS;
            mineCount = INTERMEDIATE_MINES;
            break;
            
        case GameDifficulty::EXPERT:
            rows = EXPERT_ROWS;
            cols = EXPERT_COLS;
            mineCount = EXPERT_MINES;
            break;
    }
    
    // 设置棋盘尺寸
    board.setSize(rows, cols, mineCount);
    
    // 更新剩余地雷数量
    remainingMines = mineCount;
    
    // 计算窗口尺寸，增加信息栏和难度选择栏的高度
    int windowWidth = MARGIN * 2 + cols * CELL_SIZE;
    int windowHeight = MARGIN * 2 + rows * CELL_SIZE + INFO_MARGIN + DIFFICULTY_HEIGHT + 5; // 额外加5像素空间
    
    // 调整窗口大小
    windowManager.resizeWindow(windowWidth, windowHeight);
    
    // 重新设置窗口图标，确保在窗口大小调整后图标依然存在
    windowManager.setIcon("images/Logo.ico");
    
    // 重置游戏状态
    gameTime = 0;
    isGameOver = false;
    isWin = false;
    
    // 重绘UI
    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin, currentDifficulty);
}

// 处理难度选择区域的点击
bool Game::handleDifficultyClick(int x, int y) {
    // 获取难度选择区域
    RECT beginnerRect = ui.getBeginnerRect();
    RECT intermediateRect = ui.getIntermediateRect();
    RECT expertRect = ui.getExpertRect();
    
    // 检查是否点击在难度选择区域
    if (PtInRect(&beginnerRect, {x, y})) {
        // 如果当前难度不是初级，则切换
        if (currentDifficulty != GameDifficulty::BEGINNER) {
            setDifficulty(GameDifficulty::BEGINNER);
            return true;
        }
    }
    else if (PtInRect(&intermediateRect, {x, y})) {
        // 如果当前难度不是中级，则切换
        if (currentDifficulty != GameDifficulty::INTERMEDIATE) {
            setDifficulty(GameDifficulty::INTERMEDIATE);
            return true;
        }
    }
    else if (PtInRect(&expertRect, {x, y})) {
        // 如果当前难度不是专家，则切换
        if (currentDifficulty != GameDifficulty::EXPERT) {
            setDifficulty(GameDifficulty::EXPERT);
            return true;
        }
    }
    
    return false;
} 
