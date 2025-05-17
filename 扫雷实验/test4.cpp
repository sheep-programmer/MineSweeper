#include <easyx.h>      // EasyX图形库
#include <graphics.h>   // EasyX图形库扩展
#include <stdio.h>      // 标准输入输出
#include <conio.h>      // 控制台输入输出
#include <string>       // 字符串处理
#include <windows.h>    // Windows API

// 按钮结构体
struct Button {
    int x, y;           // 按钮位置
    int width, height;  // 按钮大小
    TCHAR text[50];     // 按钮文本
    bool hover;         // 鼠标悬停状态
    bool pressed;       // 按钮按下状态
    int clickCount;     // 点击次数
    COLORREF textColor; // 文字颜色
    
    // 构造函数
    Button() : hover(false), pressed(false), clickCount(0), textColor(RGB(255, 255, 255)) {
        text[0] = _T('\0');
    }
    
    // 检查点是否在按钮内
    bool contains(int px, int py) {
        return px >= x && px <= x + width && py >= y && py <= y + height;
    }
    
    // 绘制按钮
    void draw() {
        // 设置按钮颜色
        if (pressed) {
            setfillcolor(RGB(60, 110, 200));   // 按下颜色
        } else if (hover) {
            setfillcolor(RGB(100, 150, 255));  // 悬停颜色
        } else {
            setfillcolor(RGB(70, 130, 230));   // 正常颜色
        }
        
        // 绘制按钮背景
        solidroundrect(x, y, x + width, y + height, 10, 10);
        
        // 绘制按钮边框
        setlinecolor(RGB(40, 80, 160));
        roundrect(x, y, x + width, y + height, 10, 10);
        
        // 绘制按钮文本
        settextcolor(textColor);
        settextstyle(20, 0, _T("宋体"));
        setbkmode(TRANSPARENT);  // 设置文字背景为透明，去掉黑色边框
        int textWidth = textwidth(text);
        int textHeight = textheight(text);
        outtextxy(x + (width - textWidth) / 2, y + (height - textHeight) / 2, text);
        
        // 显示点击次数
        TCHAR countText[20];
        _stprintf(countText, _T("点击: %d"), clickCount);
        settextstyle(16, 0, _T("宋体"));
        outtextxy(x + (width - textwidth(countText)) / 2, y + height - 20, countText);
    }
};

// 设置应用程序图标
bool SetAppIcon(HWND hwnd, const TCHAR* iconPath) {
    HICON hIcon = (HICON)LoadImage(
        NULL,                   // 实例句柄
        iconPath,               // 图标路径
        IMAGE_ICON,             // 图像类型
        32,                     // 宽度
        32,                     // 高度
        LR_LOADFROMFILE         // 从文件加载
    );
    
    if (hIcon) {
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        return true;
    }
    
    return false;
}

int main()
{
    // 初始化图形窗口
    initgraph(800, 600, EX_SHOWCONSOLE);
    
    // 设置文字背景为透明，去掉黑色边框
    setbkmode(TRANSPARENT);
    
    // 设置窗口标题
    HWND hwnd = GetHWnd();
    SetWindowText(hwnd, _T("按钮点击测试"));
    
    // 创建控制台窗口
    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    
    printf("按钮点击测试程序已启动\n");
    printf("请点击按钮测试功能\n");
    printf("按ESC键退出程序\n\n");
    
    // 创建按钮
    Button buttons[3];
    
    // 按钮1：红色按钮
    buttons[0].x = 100;
    buttons[0].y = 200;
    buttons[0].width = 150;
    buttons[0].height = 80;
    _tcscpy(buttons[0].text, _T("红色按钮"));
    buttons[0].textColor = RGB(255, 0, 0);
    
    // 按钮2：绿色按钮
    buttons[1].x = 325;
    buttons[1].y = 200;
    buttons[1].width = 150;
    buttons[1].height = 80;
    _tcscpy(buttons[1].text, _T("绿色按钮"));
    buttons[1].textColor = RGB(0, 255, 0);
    
    // 按钮3：蓝色按钮
    buttons[2].x = 550;
    buttons[2].y = 200;
    buttons[2].width = 150;
    buttons[2].height = 80;
    _tcscpy(buttons[2].text, _T("蓝色按钮"));
    buttons[2].textColor = RGB(0, 0, 255);
    
    // 消息显示区域
    TCHAR messageText[100] = _T("请点击上面的按钮进行测试");
    
    // 创建消息变量
    ExMessage msg;
    
    // 开始批量绘图模式
    BeginBatchDraw();
    
    // 主循环
    bool running = true;
    while (running)
    {
        // 清屏
        cleardevice();
        
        // 绘制标题
        settextcolor(RGB(255, 255, 255));
        settextstyle(32, 0, _T("宋体"));
        outtextxy(300, 50, _T("按钮点击测试"));
        
        // 绘制消息
        settextstyle(20, 0, _T("宋体"));
        outtextxy(100, 350, messageText);
        
        // 绘制说明
        settextcolor(RGB(200, 200, 200));
        settextstyle(18, 0, _T("宋体"));
        outtextxy(100, 400, _T("左键点击: 增加按钮点击次数"));
        outtextxy(100, 430, _T("右键点击: 重置按钮点击次数"));
        outtextxy(100, 460, _T("按ESC键退出程序"));
        
        // 绘制按钮
        for (int i = 0; i < 3; i++) {
            buttons[i].draw();
        }
        
        // 处理消息
        while (peekmessage(&msg)) {
            switch (msg.message) {
                case WM_MOUSEMOVE:
                    // 更新按钮悬停状态
                    for (int i = 0; i < 3; i++) {
                        buttons[i].hover = buttons[i].contains(msg.x, msg.y);
                    }
                    break;
                    
                case WM_LBUTTONDOWN:
                    // 更新按钮按下状态
                    for (int i = 0; i < 3; i++) {
                        if (buttons[i].contains(msg.x, msg.y)) {
                            buttons[i].pressed = true;
                        }
                    }
                    break;
                    
                case WM_LBUTTONUP:
                    // 检查是否点击按钮
                    for (int i = 0; i < 3; i++) {
                        if (buttons[i].pressed && buttons[i].contains(msg.x, msg.y)) {
                            // 增加点击次数
                            buttons[i].clickCount++;
                            
                            // 更新消息
                            _stprintf(messageText, _T("左键点击了 %s (总计: %d 次)"), 
                                     buttons[i].text, buttons[i].clickCount);
                            
                            // 输出到控制台
                            printf("左键点击按钮: %s (点击次数: %d)\n", 
                                  i == 0 ? "红色按钮" : (i == 1 ? "绿色按钮" : "蓝色按钮"), 
                                  buttons[i].clickCount);
                            
                            // 根据按钮更改背景色
                            if (i == 0) {
                                setbkcolor(RGB(50, 0, 0));  // 暗红色背景
                            } else if (i == 1) {
                                setbkcolor(RGB(0, 50, 0));  // 暗绿色背景
                            } else if (i == 2) {
                                setbkcolor(RGB(0, 0, 50));  // 暗蓝色背景
                            }
                        }
                        
                        // 重置按下状态
                        buttons[i].pressed = false;
                    }
                    break;
                    
                case WM_RBUTTONDOWN:
                    // 检查是否右键点击按钮
                    for (int i = 0; i < 3; i++) {
                        if (buttons[i].contains(msg.x, msg.y)) {
                            // 重置点击次数
                            buttons[i].clickCount = 0;
                            
                            // 更新消息
                            _stprintf(messageText, _T("右键点击重置了 %s 的点击次数"), buttons[i].text);
                            
                            // 输出到控制台
                            printf("右键点击重置按钮: %s\n", 
                                  i == 0 ? "红色按钮" : (i == 1 ? "绿色按钮" : "蓝色按钮"));
                        }
                    }
                    break;
                    
                case WM_KEYDOWN:
                    if (msg.vkcode == VK_ESCAPE) {
                        running = false;
                        printf("ESC键被按下，程序退出...\n");
                    }
                    break;
            }
        }
        
        // 刷新屏幕
        FlushBatchDraw();
        
        // 延时
        Sleep(10);
    }
    
    // 结束批量绘图
    EndBatchDraw();
    
    // 关闭控制台
    FreeConsole();
    
    // 关闭图形窗口
    closegraph();
    return 0;
}
