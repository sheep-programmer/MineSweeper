#include <easyx.h>      // EasyX图形库
#include <graphics.h>   // EasyX图形库扩展
#include <stdio.h>      // 标准输入输出
#include <conio.h>      // 控制台输入输出
#include <string>       // 字符串处理
#include <windows.h>    // Windows API
#include <vector>       // 向量容器

// 设置窗口图标的函数
bool SetWindowIcon(const TCHAR* iconPath) {
    // 获取窗口句柄
    HWND hwnd = GetHWnd();
    
    // 加载图标
    HICON hIcon = (HICON)LoadImage(
        NULL,                   // 实例句柄
        iconPath,               // 图标路径
        IMAGE_ICON,             // 图像类型
        32,                     // 宽度
        32,                     // 高度
        LR_LOADFROMFILE         // 从文件加载
    );
    
    if (hIcon) {
        // 设置大图标和小图标
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        return true;
    }
    
    return false;
}

int main() {
    // 初始化图形窗口
    initgraph(800, 600);
    
    // 设置窗口标题
    SetWindowText(GetHWnd(), _T("图标切换测试"));
    
    // 创建控制台窗口
    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    
    printf("图标切换测试程序已启动\n");
    printf("按F9键切换图标\n");
    printf("按ESC键退出程序\n\n");
    
    // 设置文字背景为透明
    setbkmode(TRANSPARENT);
    
    // 图标文件列表
    std::vector<const TCHAR*> iconPaths;
    iconPaths.push_back(_T("images/logo1.ico"));
    iconPaths.push_back(_T("images/logo2.ico"));
    iconPaths.push_back(_T("images/logo3.ico"));
    iconPaths.push_back(_T("images/logo4.ico"));
    iconPaths.push_back(_T("images/logo5.ico"));
    
    // 当前图标索引
    int currentIconIndex = 0;
    
    // 设置初始图标
    if (SetWindowIcon(iconPaths[currentIconIndex])) {
        printf("已设置初始图标: %s\n", iconPaths[currentIconIndex]);
    } else {
        printf("设置初始图标失败\n");
    }
    
    // 开启双缓冲绘图
    BeginBatchDraw();
    
    // 主循环
    bool running = true;
    bool needRedraw = true;  // 是否需要重绘
    
    // 创建消息变量
    ExMessage msg;
    
    while (running) {
        // 只在需要时重绘
        if (needRedraw) {
            // 清屏
            cleardevice();
            
            // 显示提示信息
            settextcolor(WHITE);
            settextstyle(30, 0, _T("宋体"));
            outtextxy(200, 100, _T("图标切换测试"));
            
            settextstyle(20, 0, _T("宋体"));
            outtextxy(200, 200, _T("按F9键切换图标"));
            outtextxy(200, 230, _T("按ESC键退出程序"));
            
            TCHAR info[100];
            _stprintf(info, _T("当前图标: %s"), iconPaths[currentIconIndex]);
            outtextxy(200, 300, info);
            
            // 刷新屏幕
            FlushBatchDraw();
            
            needRedraw = false;
        }
        
        // 检测消息
        while (peekmessage(&msg)) {
            switch (msg.message) {
                case WM_KEYDOWN:
                    // F9键切换图标 (F9的虚拟键码是VK_F9，即120)
                    if (msg.vkcode == VK_F9) {
                        currentIconIndex = (currentIconIndex + 1) % iconPaths.size();
                        
                        if (SetWindowIcon(iconPaths[currentIconIndex])) {
                            printf("已切换图标: %s\n", iconPaths[currentIconIndex]);
                        } else {
                            printf("切换图标失败\n");
                        }
                        
                        needRedraw = true;  // 切换图标后需要重绘
                    }
                    // ESC键退出
                    else if (msg.vkcode == VK_ESCAPE) {
                        running = false;
                        printf("ESC键被按下，程序退出...\n");
                    }
                    break;
            }
        }
        
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
