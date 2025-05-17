#include <easyx.h>      // EasyXͼ�ο�
#include <graphics.h>   // EasyXͼ�ο���չ
#include <stdio.h>      // ��׼�������
#include <conio.h>      // ����̨�������
#include <string>       // �ַ�������
#include <windows.h>    // Windows API

// ��ť�ṹ��
struct Button {
    int x, y;           // ��ťλ��
    int width, height;  // ��ť��С
    TCHAR text[50];     // ��ť�ı�
    bool hover;         // �����ͣ״̬
    bool pressed;       // ��ť����״̬
    int clickCount;     // �������
    COLORREF textColor; // ������ɫ
    
    // ���캯��
    Button() : hover(false), pressed(false), clickCount(0), textColor(RGB(255, 255, 255)) {
        text[0] = _T('\0');
    }
    
    // �����Ƿ��ڰ�ť��
    bool contains(int px, int py) {
        return px >= x && px <= x + width && py >= y && py <= y + height;
    }
    
    // ���ư�ť
    void draw() {
        // ���ð�ť��ɫ
        if (pressed) {
            setfillcolor(RGB(60, 110, 200));   // ������ɫ
        } else if (hover) {
            setfillcolor(RGB(100, 150, 255));  // ��ͣ��ɫ
        } else {
            setfillcolor(RGB(70, 130, 230));   // ������ɫ
        }
        
        // ���ư�ť����
        solidroundrect(x, y, x + width, y + height, 10, 10);
        
        // ���ư�ť�߿�
        setlinecolor(RGB(40, 80, 160));
        roundrect(x, y, x + width, y + height, 10, 10);
        
        // ���ư�ť�ı�
        settextcolor(textColor);
        settextstyle(20, 0, _T("����"));
        setbkmode(TRANSPARENT);  // �������ֱ���Ϊ͸����ȥ����ɫ�߿�
        int textWidth = textwidth(text);
        int textHeight = textheight(text);
        outtextxy(x + (width - textWidth) / 2, y + (height - textHeight) / 2, text);
        
        // ��ʾ�������
        TCHAR countText[20];
        _stprintf(countText, _T("���: %d"), clickCount);
        settextstyle(16, 0, _T("����"));
        outtextxy(x + (width - textwidth(countText)) / 2, y + height - 20, countText);
    }
};

// ����Ӧ�ó���ͼ��
bool SetAppIcon(HWND hwnd, const TCHAR* iconPath) {
    HICON hIcon = (HICON)LoadImage(
        NULL,                   // ʵ�����
        iconPath,               // ͼ��·��
        IMAGE_ICON,             // ͼ������
        32,                     // ���
        32,                     // �߶�
        LR_LOADFROMFILE         // ���ļ�����
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
    // ��ʼ��ͼ�δ���
    initgraph(800, 600, EX_SHOWCONSOLE);
    
    // �������ֱ���Ϊ͸����ȥ����ɫ�߿�
    setbkmode(TRANSPARENT);
    
    // ���ô��ڱ���
    HWND hwnd = GetHWnd();
    SetWindowText(hwnd, _T("��ť�������"));
    
    // ��������̨����
    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    
    printf("��ť������Գ���������\n");
    printf("������ť���Թ���\n");
    printf("��ESC���˳�����\n\n");
    
    // ������ť
    Button buttons[3];
    
    // ��ť1����ɫ��ť
    buttons[0].x = 100;
    buttons[0].y = 200;
    buttons[0].width = 150;
    buttons[0].height = 80;
    _tcscpy(buttons[0].text, _T("��ɫ��ť"));
    buttons[0].textColor = RGB(255, 0, 0);
    
    // ��ť2����ɫ��ť
    buttons[1].x = 325;
    buttons[1].y = 200;
    buttons[1].width = 150;
    buttons[1].height = 80;
    _tcscpy(buttons[1].text, _T("��ɫ��ť"));
    buttons[1].textColor = RGB(0, 255, 0);
    
    // ��ť3����ɫ��ť
    buttons[2].x = 550;
    buttons[2].y = 200;
    buttons[2].width = 150;
    buttons[2].height = 80;
    _tcscpy(buttons[2].text, _T("��ɫ��ť"));
    buttons[2].textColor = RGB(0, 0, 255);
    
    // ��Ϣ��ʾ����
    TCHAR messageText[100] = _T("��������İ�ť���в���");
    
    // ������Ϣ����
    ExMessage msg;
    
    // ��ʼ������ͼģʽ
    BeginBatchDraw();
    
    // ��ѭ��
    bool running = true;
    while (running)
    {
        // ����
        cleardevice();
        
        // ���Ʊ���
        settextcolor(RGB(255, 255, 255));
        settextstyle(32, 0, _T("����"));
        outtextxy(300, 50, _T("��ť�������"));
        
        // ������Ϣ
        settextstyle(20, 0, _T("����"));
        outtextxy(100, 350, messageText);
        
        // ����˵��
        settextcolor(RGB(200, 200, 200));
        settextstyle(18, 0, _T("����"));
        outtextxy(100, 400, _T("������: ���Ӱ�ť�������"));
        outtextxy(100, 430, _T("�Ҽ����: ���ð�ť�������"));
        outtextxy(100, 460, _T("��ESC���˳�����"));
        
        // ���ư�ť
        for (int i = 0; i < 3; i++) {
            buttons[i].draw();
        }
        
        // ������Ϣ
        while (peekmessage(&msg)) {
            switch (msg.message) {
                case WM_MOUSEMOVE:
                    // ���°�ť��ͣ״̬
                    for (int i = 0; i < 3; i++) {
                        buttons[i].hover = buttons[i].contains(msg.x, msg.y);
                    }
                    break;
                    
                case WM_LBUTTONDOWN:
                    // ���°�ť����״̬
                    for (int i = 0; i < 3; i++) {
                        if (buttons[i].contains(msg.x, msg.y)) {
                            buttons[i].pressed = true;
                        }
                    }
                    break;
                    
                case WM_LBUTTONUP:
                    // ����Ƿ�����ť
                    for (int i = 0; i < 3; i++) {
                        if (buttons[i].pressed && buttons[i].contains(msg.x, msg.y)) {
                            // ���ӵ������
                            buttons[i].clickCount++;
                            
                            // ������Ϣ
                            _stprintf(messageText, _T("�������� %s (�ܼ�: %d ��)"), 
                                     buttons[i].text, buttons[i].clickCount);
                            
                            // ���������̨
                            printf("��������ť: %s (�������: %d)\n", 
                                  i == 0 ? "��ɫ��ť" : (i == 1 ? "��ɫ��ť" : "��ɫ��ť"), 
                                  buttons[i].clickCount);
                            
                            // ���ݰ�ť���ı���ɫ
                            if (i == 0) {
                                setbkcolor(RGB(50, 0, 0));  // ����ɫ����
                            } else if (i == 1) {
                                setbkcolor(RGB(0, 50, 0));  // ����ɫ����
                            } else if (i == 2) {
                                setbkcolor(RGB(0, 0, 50));  // ����ɫ����
                            }
                        }
                        
                        // ���ð���״̬
                        buttons[i].pressed = false;
                    }
                    break;
                    
                case WM_RBUTTONDOWN:
                    // ����Ƿ��Ҽ������ť
                    for (int i = 0; i < 3; i++) {
                        if (buttons[i].contains(msg.x, msg.y)) {
                            // ���õ������
                            buttons[i].clickCount = 0;
                            
                            // ������Ϣ
                            _stprintf(messageText, _T("�Ҽ���������� %s �ĵ������"), buttons[i].text);
                            
                            // ���������̨
                            printf("�Ҽ�������ð�ť: %s\n", 
                                  i == 0 ? "��ɫ��ť" : (i == 1 ? "��ɫ��ť" : "��ɫ��ť"));
                        }
                    }
                    break;
                    
                case WM_KEYDOWN:
                    if (msg.vkcode == VK_ESCAPE) {
                        running = false;
                        printf("ESC�������£������˳�...\n");
                    }
                    break;
            }
        }
        
        // ˢ����Ļ
        FlushBatchDraw();
        
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
