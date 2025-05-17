#define _CRT_SECURE_NO_WARNINGS 1
#include "Game.h"
#include <cstdio>

// ȫ�ֱ��� - ԭʼ���ڹ��̺���ָ��
static WNDPROC g_oldWndProc = NULL;
static Game* g_gameInstance = NULL;  // ȫ����Ϸʵ��ָ�룬���ھ�̬�ص�

// ������Ϣ������
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // ����ԭʼ���ڹ���
    return g_oldWndProc ? CallWindowProc(g_oldWndProc, hwnd, uMsg, wParam, lParam) : DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//----------------------------------------------------------------
// WindowManager ��ʵ��
//----------------------------------------------------------------

// ���캯��
WindowManager::WindowManager() : hwnd(NULL), hIcon(NULL), gameInstance(NULL) {
}

// ��������
WindowManager::~WindowManager() {
    // �ͷ�ͼ����Դ
    if (hIcon) {
        DestroyIcon(hIcon);
        hIcon = NULL;
    }
}

// ��ʼ������
void WindowManager::initWindow(int width, int height, Game* game) {
    gameInstance = game;
    
    // ������ʾ����
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    // ����ָ��λ�úʹ�С�Ĵ���
    initgraph(width, height);
    hwnd = GetHWnd(); // ��ȡ���ھ��
    SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    
    // ���ô��ڱ���
    SetWindowText(hwnd, _T("ɨ����Ϸ"));
    
    // ���ñ���ɫ
    setbkcolor(RGB(240, 240, 240));
    cleardevice();
}

// ���ô���ͼ��
void WindowManager::setIcon(const char* iconPath) {
    // ����Ӧ�ó���ͼ��
    int len = MultiByteToWideChar(CP_ACP, 0, iconPath, -1, NULL, 0);
    wchar_t* wIconPath = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, iconPath, -1, wIconPath, len);
    
    // ʹ�ÿ��ַ��汾��LoadImage (LoadImageW)
    hIcon = (HICON)LoadImageW(NULL, wIconPath, IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    if (hIcon) {
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    }
    
    delete[] wIconPath;
}

// ��ȡ���ھ��
HWND WindowManager::getHWnd() const {
    return hwnd;
}

// ע�ᴰ�ڹ���
void WindowManager::registerWindowProcedure() {
    // ע�ᴰ����Ϣ������
    g_oldWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WindowProcedure);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)gameInstance);
}

// �رմ���
void WindowManager::closeWindow() {
    // �ر�ͼ�δ���
    closegraph();
}

// �������ڴ�С
void WindowManager::resizeWindow(int width, int height) {
    // �رյ�ǰͼ�λ���
    closegraph();
    
    // ������ʾ����
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    // ����ָ��λ�úʹ�С�Ĵ���
    initgraph(width, height);
    hwnd = GetHWnd();
    SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    
    // ���ñ���ɫ������
    setbkcolor(RGB(240, 240, 240));
    cleardevice();
    
    // ����ע�ᴰ�ڹ���
    g_oldWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WindowProcedure);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)gameInstance);
    
    // ���ô��ڱ���
    SetWindowText(hwnd, _T("ɨ����Ϸ"));
}

//----------------------------------------------------------------
// GameUI ��ʵ��
//----------------------------------------------------------------

// ���캯��
GameUI::GameUI() {
    // ������Ϣ��λ��
    int panelWidth = COLS * CELL_SIZE;
    int panelHeight = INFO_HEIGHT;
    int panelX = MARGIN;
    int panelY = MARGIN - INFO_HEIGHT - INFO_MARGIN;  // �����������Ϸ�
    
    // ������Ϣ������
    infoRect.left = panelX;
    infoRect.top = panelY;
    infoRect.right = panelX + panelWidth;
    infoRect.bottom = panelY + panelHeight;
    
    // �������¿�ʼ��ťλ��
    restartBtnPos.x = panelX + panelWidth / 2;
    restartBtnPos.y = panelY + panelHeight / 2;
}

// ��ʼ��UI
void GameUI::init() {
    // ����ͼƬ��Դ
    loadImages();
}

// ����ͼƬ��Դ
void GameUI::loadImages() {
    try {
        // ���ظ������ͼƬ������Grid��

        // �������¿�ʼ��ťͼƬ
        loadimage(&imgRestart[0], _T("images/ReStart.png"), 30, 30);
        loadimage(&imgRestart[1], _T("images/ReStartWin.png"), 30, 30);
        loadimage(&imgRestart[2], _T("images/ReStartLose.png"), 30, 30);
        
        // ���ؼ���������ͼƬ��num0-num9��
        for (int i = 0; i <= 9; i++) {
            TCHAR filename[50];
            _stprintf(filename, _T("images/num%d.png"), i);
            loadimage(&digitsImg[i], filename, 15, 30);
        }
        
        // ����Grid�������ͼƬ��Դ
        Grid::LoadImages(CELL_SIZE);
    } catch (...) {
        // �������ʧ�ܣ���ʾ������Ϣ
        MessageBox(NULL, _T("����ͼƬ��Դʧ�ܣ���ȷ��images�ļ����а������б�Ҫ��ͼƬ��"), _T("����"), MB_OK | MB_ICONERROR);
    }
}

// ������Ϸ����
void GameUI::drawUI(const Board& board, int gameTime, bool debugMode, bool isGameOver, bool isWin) {
    // �����Ļ
    cleardevice();
    
    // ������Ϣ��
    drawInfoPanel(board, gameTime, isGameOver, isWin);
    
    // ������Ϸ����
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            const Grid& cell = board.getCell(i, j);
            int x = MARGIN + j * CELL_SIZE;
            int y = MARGIN + i * CELL_SIZE;
            
            // ʹ��Grid���е�draw�������Ƹ���
            cell.draw(x, y, isGameOver, isWin, debugMode);
        }
    }
    
    // ˢ����Ļ
    FlushBatchDraw();
}

// ������Ϣ��
void GameUI::drawInfoPanel(const Board& board, int gameTime, bool isGameOver, bool isWin) {
    // ������Ϣ���������ڿ�
    setlinecolor(BLACK);
    setfillcolor(RGB(220, 220, 220));  // ǳ��ɫ����
    fillrectangle(infoRect.left, infoRect.top, infoRect.right, infoRect.bottom);
    
    // ������Ϣ���ڲ�Ԫ��λ��
    int distanceFromCenter = (infoRect.right - infoRect.left) / 3;  // ���ȷֲ�����Ԫ��
    int centerY = (infoRect.top + infoRect.bottom) / 2;
    
    // ������Ϸ״̬ͼ�꣨Ц����
    int imgIndex = 0;
    if (isGameOver) {
        imgIndex = isWin ? 1 : 2;
    }
    putimage(restartBtnPos.x - 15, centerY - 15, &imgRestart[imgIndex]);
    
    // ��ʾʣ����������ࣩ
    int minesLeft = board.getRemainingMines();
    drawDigits(infoRect.left + distanceFromCenter / 2 - 22, centerY - 15, minesLeft, 3);
    
    // ��ʾ��Ϸʱ�䣨�Ҳࣩ
    drawDigits(infoRect.right - distanceFromCenter / 2 - 22, centerY - 15, gameTime, 3);
}

// �������֣�������ʾ������ʱ�䣩
void GameUI::drawDigits(int x, int y, int number, int digits) {
    // ������
    if (number < 0) {
        // ����Ǹ��������Ƹ��Ų�ȡ����ֵ
        settextcolor(RGB(255, 0, 0));
        outtextxy(x - 15, y + 7, _T("-"));
        number = abs(number);
    }
    
    // ȷ�����ֲ�����ָ��λ��
    int maxValue = 1;
    for (int i = 0; i < digits; i++) maxValue *= 10;
    if (number >= maxValue) number = maxValue - 1;
    
    // �Ӹ�λ����λ��ʾ����
    for (int i = digits - 1; i >= 0; i--) {
        int divisor = 1;
        for (int j = 0; j < i; j++) divisor *= 10;
        
        int digit = (number / divisor) % 10;
        putimage(x + (digits - 1 - i) * 15, y, &digitsImg[digit]);
    }
}

// ��ȡ���¿�ʼ��ťλ��
POINT GameUI::getRestartBtnPos() const {
    return restartBtnPos;
}

// ��ȡ��Ϣ������
RECT GameUI::getInfoRect() const {
    return infoRect;
}

//----------------------------------------------------------------
// Game ��ʵ��
//----------------------------------------------------------------

// ��Ϸ״̬�仯�ص�����ʵ��
void Game::GameStateChangeCallback(bool isMine, int remainingCells) {
    if (g_gameInstance) {
        if (isMine) {
            // �ȵ����ף���Ϸ����
            g_gameInstance->gameOver(false);
        } else {
            // ����Ƿ��ʤ
            g_gameInstance->checkWin(remainingCells);
        }
    }
}

// ��̬�ص�������������Ϸʱ��
static void ResetGameCallback() {
    if (g_gameInstance) {
        g_gameInstance->restart();
    }
}

// ���캯��
Game::Game() : isRunning(false), startTime(0), gameTime(0), isDebugMode(false),
               isGameOver(false), isWin(false), remainingMines(MINE_COUNT) {
    // ����ȫ��ʵ�������ھ�̬�ص�
    g_gameInstance = this;
}

// ��������
Game::~Game() {
}

// ��Ϸ��ʼ��
void Game::init() {
    // ��ʼ����Ϸ����
    board.initGrid();
    
    // ��ʼ����ϷUI
    ui.init();
    
    // ������Ϸʱ���״̬
    gameTime = 0;
    isGameOver = false;
    isWin = false;
    remainingMines = MINE_COUNT;
    
    // ���ûص�����
    board.setResetCallback(ResetGameCallback);
    board.setStateCallback(GameStateChangeCallback);
}

// ��Ϸ��ʼ
void Game::start() {
    // ���㴰�ڳߴ磬������Ϣ���ĸ߶�
    int windowWidth = MARGIN * 2 + COLS * CELL_SIZE;
    int windowHeight = MARGIN * 2 + ROWS * CELL_SIZE + INFO_MARGIN;
    
    // ��ʼ������
    windowManager.initWindow(windowWidth, windowHeight, this);
    
    // ���ô���ͼ��
    windowManager.setIcon("images/logo.ico");
    
    // ע�ᴰ�ڹ���
    windowManager.registerWindowProcedure();
    
    // ��ʼ����Ϸ
    init();
    
    // �����������ƣ��������
    BeginBatchDraw();
    
    // ������Ϸ����״̬
    isRunning = true;
    
    // ������Ϸ��ѭ��
    gameLoop();
    
    // �ر���������
    EndBatchDraw();
    
    // �رմ���
    windowManager.closeWindow();
}

// ���¿�ʼ��Ϸ
void Game::restart() {
    // ������Ϸ����
    board.restart();
    
    // ������Ϸʱ���״̬
    gameTime = 0;
    isGameOver = false;
    isWin = false;
    remainingMines = MINE_COUNT;
    
    // �ػ�UI
    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin);
}

// ��Ϸ��������
void Game::gameOver(bool win) {
    isGameOver = true;
    isWin = win;
    
    // �����ʤ������ʣ������Ϊ0
    if (win) {
        remainingMines = 0;
    }
    
    // ��ʾ���е���
    showAllMines();
    
    // ����UI��ʾ
    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin);
}

// ����Ƿ��ʤ
void Game::checkWin(int remainingCells) {
    if (remainingCells == 0) {
        // ʣ�������Ϊ0����Ϸ��ʤ
        gameOver(true);
    }
}

// ��ʾ���е���
void Game::showAllMines() {
    board.showAllMines(isWin);
}

// ����ʣ���������
void Game::updateRemainingMines(int mines) {
    remainingMines = mines;
}

// ����������¼�
void Game::handleMouseClick(int x, int y, bool isLeftClick) {
    // �����Ϸ�ѽ�����ֻ���������ð�ť
    if (isGameOver) {
        // ����Ƿ��������ð�ť
        POINT btnPos = ui.getRestartBtnPos();
        if (abs(x - btnPos.x) <= 15 && abs(y - btnPos.y) <= 15) {
            restart();
        }
        return;
    }
    
    // ����Ƿ��������ð�ť
    POINT btnPos = ui.getRestartBtnPos();
    if (abs(x - btnPos.x) <= 15 && abs(y - btnPos.y) <= 15) {
        restart();
        return;
    }
    
    // ����Ƿ��ǵ�һ�ε��
    bool wasFirstClick = board.isFirstClickState();
    
    // �������¼�
    board.handleMouseClick(x, y, isLeftClick, ui.getRestartBtnPos());
    
    // ����ǵ�һ�ε��(��true��Ϊfalse)����ʼ��ʱ
    if (wasFirstClick && !board.isFirstClickState()) {
        startTime = time(NULL);
    }
    
    // ���½���
    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin);
}

// ��Ϸ��ѭ��
void Game::gameLoop() {
    // ���Ƴ�ʼ����
    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin);
    
    // �Զ�����Ϣѭ��
    MSG msg;
    int lastSecond = 0;
    
    while (isRunning) {
        // ������Ϸʱ��
        updateGameTime();
        
        // �����Ϸ�ѿ�ʼ(���״ε��)��δ��������ʱ��仯�˻��ڵ���ģʽ���ػ�UI
        if ((!board.isFirstClickState() && !isGameOver && gameTime != lastSecond) || isDebugMode) {
            lastSecond = gameTime;
            ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin);
        }
        
        // ����Windows��Ϣ
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                isRunning = false;
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        
        // ���������Ϣ
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
        
        // ���F9���Ƿ��£����ڵ��ԣ�
        if (GetAsyncKeyState(VK_F9) & 0x8000) {
            // ���õ��Ժ���
            debugShowAllMines();
            // �ӳ�һ��ʱ�䣬�����ظ�����
            Sleep(500);
        }
        
        // ���һЩ��ʱ������CPUռ�ù���
        Sleep(10);
    }
}

// ������Ϸʱ��
void Game::updateGameTime() {
    // ֻ����Ϸ�ѿ�ʼ��δ����ʱ�Ÿ���ʱ��
    if (!board.isFirstClickState() && !isGameOver) {
        gameTime = static_cast<int>(time(NULL) - startTime);
    }
}

// ��ȡ��Ϸʱ��
int Game::getGameTime() const {
    return gameTime;
}

// ���Ժ������л���ʾ�����׵�λ��
void Game::debugShowAllMines() {
    // ����Board�ĵ��Ժ��������µ���״̬
    isDebugMode = board.debugShowAllMines();
    
    // ����UI����ʾ�����Ľ���
    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin);
} 
