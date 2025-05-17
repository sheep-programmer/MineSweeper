#include <easyx.h>      // EasyX图形库
#include <graphics.h>   // EasyX图形库扩展
#include <stdio.h>      // 标准输入输出
#include <conio.h>      // 控制台输入输出
#include <windows.h>    // Windows API

// 自定义窗口过程函数
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main()
{
    // 创建控制台窗口
    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    
    printf("窗口大小和位置调节测试程序\n\n");
    
    // 初始窗口参数
    int windowWidth = 800;
    int windowHeight = 600;
    int windowX = 100;
    int windowY = 100;
    
    // 初始化图形窗口（不显示）
    HWND hwnd = initgraph(windowWidth, windowHeight, EW_NOMINIMIZE | EW_SHOWCONSOLE);
    
    // 设置窗口标题
    SetWindowText(hwnd, _T("窗口大小和位置测试"));
    
    // 设置窗口位置
    SetWindowPos(hwnd, HWND_TOP, windowX, windowY, windowWidth, windowHeight, SWP_SHOWWINDOW);
    
    // 显示当前窗口信息
    printf("当前窗口大小: %d x %d\n", windowWidth, windowHeight);
    printf("当前窗口位置: (%d, %d)\n\n", windowX, windowY);
    
    // 显示操作说明
    printf("操作说明:\n");
    printf("1 - 增加窗口宽度\n");
    printf("2 - 减少窗口宽度\n");
    printf("3 - 增加窗口高度\n");
    printf("4 - 减少窗口高度\n");
    printf("W - 向上移动窗口\n");
    printf("S - 向下移动窗口\n");
    printf("A - 向左移动窗口\n");
    printf("D - 向右移动窗口\n");
    printf("C - 将窗口居中显示\n");
    printf("R - 重置窗口大小和位置\n");
    printf("ESC - 退出程序\n\n");
    
    // 创建消息变量
    ExMessage msg;
    
    // 更新窗口内容的函数
    auto updateWindow = [&]() {
        // 清屏
        cleardevice();
        
        // 标题
        settextcolor(RGB(255, 255, 255));
        settextstyle(24, 0, _T("宋体"));
        outtextxy(10, 10, _T("窗口大小和位置测试"));
        
        // 绘制边框
        setlinecolor(RGB(255, 0, 0));
        rectangle(10, 50, windowWidth - 10, windowHeight - 10);
        
        // 显示窗口尺寸
        settextstyle(20, 0, _T("宋体"));
        TCHAR sizeText[50];
        _stprintf(sizeText, _T("窗口大小: %d x %d"), windowWidth, windowHeight);
        outtextxy(10, 60, sizeText);
        
        TCHAR posText[50];
        _stprintf(posText, _T("窗口位置: (%d, %d)"), windowX, windowY);
        outtextxy(10, 90, posText);
        
        // 添加操作说明到窗口
        settextcolor(RGB(200, 200, 200));
        settextstyle(16, 0, _T("宋体"));
        outtextxy(10, 130, _T("操作说明:"));
        outtextxy(10, 150, _T("1 - 增加窗口宽度"));
        outtextxy(10, 170, _T("2 - 减少窗口宽度"));
        outtextxy(10, 190, _T("3 - 增加窗口高度"));
        outtextxy(10, 210, _T("4 - 减少窗口高度"));
        outtextxy(10, 230, _T("W - 向上移动窗口"));
        outtextxy(10, 250, _T("S - 向下移动窗口"));
        outtextxy(10, 270, _T("A - 向左移动窗口"));
        outtextxy(10, 290, _T("D - 向右移动窗口"));
        outtextxy(10, 310, _T("C - 将窗口居中显示"));
        outtextxy(10, 330, _T("R - 重置窗口大小和位置"));
        outtextxy(10, 350, _T("ESC - 退出程序"));
    };
    
    // 初始绘制
    updateWindow();
    
    // 主循环
    bool running = true;
    while (running)
    {
        // 获取消息
        if (peekmessage(&msg))
        {
            if (msg.message == WM_KEYDOWN)
            {
                // 更新窗口参数
                switch (msg.vkcode)
                {
                    case '1':  // 增加宽度
                        windowWidth += 50;
                        break;
                        
                    case '2':  // 减少宽度
                        if (windowWidth > 300)
                            windowWidth -= 50;
                        break;
                        
                    case '3':  // 增加高度
                        windowHeight += 50;
                        break;
                        
                    case '4':  // 减少高度
                        if (windowHeight > 200)
                            windowHeight -= 50;
                        break;
                        
                    case 'W':  // 向上移动
                        windowY -= 50;
                        break;
                        
                    case 'S':  // 向下移动
                        windowY += 50;
                        break;
                        
                    case 'A':  // 向左移动
                        windowX -= 50;
                        break;
                        
                    case 'D':  // 向右移动
                        windowX += 50;
                        break;
                        
                    case 'C':  // 窗口居中
                        {
                            int screenWidth = GetSystemMetrics(SM_CXSCREEN);
                            int screenHeight = GetSystemMetrics(SM_CYSCREEN);
                            windowX = (screenWidth - windowWidth) / 2;
                            windowY = (screenHeight - windowHeight) / 2;
                        }
                        break;
                        
                    case 'R':  // 重置窗口
                        windowWidth = 800;
                        windowHeight = 600;
                        windowX = 100;
                        windowY = 100;
                        break;
                        
                    case VK_ESCAPE:  // ESC键退出
                        running = false;
                        printf("ESC键被按下，程序退出...\n");
                        break;
                }
                
                // 调整窗口大小和位置
                SetWindowPos(hwnd, HWND_TOP, windowX, windowY, windowWidth, windowHeight, SWP_SHOWWINDOW);
                
                // 更新显示
                updateWindow();
                
                // 显示当前窗口信息到控制台
                printf("当前窗口大小: %d x %d\n", windowWidth, windowHeight);
                printf("当前窗口位置: (%d, %d)\n", windowX, windowY);
            }
        }
        
        // 延时
        Sleep(10);
    }
    
    // 关闭控制台
    FreeConsole();
    
    // 关闭图形窗口
    closegraph();
    return 0;
}
