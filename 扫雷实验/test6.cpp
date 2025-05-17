#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>
#include <easyx.h>
#include <graphics.h>
#pragma comment(lib, "winmm.lib")

int main()
{
    // ����EasyX����
    initgraph(640, 480, EX_SHOWCONSOLE);
    
    // ���ô��ڱ���
    SetWindowText(GetHWnd(), _T("�������Ų���"));
    
    // �������ֱ���͸��
    setbkmode(TRANSPARENT);
    
    // ���尴ť����
    int btnX = 220, btnY = 200, btnWidth = 200, btnHeight = 60;
    
    // Ԥ���������ļ�
    mciSendStringW(L"open sounds/boom.wav alias clicksound", NULL, 0, NULL);
    
    // ������Ϣ����
    ExMessage msg;
    
    // ����˫�����ͼ
    BeginBatchDraw();
    
    // ���ƽ��棨ֻ����һ�ι̶����ݣ�
    cleardevice();
    
    // ��ʾ����
    settextcolor(WHITE);
    settextstyle(30, 0, _T("����"));
    outtextxy(200, 80, _T("�������Ų���"));
    
    // ��ʾ��ʾ
    settextstyle(20, 0, _T("����"));
    outtextxy(180, 140, _T("�������İ�ť��������"));
    
    // ��ʾ�˳���ʾ
    outtextxy(220, 300, _T("��ESC���˳�"));
    
    // ��ѭ��
    bool running = true;
    bool btnPressed = false;
    
    while (running) {
        // ���ư�ť��ֻ�ػ水ť���֣�
        if (btnPressed) {
            setfillcolor(RGB(0, 0, 150));  // ����ɫ������״̬��
        } else {
            setfillcolor(BLUE);  // ��ɫ������״̬��
        }
        solidrectangle(btnX, btnY, btnX + btnWidth, btnY + btnHeight);
        settextcolor(WHITE);
        settextstyle(24, 0, _T("����"));
        outtextxy(btnX + 50, btnY + 15, _T("��������"));
        
        // ˢ����Ļ
        FlushBatchDraw();
        
        // ������Ϣ
        if (peekmessage(&msg)) {
            // ���ESC���˳�
            if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE) {
                running = false;
            }
            
            // �����갴�°�ť
            if (msg.message == WM_LBUTTONDOWN) {
                if (msg.x >= btnX && msg.x <= btnX + btnWidth && 
                    msg.y >= btnY && msg.y <= btnY + btnHeight) {
                    btnPressed = true;
                }
            }
            
            // �������ͷŰ�ť
            if (msg.message == WM_LBUTTONUP) {
                if (btnPressed && msg.x >= btnX && msg.x <= btnX + btnWidth && 
                    msg.y >= btnY && msg.y <= btnY + btnHeight) {
                    // ����������ʹ�ñ���ֱ�Ӳ��ţ�
                    mciSendStringW(L"seek clicksound to start", NULL, 0, NULL);
                    mciSendStringW(L"play clicksound", NULL, 0, NULL);
                }
                btnPressed = false;
            }
        }
        
        // ��ʱ
        Sleep(10);
    }
    
    // ����������ͼ
    EndBatchDraw();
    
    // �ر�����
    mciSendStringW(L"close clicksound", NULL, 0, NULL);
    
    // �ر�ͼ�δ���
    closegraph();
    
    return 0;
}
