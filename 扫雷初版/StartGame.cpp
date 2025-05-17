#include <easyx.h>      // EasyX图形库
#include <graphics.h>   // EasyX图形库扩展
#include <windows.h>    // Windows API
#include "Game.h"

int main() {
    // 创建扫雷游戏对象
    Game game;
    
    // 开始游戏（按F9键可以显示所有雷的位置）0.
    game.start();
    
    return 0;
} 
