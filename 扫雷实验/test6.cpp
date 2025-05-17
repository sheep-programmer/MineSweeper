#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>
#include <easyx.h>
#include <graphics.h>
#pragma comment(lib, "winmm.lib")

int main()
{
    // 创建EasyX窗口
    initgraph(640, 480, EX_SHOWCONSOLE);
    
    // 设置窗口标题
    SetWindowText(GetHWnd(), _T("声音播放测试"));
    
    // 设置文字背景透明
    setbkmode(TRANSPARENT);
    
    // 定义按钮区域
    int btnX = 220, btnY = 200, btnWidth = 200, btnHeight = 60;
    
    // 预加载声音文件
    mciSendStringW(L"open sounds/boom.wav alias clicksound", NULL, 0, NULL);
    
    // 创建消息变量
    ExMessage msg;
    
    // 开启双缓冲绘图
    BeginBatchDraw();
    
    // 绘制界面（只绘制一次固定内容）
    cleardevice();
    
    // 显示标题
    settextcolor(WHITE);
    settextstyle(30, 0, _T("宋体"));
    outtextxy(200, 80, _T("声音播放测试"));
    
    // 显示提示
    settextstyle(20, 0, _T("宋体"));
    outtextxy(180, 140, _T("点击下面的按钮播放声音"));
    
    // 显示退出提示
    outtextxy(220, 300, _T("按ESC键退出"));
    
    // 主循环
    bool running = true;
    bool btnPressed = false;
    
    while (running) {
        // 绘制按钮（只重绘按钮部分）
        if (btnPressed) {
            setfillcolor(RGB(0, 0, 150));  // 深蓝色（按下状态）
        } else {
            setfillcolor(BLUE);  // 蓝色（正常状态）
        }
        solidrectangle(btnX, btnY, btnX + btnWidth, btnY + btnHeight);
        settextcolor(WHITE);
        settextstyle(24, 0, _T("宋体"));
        outtextxy(btnX + 50, btnY + 15, _T("播放声音"));
        
        // 刷新屏幕
        FlushBatchDraw();
        
        // 处理消息
        if (peekmessage(&msg)) {
            // 检测ESC键退出
            if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE) {
                running = false;
            }
            
            // 检测鼠标按下按钮
            if (msg.message == WM_LBUTTONDOWN) {
                if (msg.x >= btnX && msg.x <= btnX + btnWidth && 
                    msg.y >= btnY && msg.y <= btnY + btnHeight) {
                    btnPressed = true;
                }
            }
            
            // 检测鼠标释放按钮
            if (msg.message == WM_LBUTTONUP) {
                if (btnPressed && msg.x >= btnX && msg.x <= btnX + btnWidth && 
                    msg.y >= btnY && msg.y <= btnY + btnHeight) {
                    // 播放声音（使用别名直接播放）
                    mciSendStringW(L"seek clicksound to start", NULL, 0, NULL);
                    mciSendStringW(L"play clicksound", NULL, 0, NULL);
                }
                btnPressed = false;
            }
        }
        
        // 延时
        Sleep(10);
    }
    
    // 结束批量绘图
    EndBatchDraw();
    
    // 关闭声音
    mciSendStringW(L"close clicksound", NULL, 0, NULL);
    
    // 关闭图形窗口
    closegraph();
    
    return 0;
}
