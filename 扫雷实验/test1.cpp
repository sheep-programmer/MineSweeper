#include <easyx.h>      // EasyXͼ�ο�
#include <graphics.h>   // EasyXͼ�ο���չ
#include <stdio.h>      // ��׼�������
#include <conio.h>      // ����̨�������

int main()
{
    // ��ʼ��ͼ�δ���
    initgraph(800, 600);
    
    // ���ô��ڱ���
    HWND hwnd = GetHWnd();
    SetWindowText(hwnd, _T("ͼƬ��ʾ�����Ų���"));
    
    // ����ͼƬ
    IMAGE img;
    loadimage(&img, _T("images/test.png"));  // ��ȷ��ͼƬ·����ȷ
    
    // ��ȡͼƬ�ߴ�
    int imgWidth = img.getwidth();
    int imgHeight = img.getheight();
    
    // ���ű���
    float scale = 1.0f;
    
    // ��������˫������ڴ�DC
    IMAGE backBuffer(getwidth(), getheight());
    
    // ������Ϣ����
    MOUSEMSG m;
    ExMessage msg;
    
    // ��ѭ��
    bool running = true;
    while (running)
    {
        // ���û�ͼĿ��Ϊ�ڴ滺����
        SetWorkingImage(&backBuffer);
        
        // ��ջ�����
        cleardevice();
        
        // �������ź�ĳߴ�
        int newWidth = (int)(imgWidth * scale);
        int newHeight = (int)(imgHeight * scale);
        
        // �������ź��ͼƬ
        IMAGE scaledImg;
        scaledImg.Resize(newWidth, newHeight);
        
        // ��ԭͼ���Ų����Ƶ���ͼ��
        SetStretchBltMode(GetImageHDC(&scaledImg), HALFTONE);
        StretchBlt(GetImageHDC(&scaledImg), 0, 0, newWidth, newHeight,
                  GetImageHDC(&img), 0, 0, imgWidth, imgHeight, SRCCOPY);
        
        // ����Ļ������ʾͼƬ
        int x = (getwidth() - newWidth) / 2;
        int y = (getheight() - newHeight) / 2;
        putimage(x, y, &scaledImg);
        
        // ��ʾ����˵��
        settextcolor(RGB(255, 255, 255));
        settextstyle(20, 0, _T("����"));
        outtextxy(10, 10, _T("�� + �� = ���Ŵ�ͼƬ"));
        outtextxy(10, 40, _T("�� - ����СͼƬ"));
        outtextxy(10, 70, _T("�� ESC ���˳�����"));
        
        // ��ʾ��ǰ���ű���
        TCHAR scaleText[50];
        _stprintf(scaleText, _T("��ǰ���ű���: %.2f"), scale);
        outtextxy(10, 100, scaleText);
        
        // ����ͼĿ�����û���Ļ
        SetWorkingImage(NULL);
        
        // һ���Խ����������ݸ��Ƶ���Ļ��
        putimage(0, 0, &backBuffer);
        
        // ������Ϣ
        while (peekmessage(&msg, EM_KEY))
        {
            switch (msg.vkcode)
            {
                case VK_ADD:       // ���ּ��̵ļӺ�
                case VK_OEM_PLUS:  // �����̵ļӺ�/�Ⱥż�
                    scale += 0.1f;
                    break;
                    
                case VK_SUBTRACT:  // ���ּ��̵ļ���
                case VK_OEM_MINUS: // �����̵ļ��ż�
                    if (scale > 0.1f)
                        scale -= 0.1f;
                    break;
                    
                case VK_ESCAPE:    // ESC��
                    running = false;
                    break;
            }
        }
        
        // ��ʱ������CPUʹ����
        Sleep(10);
    }
    
    // �ر�ͼ�δ���
    closegraph();
    return 0;
}
