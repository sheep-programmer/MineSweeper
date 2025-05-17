#include <easyx.h>      // EasyXͼ�ο�
#include <graphics.h>   // EasyXͼ�ο���չ
#include <stdio.h>      // ��׼�������
#include <conio.h>      // ����̨�������

int main()
{
    // ��ʼ��ͼ�δ���
    initgraph(800, 600, EX_SHOWCONSOLE);
    
    // ���ô��ڱ���
    HWND hwnd = GetHWnd();
    SetWindowText(hwnd, _T("���������"));
    
    // ��������̨����
    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    
    printf("��������Գ���������\n");
    printf("����ͼ�δ����е�������꽫��ʾ�ڴ˿���̨��\n");
    printf("��ESC���˳�����\n\n");
    
    // ���ñ�����ɫΪ��ɫ
    setbkcolor(WHITE);
    cleardevice();
    
    // ��ʾ��ʾ��Ϣ
    settextcolor(RGB(0, 0, 255));
    settextstyle(24, 0, _T("����"));
    outtextxy(10, 10, _T("�����Ļ����λ�ã����꽫��ʾ�ڿ���̨"));
    outtextxy(10, 40, _T("��ESC���˳�����"));
    
    // ������Ϣ����
    ExMessage msg;
    
    // ��ѭ��
    bool running = true;
    while (running)
    {
        // ��ȡ��Ϣ
        if (peekmessage(&msg))
        {
            switch (msg.message)
            {
                case WM_KEYDOWN:
                    // ���ESC��
                    if (msg.vkcode == VK_ESCAPE)
                    {
                        running = false;
                        printf("ESC�������£������˳�...\n");
                    }
                    break;
                    
                case WM_LBUTTONDOWN:  // ������
                    printf("������ - ����: (%d, %d)\n", msg.x, msg.y);
                    
                    // �ڵ��λ�û���һ��С���
                    setfillcolor(RGB(255, 0, 0));
                    solidcircle(msg.x, msg.y, 5);
                    break;
                    
                case WM_RBUTTONDOWN:  // �Ҽ����
                    printf("�Ҽ���� - ����: (%d, %d)\n", msg.x, msg.y);
                    
                    // �ڵ��λ�û���һ��С���
                    setfillcolor(RGB(255, 0, 0));
                    solidcircle(msg.x, msg.y, 5);
                    break;
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
