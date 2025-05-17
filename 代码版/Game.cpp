#define _CRT_SECURE_NO_WARNINGS 1
#include "Game.h"
#include <cstdio>

// ʹ��winmm�ⲥ������
#pragma comment(lib, "winmm.lib")

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
    // �����ֽ��ַ���ת��ΪTCHAR����
    TCHAR tIconPath[MAX_PATH];
    #ifdef UNICODE
        // �����Unicode�汾����Ҫ��·��ת��Ϊ���ַ�
        MultiByteToWideChar(CP_ACP, 0, iconPath, -1, tIconPath, MAX_PATH);
    #else
        // ANSI�汾ֱ�Ӹ���
        strcpy(tIconPath, iconPath);
    #endif
    
    // ʹ��LoadImage��������ͼ��
    hIcon = (HICON)LoadImage(
        NULL,                // ʵ�����ΪNULL
        tIconPath,           // ʹ�ô����ͼ��·��
        IMAGE_ICON,          // ���ص���ͼ��
        32,                  // ͼ����
        32,                  // ͼ��߶�
        LR_LOADFROMFILE      // ���ļ�����
    );
    
    if (hIcon) {
        // ���ô�ͼ���Сͼ��
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    } else {
        // ͼ�����ʧ�ܣ���ʾ������Ϣ
        DWORD error = GetLastError();
        TCHAR errorMsg[256];
        _stprintf(errorMsg, _T("ͼ�����ʧ��! �������: %lu\n·��: %s"), error, tIconPath);
        MessageBox(hwnd, errorMsg, _T("ͼ����ش���"), MB_OK | MB_ICONERROR);
    }
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
    // ��ʼ��ʹ��Ĭ�ϳߴ磬��drawUIʱ�����
    updateUILayout(BEGINNER_COLS);
}

// ����UI����
void GameUI::updateUILayout(int cols) {
    // ������Ϣ��λ��
    int panelWidth = cols * CELL_SIZE;
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
    
    // �����Ѷ�ѡ������
    difficultyRect.left = panelX;
    difficultyRect.top = panelY - DIFFICULTY_HEIGHT - 5;  // ����Ϣ���Ϸ�
    difficultyRect.right = panelX + panelWidth;
    difficultyRect.bottom = panelY - 5;
    
    // ���ø��Ѷ���������
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
void GameUI::drawUI(const Board& board, int gameTime, bool debugMode, bool isGameOver, bool isWin, GameDifficulty difficulty) {
    // ����UI��������Ӧ��ǰ���̴�С
    updateUILayout(board.getCols());
    
    // �����Ļ
    cleardevice();
    
    // �����Ѷ�ѡ����
    drawDifficultyPanel(difficulty);
    
    // ������Ϣ��
    drawInfoPanel(board, gameTime, isGameOver, isWin);
    
    // ������Ϸ����
    int rows = board.getRows();
    int cols = board.getCols();
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
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

// �����Ѷ�ѡ����
void GameUI::drawDifficultyPanel(GameDifficulty currentDifficulty) {
    // �����Ѷ�ѡ��������
    setlinecolor(BLACK);
    setfillcolor(RGB(220, 220, 220));  // ǳ��ɫ����
    fillrectangle(difficultyRect.left, difficultyRect.top, difficultyRect.right, difficultyRect.bottom);
    
    // �����������ɫ
    settextcolor(RGB(0, 0, 255));  // ��ɫ����
    settextstyle(18, 0, _T("����"));
    setbkmode(TRANSPARENT);
    
    // ���������Ѷ�ѡ��
    const TCHAR* difficulty_names[3] = { _T("����"), _T("�м�"), _T("ר��") };
    RECT textRects[3] = { beginnerRect, intermediateRect, expertRect };
    
    // ����ÿ���Ѷȵ�����
    for (int i = 0; i < 3; i++) {
        // ����ǵ�ǰѡ�е��Ѷȣ�ʹ�üӴ�����
        if ((i == 0 && currentDifficulty == GameDifficulty::BEGINNER) ||
            (i == 1 && currentDifficulty == GameDifficulty::INTERMEDIATE) ||
            (i == 2 && currentDifficulty == GameDifficulty::EXPERT)) {
            // ��ǰѡ�е��Ѷȣ��Ӵ���ʾ
            settextstyle(18, 0, _T("����"), 0, 0, FW_BOLD, false, false, false);  // �Ӵ֣����»���
        } else {
            // δѡ�е��Ѷȣ���ͨ��ʾ
            settextstyle(18, 0, _T("����"), 0, 0, FW_NORMAL, false, false, false);  // ���»���
        }
        
        // ��������
        drawtext(difficulty_names[i], &textRects[i], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
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

// ��ȡ�Ѷ�ѡ������
RECT GameUI::getDifficultyRect() const {
    return difficultyRect;
}

// ��ȡ���Ѷ�����
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
// Game ��ʵ��
//----------------------------------------------------------------

// ��Ϸ״̬�仯�ص�����ʵ��
void Game::GameStateChangeCallback(bool isMine, int remainingCells) {
    if (g_gameInstance) {
        if (isMine) {
            // �ȵ����ף���Ϸ����
            g_gameInstance->playBoomSound(); // ���ű�ը��Ч
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
               isGameOver(false), isWin(false), remainingMines(BEGINNER_MINES), 
               currentDifficulty(GameDifficulty::BEGINNER),
               rows(BEGINNER_ROWS), cols(BEGINNER_COLS), 
               mineCount(BEGINNER_MINES), soundEnabled(true) {
    // ����ȫ��ʵ�������ھ�̬�ص�
    g_gameInstance = this;
    
    // ��ʼ����Ч
    initSound();
}

// ��������
Game::~Game() {
    // �ر���Ч
    closeSound();
}

// ��ʼ����Ч
void Game::initSound() {
    // Ԥ������Ч�ļ�
    mciSendString(_T("open sounds/click.wav alias clicksound"), NULL, 0, NULL);
    mciSendString(_T("open sounds/boom.wav alias boomsound"), NULL, 0, NULL);
}
// ���ŵ����Ч
void Game::playClickSound() {
    if (soundEnabled) {
        mciSendString(_T("seek clicksound to start"), NULL, 0, NULL);
        mciSendString(_T("play clicksound"), NULL, 0, NULL);
    }
}

// ���ű�ը��Ч
void Game::playBoomSound() {
    if (soundEnabled) {
        mciSendString(_T("seek boomsound to start"), NULL, 0, NULL);
        mciSendString(_T("play boomsound"), NULL, 0, NULL);
    }
}

// �ر�������Ч
void Game::closeSound() {
    mciSendString(_T("close clicksound"), NULL, 0, NULL);
    mciSendString(_T("close boomsound"), NULL, 0, NULL);
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
    remainingMines = mineCount;
    
    // ���ûص�����
    board.setResetCallback(ResetGameCallback);
    board.setStateCallback(GameStateChangeCallback);
}

// ��Ϸ��ʼ
void Game::start() {
    // ���㴰�ڳߴ磬������Ϣ�����Ѷ�ѡ�����ĸ߶�
    int windowWidth = MARGIN * 2 + cols * CELL_SIZE;
    int windowHeight = MARGIN * 2 + rows * CELL_SIZE + INFO_MARGIN + DIFFICULTY_HEIGHT + 5; // �����5���ؿռ�
    
    // ��ʼ������
    windowManager.initWindow(windowWidth, windowHeight, this);
    
    // ���ô���ͼ��
    windowManager.setIcon("images/Logo.ico");
    
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
    remainingMines = mineCount;
    
    // �ػ�UI
    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin, currentDifficulty);
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
    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin, currentDifficulty);
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
    // ����Ƿ������Ѷ�ѡ������
    if (isLeftClick && handleDifficultyClick(x, y)) {
        // ���������Ѷ�ѡ���������л����Ѷȣ������ŵ����Ч
        return;
    }
    
    // �����Ϸ�ѽ�����ֻ���������ð�ť
    if (isGameOver) {
        // ����Ƿ��������ð�ť
        POINT btnPos = ui.getRestartBtnPos();
        if (abs(x - btnPos.x) <= 15 && abs(y - btnPos.y) <= 15) {
            // ������Ϸ����������Ч��
            restart();
        }
        return;
    }
    
    // ����Ƿ��������ð�ť
    POINT btnPos = ui.getRestartBtnPos();
    if (abs(x - btnPos.x) <= 15 && abs(y - btnPos.y) <= 15) {
        // ������Ϸ����������Ч��
        restart();
        return;
    }
    
    // ����Ƿ��ǵ�һ�ε��
    bool wasFirstClick = board.isFirstClickState();
    
    // �����λ���Ƿ�����Ч��Χ������δ�ҿ��ĸ���
    bool canPlaySound = false;
    if (isLeftClick && x >= MARGIN && x < MARGIN + cols * CELL_SIZE && 
        y >= MARGIN && y < MARGIN + rows * CELL_SIZE) {
        
        int row = (y - MARGIN) / CELL_SIZE;
        int col = (x - MARGIN) / CELL_SIZE;
        
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            const Grid& cell = board.getCell(row, col);
            // ֻ���ڸ���δ����δ���ʱ���ܲ�����Ч
            if (!cell.isOpened() && !cell.isFlagged() && !cell.isMine()) {
                canPlaySound = true;
            }
        }
    }
    
    // �������¼�
    board.handleMouseClick(x, y, isLeftClick, ui.getRestartBtnPos());
    
    // ����ǵ�һ�ε��(��true��Ϊfalse)����ʼ��ʱ
    if (wasFirstClick && !board.isFirstClickState()) {
        startTime = time(NULL);
        playClickSound(); // ֻ�ڵ�һ�ε��ʱ������Ч
    }
    else if (isLeftClick && !isGameOver && !board.isFirstClickState() && canPlaySound) {
        // ֻ���ڵ��δ�ҿ��ķǵ��׸���ʱ�Ų��ŵ����Ч
        playClickSound();
    }
}

// ��Ϸ��ѭ��
void Game::gameLoop() {
    // ���Ƴ�ʼ����
    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin, currentDifficulty);
    
    // �Զ�����Ϣѭ��
    MSG msg;
    int lastSecond = 0;
    
    // �����Ż�����
    DWORD lastDrawTime = GetTickCount();
    const DWORD FRAME_DELAY = 50;  // ����Ϊÿ��Լ20֡
    
    while (isRunning) {
        // ������Ϸʱ��
        updateGameTime();
        
        DWORD currentTime = GetTickCount();
        bool needRedraw = false;
        
        // �����Ϸ�ѿ�ʼ(���״ε��)��δ��������ʱ��仯�ˣ������Ҫ�ػ�
        if (!board.isFirstClickState() && !isGameOver && gameTime != lastSecond) {
            lastSecond = gameTime;
            needRedraw = true;
        }
        
        // ����ģʽ�£������ػ�Ƶ�ʣ���Ҫÿ��ѭ�����ػ�
        if (isDebugMode && (currentTime - lastDrawTime >= FRAME_DELAY)) {
            needRedraw = true;
        }
        
        // �����Ҫ�ػ����Ѿ������㹻ʱ�䣬���ػ�UI
        if (needRedraw && (currentTime - lastDrawTime >= FRAME_DELAY)) {
            ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin, currentDifficulty);
            lastDrawTime = currentTime;
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
                    // �����ǿ���ػ�
                    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin, currentDifficulty);
                    lastDrawTime = GetTickCount();
                    break;
                    
                case WM_RBUTTONDOWN:
                    handleMouseClick(mouseMsg.x, mouseMsg.y, false);
                    // �����ǿ���ػ�
                    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin, currentDifficulty);
                    lastDrawTime = GetTickCount();
                    break;
            }
        }
        
        // ���F9���Ƿ��£����ڵ��ԣ�
        if (GetAsyncKeyState(VK_F9) & 0x8000) {
            // ���õ��Ժ���
            debugShowAllMines();
            // ǿ���ػ�
            ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin, currentDifficulty);
            lastDrawTime = GetTickCount();
            // �ӳ�һ��ʱ�䣬�����ظ�����
            Sleep(500);
        }
        
        // ���һЩ��ʱ������CPUռ�ù���
        Sleep(5);  // ����Sleepʱ�䣬ʹ��Ϸ������
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
    // ��ȡ��ǰ����ģʽ״̬
    bool currentDebugMode = board.isDebugMode();
    
    // ����Board�ĵ��Ժ��������µ���״̬
    isDebugMode = board.debugShowAllMines();
    
    // ֻ����״̬ȷʵ�����仯ʱ���ػ�UI
    if (currentDebugMode != isDebugMode) {
        ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin, currentDifficulty);
    }
}

// �л���Ϸ�Ѷ�
void Game::setDifficulty(GameDifficulty difficulty) {
    // ��¼��ǰ�Ѷ�
    currentDifficulty = difficulty;
    
    // �����Ѷ��������̴�С�͵�������
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
    
    // �������̳ߴ�
    board.setSize(rows, cols, mineCount);
    
    // ����ʣ���������
    remainingMines = mineCount;
    
    // ���㴰�ڳߴ磬������Ϣ�����Ѷ�ѡ�����ĸ߶�
    int windowWidth = MARGIN * 2 + cols * CELL_SIZE;
    int windowHeight = MARGIN * 2 + rows * CELL_SIZE + INFO_MARGIN + DIFFICULTY_HEIGHT + 5; // �����5���ؿռ�
    
    // �������ڴ�С
    windowManager.resizeWindow(windowWidth, windowHeight);
    
    // �������ô���ͼ�꣬ȷ���ڴ��ڴ�С������ͼ����Ȼ����
    windowManager.setIcon("images/Logo.ico");
    
    // ������Ϸ״̬
    gameTime = 0;
    isGameOver = false;
    isWin = false;
    
    // �ػ�UI
    ui.drawUI(board, gameTime, isDebugMode, isGameOver, isWin, currentDifficulty);
}

// �����Ѷ�ѡ������ĵ��
bool Game::handleDifficultyClick(int x, int y) {
    // ��ȡ�Ѷ�ѡ������
    RECT beginnerRect = ui.getBeginnerRect();
    RECT intermediateRect = ui.getIntermediateRect();
    RECT expertRect = ui.getExpertRect();
    
    // ����Ƿ������Ѷ�ѡ������
    if (PtInRect(&beginnerRect, {x, y})) {
        // �����ǰ�ѶȲ��ǳ��������л�
        if (currentDifficulty != GameDifficulty::BEGINNER) {
            setDifficulty(GameDifficulty::BEGINNER);
            return true;
        }
    }
    else if (PtInRect(&intermediateRect, {x, y})) {
        // �����ǰ�ѶȲ����м������л�
        if (currentDifficulty != GameDifficulty::INTERMEDIATE) {
            setDifficulty(GameDifficulty::INTERMEDIATE);
            return true;
        }
    }
    else if (PtInRect(&expertRect, {x, y})) {
        // �����ǰ�ѶȲ���ר�ң����л�
        if (currentDifficulty != GameDifficulty::EXPERT) {
            setDifficulty(GameDifficulty::EXPERT);
            return true;
        }
    }
    
    return false;
} 
