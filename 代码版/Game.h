#pragma once
#include <easyx.h>     // EasyXͼ�ο�
#include <graphics.h>  // EasyXͼ�ο���չ
#include <ctime>
#include <windows.h>
#include <mmsystem.h>  // ����mmsystem.hͷ�ļ����ڲ�����Ч
#include "Board.h"

// ��Ϸ�Ѷ�ö��
enum class GameDifficulty {
    BEGINNER,    // ������9x9, 10����
    INTERMEDIATE, // �м���16x16, 40����
    EXPERT       // ר�ң�16x30, 99����
};

// ��Ϸ���ó���
// �����Ѷ�����
#define BEGINNER_ROWS 9        // ��������
#define BEGINNER_COLS 9        // ��������
#define BEGINNER_MINES 10      // ����������

// �м��Ѷ�����
#define INTERMEDIATE_ROWS 16   // �м�����
#define INTERMEDIATE_COLS 16   // �м�����
#define INTERMEDIATE_MINES 40  // �м�������

// ר���Ѷ�����
#define EXPERT_ROWS 16         // ר������
#define EXPERT_COLS 30         // ר������
#define EXPERT_MINES 99        // ר�ҵ�����

// Ĭ��ʹ�ó����Ѷ�
#define ROWS BEGINNER_ROWS
#define COLS BEGINNER_COLS
#define MINE_COUNT BEGINNER_MINES

#define CELL_SIZE 30   // ���Ӵ�С
#define MARGIN 100     // �߾�
#define INFO_HEIGHT 40 // ��Ϣ���߶�
#define INFO_MARGIN 10 // ��Ϣ���ڱ߾�
#define DIFFICULTY_HEIGHT 30 // �Ѷ�ѡ�����߶�

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
    RECT difficultyRect;          // �Ѷ�ѡ������
    RECT beginnerRect;            // �����Ѷ���������
    RECT intermediateRect;        // �м��Ѷ���������
    RECT expertRect;              // ר���Ѷ���������
    
public:
    // ���캯��
    GameUI();
    
    // ��ʼ��UI
    void init();
    
    // ����ͼƬ��Դ
    void loadImages();
    
    // ����UI����
    void updateUILayout(int cols);
    
    // ������Ϸ����
    void drawUI(const Board& board, int gameTime = 0, bool debugMode = false, bool isGameOver = false, bool isWin = false, GameDifficulty difficulty = GameDifficulty::BEGINNER);
    
    // ������Ϣ��������������Ц���ͼ�ʱ����
    void drawInfoPanel(const Board& board, int gameTime, bool isGameOver, bool isWin);
    
    // �����Ѷ�ѡ����
    void drawDifficultyPanel(GameDifficulty currentDifficulty);
    
    // ����������ʾ������������ʱ�䣩
    void drawDigits(int x, int y, int number, int digits);
    
    // ��ȡ���¿�ʼ��ťλ��
    POINT getRestartBtnPos() const;
    
    // ��ȡ��Ϣ������
    RECT getInfoRect() const;
    
    // ��ȡ�Ѷ�ѡ�����������Ϣ
    RECT getDifficultyRect() const;
    RECT getBeginnerRect() const;
    RECT getIntermediateRect() const;
    RECT getExpertRect() const;
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
    
    // �Ѷ����
    GameDifficulty currentDifficulty; // ��ǰ��Ϸ�Ѷ�
    int rows;                // ��ǰ����
    int cols;                // ��ǰ����
    int mineCount;           // ��ǰ������
    
    // ��Ч���
    bool soundEnabled;       // �Ƿ�������Ч
    
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
    
    // �л���Ϸ�Ѷ�
    void setDifficulty(GameDifficulty difficulty);
    
    // ����������¼�
    void handleMouseClick(int x, int y, bool isLeftClick);
    
    // �����Ѷ�ѡ������ĵ��
    bool handleDifficultyClick(int x, int y);
    
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
    
    // ��Ч��ط���
    void initSound();         // ��ʼ����Ч
    void playClickSound();    // ���ŵ����Ч
    void playBoomSound();     // ���ű�ը��Ч
    void closeSound();        // �ر�������Ч
}; 
