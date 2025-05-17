#include <easyx.h>      // EasyXͼ�ο�
#include <graphics.h>   // EasyXͼ�ο���չ
#include <stdio.h>      // ��׼�������
#include <conio.h>      // ����̨�������
#include <windows.h>    // Windows API

// �Զ��崰�ڹ��̺���
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
    // ��������̨����
    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    
    printf("���ڴ�С��λ�õ��ڲ��Գ���\n\n");
    
    // ��ʼ���ڲ���
    int windowWidth = 800;
    int windowHeight = 600;
    int windowX = 100;
    int windowY = 100;
    
    // ��ʼ��ͼ�δ��ڣ�����ʾ��
    HWND hwnd = initgraph(windowWidth, windowHeight, EW_NOMINIMIZE | EW_SHOWCONSOLE);
    
    // ���ô��ڱ���
    SetWindowText(hwnd, _T("���ڴ�С��λ�ò���"));
    
    // ���ô���λ��
    SetWindowPos(hwnd, HWND_TOP, windowX, windowY, windowWidth, windowHeight, SWP_SHOWWINDOW);
    
    // ��ʾ��ǰ������Ϣ
    printf("��ǰ���ڴ�С: %d x %d\n", windowWidth, windowHeight);
    printf("��ǰ����λ��: (%d, %d)\n\n", windowX, windowY);
    
    // ��ʾ����˵��
    printf("����˵��:\n");
    printf("1 - ���Ӵ��ڿ��\n");
    printf("2 - ���ٴ��ڿ��\n");
    printf("3 - ���Ӵ��ڸ߶�\n");
    printf("4 - ���ٴ��ڸ߶�\n");
    printf("W - �����ƶ�����\n");
    printf("S - �����ƶ�����\n");
    printf("A - �����ƶ�����\n");
    printf("D - �����ƶ�����\n");
    printf("C - �����ھ�����ʾ\n");
    printf("R - ���ô��ڴ�С��λ��\n");
    printf("ESC - �˳�����\n\n");
    
    // ������Ϣ����
    ExMessage msg;
    
    // ���´������ݵĺ���
    auto updateWindow = [&]() {
        // ����
        cleardevice();
        
        // ����
        settextcolor(RGB(255, 255, 255));
        settextstyle(24, 0, _T("����"));
        outtextxy(10, 10, _T("���ڴ�С��λ�ò���"));
        
        // ���Ʊ߿�
        setlinecolor(RGB(255, 0, 0));
        rectangle(10, 50, windowWidth - 10, windowHeight - 10);
        
        // ��ʾ���ڳߴ�
        settextstyle(20, 0, _T("����"));
        TCHAR sizeText[50];
        _stprintf(sizeText, _T("���ڴ�С: %d x %d"), windowWidth, windowHeight);
        outtextxy(10, 60, sizeText);
        
        TCHAR posText[50];
        _stprintf(posText, _T("����λ��: (%d, %d)"), windowX, windowY);
        outtextxy(10, 90, posText);
        
        // ��Ӳ���˵��������
        settextcolor(RGB(200, 200, 200));
        settextstyle(16, 0, _T("����"));
        outtextxy(10, 130, _T("����˵��:"));
        outtextxy(10, 150, _T("1 - ���Ӵ��ڿ��"));
        outtextxy(10, 170, _T("2 - ���ٴ��ڿ��"));
        outtextxy(10, 190, _T("3 - ���Ӵ��ڸ߶�"));
        outtextxy(10, 210, _T("4 - ���ٴ��ڸ߶�"));
        outtextxy(10, 230, _T("W - �����ƶ�����"));
        outtextxy(10, 250, _T("S - �����ƶ�����"));
        outtextxy(10, 270, _T("A - �����ƶ�����"));
        outtextxy(10, 290, _T("D - �����ƶ�����"));
        outtextxy(10, 310, _T("C - �����ھ�����ʾ"));
        outtextxy(10, 330, _T("R - ���ô��ڴ�С��λ��"));
        outtextxy(10, 350, _T("ESC - �˳�����"));
    };
    
    // ��ʼ����
    updateWindow();
    
    // ��ѭ��
    bool running = true;
    while (running)
    {
        // ��ȡ��Ϣ
        if (peekmessage(&msg))
        {
            if (msg.message == WM_KEYDOWN)
            {
                // ���´��ڲ���
                switch (msg.vkcode)
                {
                    case '1':  // ���ӿ��
                        windowWidth += 50;
                        break;
                        
                    case '2':  // ���ٿ��
                        if (windowWidth > 300)
                            windowWidth -= 50;
                        break;
                        
                    case '3':  // ���Ӹ߶�
                        windowHeight += 50;
                        break;
                        
                    case '4':  // ���ٸ߶�
                        if (windowHeight > 200)
                            windowHeight -= 50;
                        break;
                        
                    case 'W':  // �����ƶ�
                        windowY -= 50;
                        break;
                        
                    case 'S':  // �����ƶ�
                        windowY += 50;
                        break;
                        
                    case 'A':  // �����ƶ�
                        windowX -= 50;
                        break;
                        
                    case 'D':  // �����ƶ�
                        windowX += 50;
                        break;
                        
                    case 'C':  // ���ھ���
                        {
                            int screenWidth = GetSystemMetrics(SM_CXSCREEN);
                            int screenHeight = GetSystemMetrics(SM_CYSCREEN);
                            windowX = (screenWidth - windowWidth) / 2;
                            windowY = (screenHeight - windowHeight) / 2;
                        }
                        break;
                        
                    case 'R':  // ���ô���
                        windowWidth = 800;
                        windowHeight = 600;
                        windowX = 100;
                        windowY = 100;
                        break;
                        
                    case VK_ESCAPE:  // ESC���˳�
                        running = false;
                        printf("ESC�������£������˳�...\n");
                        break;
                }
                
                // �������ڴ�С��λ��
                SetWindowPos(hwnd, HWND_TOP, windowX, windowY, windowWidth, windowHeight, SWP_SHOWWINDOW);
                
                // ������ʾ
                updateWindow();
                
                // ��ʾ��ǰ������Ϣ������̨
                printf("��ǰ���ڴ�С: %d x %d\n", windowWidth, windowHeight);
                printf("��ǰ����λ��: (%d, %d)\n", windowX, windowY);
            }
        }
        
        // ��ʱ
        Sleep(10);
    }
    
    // �رտ���̨
    FreeConsole();
    
    // �ر�ͼ�δ���
    closegraph();
    return 0;
}
