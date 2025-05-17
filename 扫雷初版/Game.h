#pragma once
#include <easyx.h>     // EasyXͼ�ο�
#include <graphics.h>  // EasyXͼ�ο���չ
#include <ctime>
#include <windows.h>
#include "Board.h"

// ��Ϸ���ó���
#define ROWS 9        // ����
#define COLS 9         // ����
#define MINE_COUNT 10  // ��������������һЩ����ʹ��Ϸ������ս��
#define CELL_SIZE 30   // ���Ӵ�С
#define MARGIN 100     // �߾�
#define INFO_HEIGHT 40 // ��Ϣ���߶�
#define INFO_MARGIN 10 // ��Ϣ���ڱ߾�

// ǰ������
class Game;

// ��Ϸ״̬�ص�����
typedef void (*GameStateCallback)(bool isMine, int remainingCells);

// ���ڹ����࣬���𴴽��͹�����
class WindowManager {
private:
    HWND hwnd;             // ���ھ��
    HICON hIcon;           // ͼ����
    Game* gameInstance;    // ��Ϸʵ��ָ��

public:
    // ���캯��
    WindowManager();
    
    // ��������
    ~WindowManager();
    
    // ��ʼ������
    void initWindow(int width, int height, Game* game);
    
    // ���ô���ͼ��
    void setIcon(const char* iconPath);
    
    // ��ȡ���ھ��
    HWND getHWnd() const;
    
    // ע�ᴰ�ڹ���
    void registerWindowProcedure();
    
    // �رմ���
    void closeWindow();
    
    // �������ڴ�С
    void resizeWindow(int width, int height);
};

// ��ϷUI�࣬���������Ϸ����
class GameUI {
private:
    // ͼƬ��Դ
    IMAGE imgRestart[3];          // ��Ϸ״̬ͼ�꣨��������ʤ��ʧ�ܣ�
    IMAGE digitsImg[10];          // ������ʾ���������ͼ�ʱ����
    
    POINT restartBtnPos;          // ���¿�ʼ��ťλ��
    RECT infoRect;                // ��Ϣ������
    
public:
    // ���캯��
    GameUI();
    
    // ��ʼ��UI
    void init();
    
    // ����ͼƬ��Դ
    void loadImages();
    
    // ������Ϸ����
    void drawUI(const Board& board, int gameTime = 0, bool debugMode = false, bool isGameOver = false, bool isWin = false);
    
    // ������Ϣ��������������Ц���ͼ�ʱ����
    void drawInfoPanel(const Board& board, int gameTime, bool isGameOver, bool isWin);
    
    // ����������ʾ������������ʱ�䣩
    void drawDigits(int x, int y, int number, int digits);
    
    // ��ȡ���¿�ʼ��ťλ��
    POINT getRestartBtnPos() const;
    
    // ��ȡ��Ϣ������
    RECT getInfoRect() const;
};

// ɨ����Ϸ����
class Game {
private:
    Board board;             // ��Ϸ����
    GameUI ui;               // ��ϷUI
    WindowManager windowManager; // ���ڹ�����
    bool isRunning;          // ��Ϸ�Ƿ�������
    time_t startTime;        // ��Ϸ��ʼʱ��
    int gameTime;            // ��Ϸ�ѽ���ʱ�䣨�룩
    bool isDebugMode;        // �Ƿ��ڵ���ģʽ
    bool isGameOver;         // ��Ϸ�Ƿ����
    bool isWin;              // �Ƿ��ʤ
    int remainingMines;      // ʣ���������
    
    // ��Ϸ״̬�ص���������Board�����֪ͨ��Ϸ״̬�仯
    static void GameStateChangeCallback(bool isMine, int remainingCells);
    
public:
    // ���캯��
    Game();
    
    // ��������
    ~Game();
    
    // ��Ϸ��ʼ��
    void init();
    
    // ��Ϸ��ʼ
    void start();
    
    // ���¿�ʼ��Ϸ
    void restart();
    
    // ����������¼�
    void handleMouseClick(int x, int y, bool isLeftClick);
    
    // ��Ϸ��ѭ��
    void gameLoop();
    
    // ������Ϸʱ��
    void updateGameTime();
    
    // ��ȡ��Ϸʱ��
    int getGameTime() const;
    
    // ���Ժ������л���ʾ�����׵�λ��
    void debugShowAllMines();
    
    // ��Ϸ��������
    void gameOver(bool win);
    
    // ����Ƿ��ʤ
    void checkWin(int remainingCells);
    
    // ��ʾ���е���
    void showAllMines();
    
    // ����ʣ���������
    void updateRemainingMines(int mines);
    
    // ��ȡ��Ϸ״̬
    bool isGameOverState() const { return isGameOver; }
    bool isWinState() const { return isWin; }
    
    // ��ȡ��Ϸ���̶���
    Board& getBoard() { return board; }
    
    // ��ȡ��ǰ�Ƿ��ڵ���ģʽ
    bool isInDebugMode() const { return isDebugMode; }
}; 
