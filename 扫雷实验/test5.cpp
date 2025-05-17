#include <easyx.h>      // EasyXͼ�ο�
#include <graphics.h>   // EasyXͼ�ο���չ
#include <stdio.h>      // ��׼�������
#include <conio.h>      // ����̨�������
#include <string>       // �ַ�������
#include <windows.h>    // Windows API
#include <vector>       // ��������

// ���ô���ͼ��ĺ���
bool SetWindowIcon(const TCHAR* iconPath) {
    // ��ȡ���ھ��
    HWND hwnd = GetHWnd();
    
    // ����ͼ��
    HICON hIcon = (HICON)LoadImage(
        NULL,                   // ʵ�����
        iconPath,               // ͼ��·��
        IMAGE_ICON,             // ͼ������
        32,                     // ���
        32,                     // �߶�
        LR_LOADFROMFILE         // ���ļ�����
    );
    
    if (hIcon) {
        // ���ô�ͼ���Сͼ��
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        return true;
    }
    
    return false;
}

int main() {
    // ��ʼ��ͼ�δ���
    initgraph(800, 600);
    
    // ���ô��ڱ���
    SetWindowText(GetHWnd(), _T("ͼ���л�����"));
    
    // ��������̨����
    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    
    printf("ͼ���л����Գ���������\n");
    printf("��F9���л�ͼ��\n");
    printf("��ESC���˳�����\n\n");
    
    // �������ֱ���Ϊ͸��
    setbkmode(TRANSPARENT);
    
    // ͼ���ļ��б�
    std::vector<const TCHAR*> iconPaths;
    iconPaths.push_back(_T("images/logo1.ico"));
    iconPaths.push_back(_T("images/logo2.ico"));
    iconPaths.push_back(_T("images/logo3.ico"));
    iconPaths.push_back(_T("images/logo4.ico"));
    iconPaths.push_back(_T("images/logo5.ico"));
    
    // ��ǰͼ������
    int currentIconIndex = 0;
    
    // ���ó�ʼͼ��
    if (SetWindowIcon(iconPaths[currentIconIndex])) {
        printf("�����ó�ʼͼ��: %s\n", iconPaths[currentIconIndex]);
    } else {
        printf("���ó�ʼͼ��ʧ��\n");
    }
    
    // ����˫�����ͼ
    BeginBatchDraw();
    
    // ��ѭ��
    bool running = true;
    bool needRedraw = true;  // �Ƿ���Ҫ�ػ�
    
    // ������Ϣ����
    ExMessage msg;
    
    while (running) {
        // ֻ����Ҫʱ�ػ�
        if (needRedraw) {
            // ����
            cleardevice();
            
            // ��ʾ��ʾ��Ϣ
            settextcolor(WHITE);
            settextstyle(30, 0, _T("����"));
            outtextxy(200, 100, _T("ͼ���л�����"));
            
            settextstyle(20, 0, _T("����"));
            outtextxy(200, 200, _T("��F9���л�ͼ��"));
            outtextxy(200, 230, _T("��ESC���˳�����"));
            
            TCHAR info[100];
            _stprintf(info, _T("��ǰͼ��: %s"), iconPaths[currentIconIndex]);
            outtextxy(200, 300, info);
            
            // ˢ����Ļ
            FlushBatchDraw();
            
            needRedraw = false;
        }
        
        // �����Ϣ
        while (peekmessage(&msg)) {
            switch (msg.message) {
                case WM_KEYDOWN:
                    // F9���л�ͼ�� (F9�����������VK_F9����120)
                    if (msg.vkcode == VK_F9) {
                        currentIconIndex = (currentIconIndex + 1) % iconPaths.size();
                        
                        if (SetWindowIcon(iconPaths[currentIconIndex])) {
                            printf("���л�ͼ��: %s\n", iconPaths[currentIconIndex]);
                        } else {
                            printf("�л�ͼ��ʧ��\n");
                        }
                        
                        needRedraw = true;  // �л�ͼ�����Ҫ�ػ�
                    }
                    // ESC���˳�
                    else if (msg.vkcode == VK_ESCAPE) {
                        running = false;
                        printf("ESC�������£������˳�...\n");
                    }
                    break;
            }
        }
        
        // ��ʱ
        Sleep(10);
    }
    
    // ����������ͼ
    EndBatchDraw();
    
    // �رտ���̨
    FreeConsole();
    
    // �ر�ͼ�δ���
    closegraph();
    return 0;
}
