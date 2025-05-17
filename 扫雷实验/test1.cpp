#include <easyx.h>      // EasyX图形库
#include <graphics.h>   // EasyX图形库扩展
#include <stdio.h>      // 标准输入输出
#include <conio.h>      // 控制台输入输出

int main()
{
    // 初始化图形窗口
    initgraph(800, 600);
    
    // 设置窗口标题
    HWND hwnd = GetHWnd();
    SetWindowText(hwnd, _T("图片显示与缩放测试"));
    
    // 加载图片
    IMAGE img;
    loadimage(&img, _T("images/test.png"));  // 请确保图片路径正确
    
    // 获取图片尺寸
    int imgWidth = img.getwidth();
    int imgHeight = img.getheight();
    
    // 缩放比例
    float scale = 1.0f;
    
    // 创建用于双缓冲的内存DC
    IMAGE backBuffer(getwidth(), getheight());
    
    // 启用消息机制
    MOUSEMSG m;
    ExMessage msg;
    
    // 主循环
    bool running = true;
    while (running)
    {
        // 设置绘图目标为内存缓冲区
        SetWorkingImage(&backBuffer);
        
        // 清空缓冲区
        cleardevice();
        
        // 计算缩放后的尺寸
        int newWidth = (int)(imgWidth * scale);
        int newHeight = (int)(imgHeight * scale);
        
        // 创建缩放后的图片
        IMAGE scaledImg;
        scaledImg.Resize(newWidth, newHeight);
        
        // 将原图缩放并绘制到新图上
        SetStretchBltMode(GetImageHDC(&scaledImg), HALFTONE);
        StretchBlt(GetImageHDC(&scaledImg), 0, 0, newWidth, newHeight,
                  GetImageHDC(&img), 0, 0, imgWidth, imgHeight, SRCCOPY);
        
        // 在屏幕中央显示图片
        int x = (getwidth() - newWidth) / 2;
        int y = (getheight() - newHeight) / 2;
        putimage(x, y, &scaledImg);
        
        // 显示操作说明
        settextcolor(RGB(255, 255, 255));
        settextstyle(20, 0, _T("宋体"));
        outtextxy(10, 10, _T("按 + 或 = 键放大图片"));
        outtextxy(10, 40, _T("按 - 键缩小图片"));
        outtextxy(10, 70, _T("按 ESC 键退出程序"));
        
        // 显示当前缩放比例
        TCHAR scaleText[50];
        _stprintf(scaleText, _T("当前缩放比例: %.2f"), scale);
        outtextxy(10, 100, scaleText);
        
        // 将绘图目标设置回屏幕
        SetWorkingImage(NULL);
        
        // 一次性将缓冲区内容复制到屏幕上
        putimage(0, 0, &backBuffer);
        
        // 处理消息
        while (peekmessage(&msg, EM_KEY))
        {
            switch (msg.vkcode)
            {
                case VK_ADD:       // 数字键盘的加号
                case VK_OEM_PLUS:  // 主键盘的加号/等号键
                    scale += 0.1f;
                    break;
                    
                case VK_SUBTRACT:  // 数字键盘的减号
                case VK_OEM_MINUS: // 主键盘的减号键
                    if (scale > 0.1f)
                        scale -= 0.1f;
                    break;
                    
                case VK_ESCAPE:    // ESC键
                    running = false;
                    break;
            }
        }
        
        // 延时，降低CPU使用率
        Sleep(10);
    }
    
    // 关闭图形窗口
    closegraph();
    return 0;
}
