#include "Board.h"
#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

// ���캯��
Board::Board() : isFirstClick(true), 
                 remainingCells(0), remainingMines(0),
                 resetCallback(NULL), stateCallback(NULL), showMinesMode(false),
                 rows(BEGINNER_ROWS), cols(BEGINNER_COLS), mineCount(BEGINNER_MINES) {
    // ��ʼ�����������
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // ����Ĭ�ϴ�С
    setSize(BEGINNER_ROWS, BEGINNER_COLS, BEGINNER_MINES);
    
    // ��ʼ������
    initGrid();
}

// �������̳ߴ�͵�������
void Board::setSize(int newRows, int newCols, int newMineCount) {
    rows = newRows;
    cols = newCols;
    mineCount = newMineCount;
    remainingCells = rows * cols - mineCount;
    remainingMines = mineCount;
    isFirstClick = true;
    showMinesMode = false;
    
    // ���³�ʼ������
    initGrid();
}

// ��ʼ����Ϸ����
void Board::initGrid() {
    grid.clear();
    grid.resize(rows, std::vector<Grid>(cols));
    
    // ��ʼ��ÿ������
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grid[i][j] = Grid(i, j);
        }
    }
}

// ���õ��ף�ȷ����һ�ε������ȵ��ף�
void Board::placeMines(int firstClickX, int firstClickY) {
    int mineCount = 0;
    
    // ת��������굽��������
    int firstRow = (firstClickY - MARGIN) / CELL_SIZE;
    int firstCol = (firstClickX - MARGIN) / CELL_SIZE;
    
    // ȷ��������Ч
    if (firstRow < 0 || firstRow >= rows || firstCol < 0 || firstCol >= cols) {
        return;
    }
    
    while (mineCount < this->mineCount) {
        int row = rand() % rows;
        int col = rand() % cols;
        
        // ȷ����һ�ε���ĸ��Ӽ�����Χ8������û�е���
        bool isSafe = (abs(row - firstRow) > 1 || abs(col - firstCol) > 1);
        
        // �����λ�û�û�е��ף��Ҳ��ǵ�һ�ε���İ�ȫ��������õ���
        if (!grid[row][col].isMine() && isSafe) {
            grid[row][col].setMine(true);
            mineCount++;
        }
    }
    
    // ����ÿ��������Χ�ĵ�������
    countMines();
}

// ����ÿ��������Χ�ĵ�������
void Board::countMines() {
    // ����8�������ƫ����
    const int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j].isMine()) {
                continue;  // �������׸���
            }
            
            int count = 0;
            
            // �����Χ8������
            for (int k = 0; k < 8; k++) {
                int ni = i + dr[k];
                int nj = j + dc[k];
                
                // ���߽�
                if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                    if (grid[ni][nj].isMine()) {
                        count++;
                    }
                }
            }
            
            grid[i][j].setAdjacentMines(count);
        }
    }
}

// �򿪸���
void Board::openCell(int row, int col) {
    // ���߽�
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        return;
    }
    
    Grid& cell = grid[row][col];
    
    // ��������Ѵ򿪻��ѱ�ǣ��򲻴���
    if (cell.isOpened() || cell.isFlagged()) {
        return;
    }
    
    // �򿪸���
    cell.open();
    
    // ����ȵ����ף�֪ͨ��Ϸ����
    if (cell.isMine()) {
        if (stateCallback) {
            stateCallback(true, remainingCells);
        }
        return;
    }
    
    // ����δ�򿪵ĸ�������
    remainingCells--;
    
    // �����Χû�е��ף��ݹ����Χ�ĸ���
    if (cell.getAdjacentMines() == 0) {
        openAdjacentCells(row, col);
    }
    
    // ����Ƿ��ʤ(���зǵ��׸��Ӷ��Ѵ�)
    if (remainingCells == 0 && stateCallback) {
        stateCallback(false, 0);
    }
}

// �ݹ����Χ�հ׸���
void Board::openAdjacentCells(int row, int col) {
    // ����8�������ƫ����
    const int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    // �����Χ8������
    for (int k = 0; k < 8; k++) {
        int ni = row + dr[k];
        int nj = col + dc[k];
        
        // ���߽�
        if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
            // �������δ����δ��ǣ������
            if (!grid[ni][nj].isOpened() && !grid[ni][nj].isFlagged()) {
                openCell(ni, nj);
            }
        }
    }
}

// ����������¼�
void Board::handleMouseClick(int x, int y, bool isLeftClick, POINT restartBtnPos) {
    // ����Ƿ��������¿�ʼ��ť
    if (abs(x - restartBtnPos.x) <= 15 && abs(y - restartBtnPos.y) <= 15) {
        restart();
        // �������ûص�������֪ͨ�ϲ��������
        if (resetCallback) {
            resetCallback();
        }
        return;
    }
    
    // ����Ƿ�����Ϸ����Χ��
    if (x < MARGIN || y < MARGIN || 
        x >= MARGIN + cols * CELL_SIZE || 
        y >= MARGIN + rows * CELL_SIZE) {
        return;
    }
    
    // ת��������굽��������
    int row = (y - MARGIN) / CELL_SIZE;
    int col = (x - MARGIN) / CELL_SIZE;
    
    // ���������Ч��
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        return;
    }
    
    // ����ǵ�һ�ε������ʼ������
    if (isFirstClick && isLeftClick) {
        placeMines(x, y);
        isFirstClick = false;
    }
    
    // ����������
    if (isLeftClick) {
        // ����Ǳ�ǵĸ��ӣ�ȡ�����
        if (grid[row][col].isFlagged()) {
            toggleCellFlag(row, col);
        } else {
            // ����򿪸���
            openCell(row, col);
        }
    }
    // �����Ҽ��������ǵ��ף�
    else {
        toggleCellFlag(row, col);
    }
}

// �л����ӵı��״̬
void Board::toggleCellFlag(int row, int col) {
    if (!grid[row][col].isOpened()) {
        // �����ǰ��δ���״̬��ʣ������Ϊ0�������ٱ��
        if (!grid[row][col].isFlagged() && remainingMines <= 0) {
            return;
        }
        
        grid[row][col].toggleFlag();
        // ����ʣ������
        if (grid[row][col].isFlagged()) {
            remainingMines--;
        } else {
            remainingMines++;
        }
    }
}

// ���¿�ʼ��Ϸ
void Board::restart() {
    // ����״̬
    isFirstClick = true;
    remainingCells = rows * cols - mineCount;
    remainingMines = mineCount;
    showMinesMode = false;
    
    // ���³�ʼ������
    initGrid();
}

// ��ȡ����
const Grid& Board::getCell(int row, int col) const {
    return grid[row][col];
}

// ��ȡ��Ϸ״̬
bool Board::isFirstClickState() const {
    return isFirstClick;
}

int Board::getRemainingMines() const {
    return remainingMines;
}

int Board::getRemainingCells() const {
    return remainingCells;
}

void Board::setFirstClickState(bool state) {
    isFirstClick = state;
}

// �������ûص�����
void Board::setResetCallback(ResetCallback callback) {
    resetCallback = callback;
}

// ������Ϸ״̬�ص�����
void Board::setStateCallback(GameStateCallback callback) {
    stateCallback = callback;
}

// ���Ժ������л���ʾ�����׵�λ��ģʽ�����ص�ǰ�Ƿ��ڵ���ģʽ
bool Board::debugShowAllMines() {
    // �л���ʾģʽ
    showMinesMode = !showMinesMode;
    
    // ���ص���ģʽ״̬����UI�㴦����ʾЧ��
    return showMinesMode;
}

// ��ʾ���е��ף���Ϸ����ʱ��Game����ã�
void Board::showAllMines(bool isWin) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j].isMine()) {
                // �����Ϸ��ʤ�������е��ױ��Ϊ���ҵ�
                if (isWin) {
                    grid[i][j].setState(GridState::OPENED);
                }
                // �����Ϸʧ�ܣ�ֻ��δ��ǵĵ���
                else if (!grid[i][j].isFlagged()) {
                    grid[i][j].open();
                }
            }
        }
    }
} 
