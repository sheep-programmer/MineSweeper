#pragma once
#include <vector>
#include <windows.h>
#include "Grid.h"

// ����ص�����ָ������
typedef void (*ResetCallback)();
typedef void (*GameStateCallback)(bool isMine, int remainingCells);

// ��Ϸ�����࣬���������Ϸ������߼�
class Board {
private:
    std::vector<std::vector<Grid>> grid;  // ��Ϸ����
    bool isFirstClick;                    // �Ƿ��ǵ�һ�ε��
    int remainingCells;                   // ʣ��δ�ҿ��ĸ�����
    int remainingMines;                   // ʣ������
    ResetCallback resetCallback;          // ���ú���
    GameStateCallback stateCallback;      // ��Ϸ״̬�仯�ص�
    bool showMinesMode;                   // �Ƿ���ʾ���е��ף�����ģʽ��
    
    // ���̳ߴ�
    int rows;                           // ��������
    int cols;                           // ��������
    int mineCount;                      // ��������

public:
    // ���캯��
    Board();
    
    // �������̳ߴ�͵�������
    void setSize(int newRows, int newCols, int newMineCount);

    // ��ʼ����Ϸ����
    void initGrid();

    // ���õ���
    void placeMines(int firstClickX, int firstClickY);

    // ����ÿ��������Χ�ĵ�������
    void countMines();

    // �򿪸���
    void openCell(int row, int col);

    // �ݹ����Χ�հ׸���
    void openAdjacentCells(int row, int col);

    // ����������¼�
    void handleMouseClick(int x, int y, bool isLeftClick, POINT restartBtnPos);

    // ���¿�ʼ��Ϸ
    void restart();

    // �л����ӵı��״̬
    void toggleCellFlag(int row, int col);

    // ��ȡ����
    const Grid& getCell(int row, int col) const;

    // ��ȡ��Ϸ״̬��Ϣ
    bool isFirstClickState() const;
    int getRemainingMines() const;
    int getRemainingCells() const;
    void setFirstClickState(bool state);
    
    // ��ȡ���̳ߴ���Ϣ
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    int getMineCount() const { return mineCount; }

    // �������ûص�����
    void setResetCallback(ResetCallback callback);
    
    // ������Ϸ״̬�ص�����
    void setStateCallback(GameStateCallback callback);

    // ���Ժ������л���ʾ�����׵�λ��ģʽ�����ص�ǰ�Ƿ��ڵ���ģʽ
    bool debugShowAllMines();
    
    // ��ʾ���������е��ף���Ϸ����ʱ��Game����ã�
    void showAllMines(bool isWin);

    // ��ȡ��ǰ�Ƿ��ڵ���ģʽ
    bool isDebugMode() const { return showMinesMode; }
}; 
