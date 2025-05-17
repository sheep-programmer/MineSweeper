#include "Board.h"
#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

// 构造函数
Board::Board() : isFirstClick(true), 
                 remainingCells(0), remainingMines(0),
                 resetCallback(NULL), stateCallback(NULL), showMinesMode(false),
                 rows(BEGINNER_ROWS), cols(BEGINNER_COLS), mineCount(BEGINNER_MINES) {
    // 初始化随机数种子
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // 设置默认大小
    setSize(BEGINNER_ROWS, BEGINNER_COLS, BEGINNER_MINES);
    
    // 初始化网格
    initGrid();
}

// 设置棋盘尺寸和地雷数量
void Board::setSize(int newRows, int newCols, int newMineCount) {
    rows = newRows;
    cols = newCols;
    mineCount = newMineCount;
    remainingCells = rows * cols - mineCount;
    remainingMines = mineCount;
    isFirstClick = true;
    showMinesMode = false;
    
    // 重新初始化网格
    initGrid();
}

// 初始化游戏网格
void Board::initGrid() {
    grid.clear();
    grid.resize(rows, std::vector<Grid>(cols));
    
    // 初始化每个格子
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grid[i][j] = Grid(i, j);
        }
    }
}

// 放置地雷（确保第一次点击不会踩到雷）
void Board::placeMines(int firstClickX, int firstClickY) {
    int mineCount = 0;
    
    // 转换点击坐标到网格索引
    int firstRow = (firstClickY - MARGIN) / CELL_SIZE;
    int firstCol = (firstClickX - MARGIN) / CELL_SIZE;
    
    // 确保坐标有效
    if (firstRow < 0 || firstRow >= rows || firstCol < 0 || firstCol >= cols) {
        return;
    }
    
    while (mineCount < this->mineCount) {
        int row = rand() % rows;
        int col = rand() % cols;
        
        // 确保第一次点击的格子及其周围8个格子没有地雷
        bool isSafe = (abs(row - firstRow) > 1 || abs(col - firstCol) > 1);
        
        // 如果该位置还没有地雷，且不是第一次点击的安全区域，则放置地雷
        if (!grid[row][col].isMine() && isSafe) {
            grid[row][col].setMine(true);
            mineCount++;
        }
    }
    
    // 计算每个格子周围的地雷数量
    countMines();
}

// 计算每个格子周围的地雷数量
void Board::countMines() {
    // 定义8个方向的偏移量
    const int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j].isMine()) {
                continue;  // 跳过地雷格子
            }
            
            int count = 0;
            
            // 检查周围8个方向
            for (int k = 0; k < 8; k++) {
                int ni = i + dr[k];
                int nj = j + dc[k];
                
                // 检查边界
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

// 打开格子
void Board::openCell(int row, int col) {
    // 检查边界
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        return;
    }
    
    Grid& cell = grid[row][col];
    
    // 如果格子已打开或已标记，则不处理
    if (cell.isOpened() || cell.isFlagged()) {
        return;
    }
    
    // 打开格子
    cell.open();
    
    // 如果踩到地雷，通知游戏结束
    if (cell.isMine()) {
        if (stateCallback) {
            stateCallback(true, remainingCells);
        }
        return;
    }
    
    // 减少未打开的格子数量
    remainingCells--;
    
    // 如果周围没有地雷，递归打开周围的格子
    if (cell.getAdjacentMines() == 0) {
        openAdjacentCells(row, col);
    }
    
    // 检查是否获胜(所有非地雷格子都已打开)
    if (remainingCells == 0 && stateCallback) {
        stateCallback(false, 0);
    }
}

// 递归打开周围空白格子
void Board::openAdjacentCells(int row, int col) {
    // 定义8个方向的偏移量
    const int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    // 检查周围8个方向
    for (int k = 0; k < 8; k++) {
        int ni = row + dr[k];
        int nj = col + dc[k];
        
        // 检查边界
        if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
            // 如果格子未打开且未标记，则打开它
            if (!grid[ni][nj].isOpened() && !grid[ni][nj].isFlagged()) {
                openCell(ni, nj);
            }
        }
    }
}

// 处理鼠标点击事件
void Board::handleMouseClick(int x, int y, bool isLeftClick, POINT restartBtnPos) {
    // 检查是否点击了重新开始按钮
    if (abs(x - restartBtnPos.x) <= 15 && abs(y - restartBtnPos.y) <= 15) {
        restart();
        // 调用重置回调函数，通知上层进行重置
        if (resetCallback) {
            resetCallback();
        }
        return;
    }
    
    // 检查是否在游戏网格范围内
    if (x < MARGIN || y < MARGIN || 
        x >= MARGIN + cols * CELL_SIZE || 
        y >= MARGIN + rows * CELL_SIZE) {
        return;
    }
    
    // 转换鼠标坐标到网格索引
    int row = (y - MARGIN) / CELL_SIZE;
    int col = (x - MARGIN) / CELL_SIZE;
    
    // 检查索引有效性
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        return;
    }
    
    // 如果是第一次点击，初始化地雷
    if (isFirstClick && isLeftClick) {
        placeMines(x, y);
        isFirstClick = false;
    }
    
    // 处理左键点击
    if (isLeftClick) {
        // 如果是标记的格子，取消标记
        if (grid[row][col].isFlagged()) {
            toggleCellFlag(row, col);
        } else {
            // 否则打开格子
            openCell(row, col);
        }
    }
    // 处理右键点击（标记地雷）
    else {
        toggleCellFlag(row, col);
    }
}

// 切换格子的标记状态
void Board::toggleCellFlag(int row, int col) {
    if (!grid[row][col].isOpened()) {
        // 如果当前是未标记状态且剩余雷数为0，不能再标记
        if (!grid[row][col].isFlagged() && remainingMines <= 0) {
            return;
        }
        
        grid[row][col].toggleFlag();
        // 更新剩余雷数
        if (grid[row][col].isFlagged()) {
            remainingMines--;
        } else {
            remainingMines++;
        }
    }
}

// 重新开始游戏
void Board::restart() {
    // 重置状态
    isFirstClick = true;
    remainingCells = rows * cols - mineCount;
    remainingMines = mineCount;
    showMinesMode = false;
    
    // 重新初始化网格
    initGrid();
}

// 获取格子
const Grid& Board::getCell(int row, int col) const {
    return grid[row][col];
}

// 获取游戏状态
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

// 设置重置回调函数
void Board::setResetCallback(ResetCallback callback) {
    resetCallback = callback;
}

// 设置游戏状态回调函数
void Board::setStateCallback(GameStateCallback callback) {
    stateCallback = callback;
}

// 调试函数：切换显示所有雷的位置模式，返回当前是否处于调试模式
bool Board::debugShowAllMines() {
    // 切换显示模式
    showMinesMode = !showMinesMode;
    
    // 返回调试模式状态，让UI层处理显示效果
    return showMinesMode;
}

// 显示所有地雷（游戏结束时由Game类调用）
void Board::showAllMines(bool isWin) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j].isMine()) {
                // 如果游戏获胜，将所有地雷标记为已找到
                if (isWin) {
                    grid[i][j].setState(GridState::OPENED);
                }
                // 如果游戏失败，只打开未标记的地雷
                else if (!grid[i][j].isFlagged()) {
                    grid[i][j].open();
                }
            }
        }
    }
} 
