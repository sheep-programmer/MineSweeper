#include <easyx.h>      // EasyX图形库
#include <graphics.h>   // EasyX图形库扩展
#include <stdio.h>      // 标准输入输出
#include <conio.h>      // 控制台输入输出

int main()
{
    // 初始化图形窗口
    initgraph(800, 600, EX_SHOWCONSOLE);
    
    // 设置窗口标题
    HWND hwnd = GetHWnd();
    SetWindowText(hwnd, _T("鼠标点击测试"));
    
    // 创建控制台窗口
    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    
    printf("鼠标点击测试程序已启动\n");
    printf("请在图形窗口中点击，坐标将显示在此控制台中\n");
    printf("按ESC键退出程序\n\n");
    
    // 设置背景颜色为白色
    setbkcolor(WHITE);
    cleardevice();
    
    // 显示提示信息
    settextcolor(RGB(0, 0, 255));
    settextstyle(24, 0, _T("宋体"));
    outtextxy(10, 10, _T("点击屏幕任意位置，坐标将显示在控制台"));
    outtextxy(10, 40, _T("按ESC键退出程序"));
    
    // 创建消息变量
    ExMessage msg;
    
    // 主循环
    bool running = true;
    while (running)
    {
        // 获取消息
        if (peekmessage(&msg))
        {
            switch (msg.message)
            {
                case WM_KEYDOWN:
                    // 检测ESC键
                    if (msg.vkcode == VK_ESCAPE)
                    {
                        running = false;
                        printf("ESC键被按下，程序退出...\n");
                    }
                    break;
                    
                case WM_LBUTTONDOWN:  // 左键点击
                    printf("左键点击 - 坐标: (%d, %d)\n", msg.x, msg.y);
                    
                    // 在点击位置绘制一个小红点
                    setfillcolor(RGB(255, 0, 0));
                    solidcircle(msg.x, msg.y, 5);
                    break;
                    
                case WM_RBUTTONDOWN:  // 右键点击
                    printf("右键点击 - 坐标: (%d, %d)\n", msg.x, msg.y);
                    
                    // 在点击位置绘制一个小红点
                    setfillcolor(RGB(255, 0, 0));
                    solidcircle(msg.x, msg.y, 5);
                    break;
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
