#pragma once
#include <easyx.h>
#include <graphics.h>
#include <vector>

// ȫ�ֳ���
#define CELL_SIZE 30   // ���Ӵ�С
#define MARGIN 100     // �߾�

// ����״̬ö��
enum class GridState {
    UNOPENED,    // δ��
    OPENED,      // �Ѵ�
    FLAGGED      // ���
};

// ��ʾɨ����Ϸ�е�һ������
class Grid {
private:
    int row;            // ������
    int col;            // ������
    bool hasMine;       // �Ƿ��е���
    int adjacentMines;  // ��Χ��������
    GridState state;    // ����״̬
    
    // ����ͼƬ��Դ����̬��Ա�����и��ӹ���
    static IMAGE s_imgInitial;     // δ�ҿ�
    static IMAGE s_imgFlag;        // ���
    static IMAGE s_imgBombed;      // �ȵ�����
    static IMAGE s_imgUnbombed;    // δ�ȵ��ĵ��ף�ʤ��ʱ��
    static IMAGE s_imgNumbers[9];  // ����0-8���ҿ�����ʾ��
    static bool s_imagesLoaded;    // ͼƬ�Ƿ��Ѽ���
    static bool s_textSettingsInitialized; // �ı������Ƿ��ѳ�ʼ��
    
public:
    // ���캯��
    Grid();
    Grid(int row, int col);
    
    // ��̬���������ظ���ͼƬ��Դ
    static bool LoadImages(int cellSize);
    
    // ��ȡ���ӵ�������
    int getRow() const;
    
    // ��ȡ���ӵ�������
    int getCol() const;
    
    // ����/��ȡ�����Ƿ��е���
    void setMine(bool value);
    bool isMine() const;
    
    // ����/��ȡ��Χ��������
    void setAdjacentMines(int count);
    int getAdjacentMines() const;
    
    // ����/��ȡ����״̬
    void setState(GridState newState);
    GridState getState() const;
    
    // �жϸ����Ƿ��Ѵ�
    bool isOpened() const;
    
    // �жϸ����Ƿ񱻱��
    bool isFlagged() const;
    
    // �򿪸���
    void open();
    
    // �л����״̬
    void toggleFlag();
    
    // ���Ƹ���
    void draw(int x, int y, bool isGameOver, bool isWin, bool showMine) const;
}; 
