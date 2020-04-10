/*-----------------------------------------------------------------------------
    ÉsÉNÉ`ÉÉÅ[É{ÉbÉNÉXÇ÷ÇÃï`âÊÉvÉçÉOÉâÉÄÅiÉTÉìÉvÉã2Åj

    èäÅXÇ…
    /*******
    (Å´Ç…ãLèq)
    ********
    Ç∆Ç¢Ç§â”èäÇ™Ç†ÇÈÇÃÇ≈ÇªÇ±ÇåäñÑÇﬂÇµÇƒÇ¢Ç≠


  Å@ÅuOKÅvÉ{É^ÉìÇ™âüÇ≥ÇÍÇÈÇ∆ÉsÉNÉ`ÉÉÅ[É{ÉbÉNÉXì‡Ç…ÉEÉBÉìÉhÉEÇçÏê¨ÇµÅCâ~Ç»Ç«Çï`âÊÅD
  Å@ÉâÉWÉIÉ{É^ÉìÇëIëÇ∑ÇÈÇ±Ç∆Ç≈â~ì‡ÇÃêFÇ™ïœâªÇ∑ÇÈÅD
   
-----------------------------------------------------------------------------*/
#include <windows.h>        //Windowsä¬ã´
#include <windowsx.h>        //Windowsä¬ã´
#include <stdio.h>            //ì¸èoóÕóp
#include <process.h>        //ÉXÉåÉbÉhóp
#include <stdlib.h>

#pragma comment(lib,"winmm.lib")//çÇê∏ìxÉ^ÉCÉ}
#pragma warning(disable: 4996)

#include "header.h"            //ÉwÉbÉ_Å[ÉtÉ@ÉCÉã
#include "Header1.h"            //ÉwÉbÉ_Å[ÉtÉ@ÉCÉã
#include "resource.h"        //ÉäÉ\Å[ÉXÉtÉ@ÉCÉã

//ç\ë¢ëÃ
typedef struct {
    HWND    hwnd;
    HWND    hEdit;
}SEND_POINTER_STRUCT;


//ÉÅÉCÉìä÷êî(É_ÉCÉAÉçÉOÉoÅ[ÉWÉáÉì)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    HANDLE hMutex;

    //ëΩèdãNìÆîªíË
    hMutex = CreateMutex(NULL, TRUE, DEF_MUTEX_NAME);        //É~ÉÖÅ[ÉeÉbÉNÉXÉIÉuÉWÉFÉNÉgÇÃê∂ê¨
    if (GetLastError() == ERROR_ALREADY_EXISTS) {                //2èdãNìÆÇÃóLñ≥ÇämîF
        MessageBox(NULL, TEXT("ä˘Ç…ãNìÆÇ≥ÇÍÇƒÇ¢Ç‹Ç∑ÅD"), NULL, MB_OK | MB_ICONERROR);
        return 0;                                            //èIóπ
    }

    
    //É_ÉCÉAÉçÉOãNìÆ
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, MainDlgProc);

    return FALSE;            //èIóπ
}

//ÉÅÉCÉìÉvÉçÉVÅ[ÉWÉÉÅiÉ_ÉCÉAÉçÉOÅj
BOOL CALLBACK MainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HFONT hFont;                //ÉtÉHÉìÉg
    static HANDLE hThread;
    static UINT thID;
    static SEND_POINTER_STRUCT Sps;
    static HWND hEdit;

    switch (uMsg) {
    case WM_INITDIALOG:        //É_ÉCÉAÉçÉOèâä˙âª
        Sps.hwnd = hDlg;
        hEdit = GetDlgItem(hDlg, IDC_PICTBOX2);
        Sps.hEdit = hEdit;
        return TRUE;

    case WM_COMMAND:        //É{É^ÉìÇ™âüÇ≥ÇÍÇΩéû
        switch (LOWORD(wParam)) {
        case IDC_BUTTON3:            //OKÉ{É^Éì
                                //ÉfÅ[É^ì«Ç›çûÇ›ÉXÉåÉbÉhãNìÆ

            static HWND hPict_sin, hPict_rec;        //ÉEÉBÉìÉhÉEÉnÉìÉhÉãÅiPictureBoxÅj
            static HWND hWnd_sin, hWnd_rec;        //éqÉEÉBÉìÉhÉEÉnÉìÉhÉã



            hRadioBLACK = GetDlgItem(hDlg, IDC_BUTTON3);

            hPict_sin = GetDlgItem(hDlg, IDC_PICTBOX2);
            WinInitialize(NULL, hDlg, (HMENU)110, "TEST1", hPict_sin, WndProc, &hWnd_sin); //èâä˙âª
            

            hPict_rec = GetDlgItem(hDlg, IDC_PICTBOX3);
            WinInitialize(NULL, hDlg, (HMENU)110, "TEST2", hPict_rec, WndProc, &hWnd_rec); //èâä˙âª
            

            hThread = (HANDLE)_beginthreadex(NULL, 0, TFunc, (PVOID)&Sps, 0, &thID);   //_beginthreadexÅ®ÉXÉåÉbÉhÇóßÇøè„Ç∞ÇÈä÷êî
            
            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON3), FALSE);                         //äJénÉ{É^Éìñ≥å¯âªÅ@Å@Å@Å@//EnableWindowÇ≈ì¸óÕÇñ≥å¯Ç‹ÇΩÇÕóLå¯Ç…Ç∑ÇÈÅB
            
            //WinInitialize(NULL, hDlg, (HMENU)110, "TEST3", hPict_sin, WndProc_sin, &hWnd_sin); //èâä˙âª
            //WinInitialize(NULL, hDlg, (HMENU)110, "TEST4", hPict_rec, WndProc_rec, &hWnd_rec); //èâä˙âª

            
            
            return TRUE;

        case IDC_BUTTON4:    //í‚é~ÅEçƒäJÉ{É^Éì

                        /*Å@ÉTÉXÉyÉìÉhÉJÉEÉìÉ^Å@**************************
                        Å@Å@Å@é¿çsÇãñâ¬Ç∑ÇÈÇ‹Ç≈ÉXÉåÉbÉhÇìÆÇ©Ç≥Ç»Ç¢ÅB
                           Å@Å@ResumeThreadÅFÅ@ÉTÉXÉyÉìÉhÉJÉEÉìÉ^Ç1å∏ÇÁÇ∑
                             Å@SuspendThreadÅFÅ@ÉTÉXÉyÉìÉhÉJÉEÉìÉ^Ç1ëùÇ‚Ç∑

                              0ÇÃÇ∆Ç´ÇÕé¿çsÅBÇªÇÍà»äOÇÕë“ã@Ç∑ÇÈÅB
                              **************************************************/


            if (ResumeThread(hThread) == 0) {                    //í‚é~íÜÇ©Çí≤Ç◊ÇÈ(ÉTÉXÉyÉìÉhÉJÉEÉìÉgÇÇPå∏ÇÁÇ∑)
                SetDlgItemText(hDlg, IDC_BUTTON4, TEXT("çƒäJ"));    //çƒäJÇ…ïœçXÅ@Å@Å@Å@Å@Å@Å@Å@Å@Å@Å@Å@Å@Å@Å@Å@Å@Å@Å@//SetDlgItemTextÇ≈É_ÉCÉAÉçÉOì‡ÇÃÉeÉLÉXÉgÇ»Ç«ÇïœçXÇ∑ÇÈÇ±Ç∆Ç™Ç≈Ç´ÇÈ
                SuspendThread(hThread);                        //ÉXÉåÉbÉhÇÃé¿çsÇí‚é~(ÉTÉXÉyÉìÉhÉJÉEÉìÉgÇÇPëùÇ‚Ç∑)
            }
            else
                SetDlgItemText(hDlg, IDC_BUTTON4, TEXT("í‚é~"));    //í‚é~Ç…ïœçX

            return TRUE;
        }
        break;

    case WM_CLOSE:
        EndDialog(hDlg, 0);            //É_ÉCÉAÉçÉOèIóπ
        return TRUE;
    }

    return FALSE;
}

//ÉfÅ[É^ì«Ç›çûÇ›ópÉXÉåÉbÉh
UINT WINAPI TFunc(LPVOID thParam)
{
    static SEND_POINTER_STRUCT* FU = (SEND_POINTER_STRUCT*)thParam;        //ç\ë¢ëÃÇÃÉ|ÉCÉìÉ^éÊìæ

    FILE* fp;            //ÉtÉ@ÉCÉãÉ|ÉCÉìÉ^
    BOOL Flag = TRUE;        //ÉãÅ[ÉvÉtÉâÉO
    HFONT hFont;        //ÉtÉHÉìÉg



    double sin = 0, sin_bef = 0, rec = 0, rec_bef = 0;        //ÉfÅ[É^
    char str[256];        //ÉfÅ[É^
    char str2[256];     //ï™êîóp
    wchar_t wcs1[256];
    wchar_t wcs2[256];
    size_t ret;

    /*Å@wchar_tå^Å@***********************************************
    charå^ÇÕLPCWSå^Ç∆å›ä∑ê´Ç™Ç»Ç¢ÇΩÇﬂÇªÇÃÇ‹Ç‹ÇæÇ∆ï∂éöâªÇØÇ∑ÇÈÅB
    mbstowcs_sä÷êîÇópÇ¢ÇƒÅAcharÇ©ÇÁwcharÇ…ïœä∑Ç∑ÇÈïKóvÇ™Ç†ÇÈÅB
    ***************************************************************/


    DWORD DNum = 0, beforeTime;

    static int time = 50, count = 50;

    //ÉGÉfÉBÉ^ÇÃÉtÉHÉìÉgïœçX(case WM_INITDIALOG:ÇÃíÜÇ≈ê›íËÇµÇƒÇ‡ó«Ç¢)
    hFont = CreateFont(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_ROMAN, NULL);
    SendMessage(FU->hEdit, WM_SETFONT, (int)hFont, TRUE);

    beforeTime = timeGetTime();                        //åªç›ÇÃéûçèåvéZÅièâä˙éûä‘Åj

                                                    //ÉtÉ@ÉCÉãÉIÅ[ÉvÉì
    if ((fp = fopen("data.txt", "r")) == NULL) {
        MessageBox(NULL, TEXT("Input File Open ERROR!"), NULL, MB_OK | MB_ICONERROR);
        return FALSE;
    }


    //ÉfÅ[É^ì«Ç›çûÇ›ÅEï\é¶


    HDC            hdc, hdc2;
    PAINTSTRUCT ps, ps2;
    HBRUSH        hBrush, hOldBrush, hBrush2, hOldBrush2;    //ÉuÉâÉV
    HPEN        hPen, hOldPen, hPen2, hOldPen2;        //ÉyÉì
    static HWND hWnd;
    static HWND A_2;

    hWnd = GetDlgItem(FU->hwnd, IDC_PICTBOX3);
    hdc = BeginPaint(hWnd, &ps);//ÉfÉoÉCÉXÉRÉìÉeÉLÉXÉgÇÃÉnÉìÉhÉãéÊìæ
    A_2 = GetDlgItem(FU->hwnd, IDC_PICTBOX2);
    hdc2 = BeginPaint(A_2, &ps2);//ÉfÉoÉCÉXÉRÉìÉeÉLÉXÉgÇÃÉnÉìÉhÉãéÊìæ



    //ÉyÉìÅCÉuÉâÉVê∂ê¨
    hBrush = CreateSolidBrush(color);                //ÉuÉâÉVê∂ê¨
    hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);    //ÉuÉâÉVê›íË
    hPen = CreatePen(PS_SOLID, 2, colorPen);        //ÉyÉìê∂ê¨
    hOldPen = (HPEN)SelectObject(hdc, hPen);        //ÉyÉìê›íË
    hBrush2 = CreateSolidBrush(color);                //ÉuÉâÉVê∂ê¨
    hOldBrush2 = (HBRUSH)SelectObject(hdc2, hBrush2);    //ÉuÉâÉVê›íË
    hPen2= CreatePen(PS_SOLID, 2, colorPen);        //ÉyÉìê∂ê¨
    hOldPen2 = (HPEN)SelectObject(hdc2, hPen2);        //ÉyÉìê›íË

    //ï`âÊ
    /********************************


      à»è„ÇéQçlÇ…ê}å`Çï`âÊÇ∑ÇÈä÷êîÇà»â∫Ç…ãLèqÇµÇ‹ÇµÇÂÇ§
    ********************************/
    //Ç±Ç±Ç©ÇÁ


    /*Å@wchar_tå^Å@***********************************************
    charå^ÇÕLPCWSå^Ç∆å›ä∑ê´Ç™Ç»Ç¢ÇΩÇﬂÇªÇÃÇ‹Ç‹ÇæÇ∆ï∂éöâªÇØÇ∑ÇÈÅB
    mbstowcs_sä÷êîÇópÇ¢ÇƒÅAcharÇ©ÇÁwcharÇ…ïœä∑Ç∑ÇÈïKóvÇ™Ç†ÇÈÅB
    ***************************************************************/


    

    //ÉGÉfÉBÉ^ÇÃÉtÉHÉìÉgïœçX(case WM_INITDIALOG:ÇÃíÜÇ≈ê›íËÇµÇƒÇ‡ó«Ç¢)
    hFont = CreateFont(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_ROMAN, NULL);
    //SendMessage(FU->hEdit, WM_SETFONT, (int)hFont, TRUE);

    beforeTime = timeGetTime();                        //åªç›ÇÃéûçèåvéZÅièâä˙éûä‘Åj



    //ÉfÅ[É^ì«Ç›çûÇ›ÅEï\é¶
    colorPen = RGB(255, 255, 255);
    //colorPen = color;    //êFéwíË


    while (Flag == TRUE) {
        DWORD nowTime, progress, idealTime;


        //éûä‘ÇÃí≤êÆ
        nowTime = timeGetTime();                    //åªç›ÇÃéûçèåvéZ
        progress = nowTime - beforeTime;                //èàóùéûä‘ÇåvéZ
        idealTime = (DWORD)(DNum * (1000 / (double)DEF_DATAPERS));    //óùëzéûä‘ÇåvéZ
        if (idealTime > progress) {
            Sleep(idealTime - progress);            //óùëzéûä‘Ç…Ç»ÇÈÇ‹Ç≈ë“ã@
        }

        if (fscanf(fp, "%lf %lf", &sin, &rec) == EOF) {
            MessageBox(NULL, TEXT("èIóπ"), TEXT("INFORMATION"), MB_OK | MB_ICONEXCLAMATION);
            EnableWindow(GetDlgItem(FU->hwnd, IDC_BUTTON3), TRUE);        //äJénÉ{É^ÉìóLå¯
            Flag = FALSE;                                                //ÉãÅ[ÉvèIóπÉtÉâÉO
            return FALSE;
        }


        if (time % 5 == 0) {

            sin = 60 * sin;
            rec = 60 * rec;
            MoveToEx(hdc2, count, 80 - (int)sin_bef, NULL);
            LineTo(hdc2, count + 1, 80 - (int)sin);
            MoveToEx(hdc, count, 80 - (int)rec_bef, NULL);
            LineTo(hdc, count + 1, 80 - (int)rec);

            sin_bef = sin;
            rec_bef = rec;
            count += 1;
        }


        time = time + 1;

        DNum++;

        //àÍïbåoâﬂéû
        if (progress >= 1000.0) {
            beforeTime = nowTime;
            DNum = 0;
        }



        if (count >= 530) {
            count = 50;


            Rectangle(hdc, 0, 0, 579, 162);
            MoveToEx(hdc, 50, 20, NULL);
            LineTo(hdc, 50, 140);
            MoveToEx(hdc, 50, 80, NULL);
            LineTo(hdc, 530, 80);
            Rectangle(hdc2, 0, 0, 579, 162);
            MoveToEx(hdc2, 50, 20, NULL);
            LineTo(hdc2, 50, 140);
            MoveToEx(hdc2, 50, 80, NULL);
            LineTo(hdc2, 530, 80);

            //Ç±Ç±Ç‹Ç≈
            SetBkColor(hdc, RGB(0, 0, 0));
            SetTextColor(hdc, RGB(255, 255, 255));
            TextOut(hdc, 250, 140, TEXT("Times (s)"), 9);        //ÉeÉLÉXÉgï`âÊ
            SetBkColor(hdc2, RGB(0, 0, 0));
            SetTextColor(hdc2, RGB(255, 255, 255));
            TextOut(hdc2, 250, 140, TEXT("Times (s)"), 9);        //ÉeÉLÉXÉgï`âÊ

            

        }


    }


    //ÉyÉìÅCÉuÉâÉVîpä¸
    /********************************

    égÇ¢èIÇÌÇ¡ÇΩÉyÉìÇ∆ÉuÉâÉVÇÕîjä¸Ç∑ÇÈïKóvÇ™Ç†ÇËÇ‹Ç∑ÅD

    ********************************/
    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
    SelectObject(hdc2, hOldBrush2);
    DeleteObject(hBrush2);
    SelectObject(hdc2, hOldPen2);
    DeleteObject(hPen2);

    //ÉfÉoÉCÉXÉRÉìÉeÉLÉXÉgÇÃÉnÉìÉhÉãîjä¸
    EndPaint(FU->hwnd, &ps);
    EndPaint(FU->hwnd, &ps2);

    return 0;

}


HRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    HDC            hdc;                //ÉfÉoÉCÉXÉRÉìÉeÉLÉXÉgÇÃÉnÉìÉhÉã
    PAINTSTRUCT ps;                    //(ç\ë¢ëÃ)ÉNÉâÉCÉAÉìÉgóÃàÊï`âÊÇ∑ÇÈÇΩÇﬂÇÃèÓïÒ
    HBRUSH        hBrush, hOldBrush;    //ÉuÉâÉV
    HPEN        hPen, hOldPen;        //ÉyÉì

    switch (uMsg) {
    case WM_CREATE:
        colorPen = RGB(255, 255, 255);    //êFéwíË
        //colorPen = color;    //êFéwíË
        break;

    case WM_PAINT:

        /********************************

        PictureControlÇ…ï`âÊÇ∑ÇÈÇΩÇﬂÇ…ÇÕÅCHDCå^ÇÃÉnÉìÉhÉãÇï Ç…éÊìæÇ∑ÇÈ
        ïKóvÇ™Ç†ÇËÇ‹Ç∑ÅD

        ó·ÅFhdc = BeginPaint(hWnd, &ps);
        hdc:ÉfÉoÉCÉXÉRÉìÉeÉLÉXÉgÇÃÉnÉìÉhÉã
        hWnd:PictureControlÇÃÉnÉìÉhÉã
        psÅF(ç\ë¢ëÃ)ÉNÉâÉCÉAÉìÉgóÃàÊï`âÊÇ∑ÇÈÇΩÇﬂÇÃèÓïÒ

        ********************************/

        hdc = BeginPaint(hWnd, &ps);//ÉfÉoÉCÉXÉRÉìÉeÉLÉXÉgÇÃÉnÉìÉhÉãéÊìæ

        /********************************

        PictureControlÇ…ï`âÊÇ∑ÇÈÇΩÇﬂÇ…ÇÕÅCê¸Çà¯Ç´ÇΩÇ¢Ç∆Ç´ÇÕÉyÉìÅC
        ìhÇËÇ¬Ç‘Ç∑ç€Ç…ÇÕÉuÉâÉVÇ™ïKóvÇ≈Ç∑ÅD

        ********************************/

        //ÉyÉìÅCÉuÉâÉVê∂ê¨
        hBrush = CreateSolidBrush(color);                //ÉuÉâÉVê∂ê¨
        hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);    //ÉuÉâÉVê›íË
        hPen = CreatePen(PS_SOLID, 2, colorPen);        //ÉyÉìê∂ê¨
        hOldPen = (HPEN)SelectObject(hdc, hPen);        //ÉyÉìê›íË

        //ï`âÊ
        /********************************

        ê}å`Çï`âÊÇ∑ÇÈÇΩÇﬂÇ…ÇÕà»â∫ÇÃä÷êîÇópÇ¢Ç‹Ç∑ÅD
        í∑ï˚å`ÅFRectangle(HDC hdc ,int nLeftRect , int nTopRect ,int nRightRect , int nBottomRect);
        â~ÅFEllipse(HDC hdc ,int nLeftRect , int nTopRect ,int nRightRect , int nBottomRect);

         nLiftRectÅFí∑ï˚å`ÇÃç∂è„Xç¿ïW
          nTopRectÅFç∂è„Yç¿ïW
          nRightRectÅFâEâ∫Xç¿ïW
          nBottomRectÅFâEâ∫ÇÃYç¿ïW

        ê¸Çà¯Ç≠Ç…ÇÕà»â∫ÇÃä÷êîÇópÇ¢Ç‹Ç∑ÅD

        ê¸ÇÃénì_ê›íËÅFMoveToEx(HDC hdc , int X , int Y , NULL);
          X,YÅFê¸ÇÃénì_ÇÃç¿ïW
        ê¸ÅGLineTo(HDC hdc , int nXEnd , int nYEnd);
          nXEnd, nYEndÅFê¸ÇÃèIì_ÇÃê›íË


          à»è„ÇéQçlÇ…ê}å`Çï`âÊÇ∑ÇÈä÷êîÇà»â∫Ç…ãLèqÇµÇ‹ÇµÇÂÇ§
        ********************************/
        //Ç±Ç±Ç©ÇÁ

        Rectangle(hdc, 0, 0, 579, 162);
        MoveToEx(hdc, 50, 20, NULL);
        LineTo(hdc, 50, 140);
        MoveToEx(hdc, 50, 80, NULL);
        LineTo(hdc, 530, 80);

        //Ç±Ç±Ç‹Ç≈

        SetBkColor(hdc, RGB(0, 0, 0));
        SetTextColor(hdc, RGB(255, 255, 255));
        TextOut(hdc, 250, 140, TEXT("Times (s)"), 9);        //ÉeÉLÉXÉgï`âÊ


            //ÉyÉìÅCÉuÉâÉVîpä¸
            /********************************

            égÇ¢èIÇÌÇ¡ÇΩÉyÉìÇ∆ÉuÉâÉVÇÕîjä¸Ç∑ÇÈïKóvÇ™Ç†ÇËÇ‹Ç∑ÅD

            ********************************/
        SelectObject(hdc, hOldBrush);
        DeleteObject(hBrush);
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);

        //ÉfÉoÉCÉXÉRÉìÉeÉLÉXÉgÇÃÉnÉìÉhÉãîjä¸
        EndPaint(hWnd, &ps);
        break;
    }


    return TRUE;
}



//-----------------------------------------------------------------------------
//éqÉEÉBÉìÉhÉEèâä˙âªÅïê∂ê¨
//éwíËÇµÇΩÉEÉBÉìÉhÉEÉnÉìÉhÉãÇÃóÃàÊÇ…éqÉEÉBÉìÉhÉEÇê∂ê¨Ç∑ÇÈÅD
//----------------------------------------------------------
// hInst    : ê∂ê¨ópÉCÉìÉXÉ^ÉìÉXÉnÉìÉhÉã
// hPaWnd    : êeÉEÉBÉìÉhÉEÇÃÉnÉìÉhÉã
// chID        : éqÉEÉBÉìÉhÉEÇÃID
// cWinName    : éqÉEÉBÉìÉhÉEñº
// PaintArea: éqÉEÉBÉìÉhÉEÇê∂ê¨Ç∑ÇÈóÃàÊÇÃÉfÉoÉCÉXÉnÉìÉhÉã
// WndProc    : ÉEÉBÉìÉhÉEÉvÉçÉVÅ[ÉWÉÉ
// *hWnd    : éqÉEÉBÉìÉhÉEÇÃÉnÉìÉhÉãÅiÉ|ÉCÉìÉ^Åj
// ñﬂÇËíl    : ê¨å˜éû=true
//-----------------------------------------------------------------------------

BOOL WinInitialize(HINSTANCE hInst, HWND hPaWnd, HMENU chID, const char* cWinName, HWND PaintArea, WNDPROC WndProc, HWND* hWnd)
{
    WNDCLASS wc;            //ÉEÉBÉìÉhÉEÉNÉâÉX
    WINDOWPLACEMENT    wplace;    //éqÉEÉBÉìÉhÉEê∂ê¨óÃàÊåvéZópÅiâÊñ è„ÇÃÉEÉBÉìÉhÉEÇÃîzíuèÓïÒÇäiî[Ç∑ÇÈç\ë¢ëÃÅj
    RECT WinRect;            //éqÉEÉBÉìÉhÉEê∂ê¨óÃàÊ
    ATOM atom;                //ÉAÉgÉÄ

    //ÉEÉBÉìÉhÉEÉNÉâÉXèâä˙âª
    wc.style = CS_HREDRAW ^ WS_MAXIMIZEBOX | CS_VREDRAW;    //ÉEÉCÉìÉhÉEÉXÉ^ÉCÉã
    wc.lpfnWndProc = WndProc;                                    //ÉEÉCÉìÉhÉEÇÃÉÅÉbÉZÅ[ÉWÇèàóùÇ∑ÇÈÉRÅ[ÉãÉoÉbÉNä÷êîÇ÷ÇÃÉ|ÉCÉìÉ^
    wc.cbClsExtra = 0;                                            //
    wc.cbWndExtra = 0;
    wc.hCursor = NULL;                                        //ÉvÉçÉOÉâÉÄÇÃÉnÉìÉhÉã
    wc.hIcon = NULL;                                        //ÉAÉCÉRÉìÇÃÉnÉìÉhÉã
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);        //ÉEÉCÉìÉhÉEîwåiêF
    wc.hInstance = hInst;                                        //ÉEÉCÉìÉhÉEÉvÉçÉVÅ[ÉWÉÉÇ™Ç†ÇÈÉCÉìÉXÉ^ÉìÉXÉnÉìÉhÉã
    wc.lpszMenuName = NULL;                                        //ÉÅÉjÉÖÅ[ñº
    wc.lpszClassName = (LPCTSTR)cWinName;                                    //ÉEÉCÉìÉhÉEÉNÉâÉXñº

    if (!(atom = RegisterClass(&wc))) {
        MessageBox(hPaWnd, TEXT("ÉEÉBÉìÉhÉEÉNÉâÉXÇÃê∂ê¨Ç…é∏îsÇµÇ‹ÇµÇΩÅD"), NULL, MB_OK | MB_ICONERROR);
        return false;
    }

    GetWindowPlacement(PaintArea, &wplace);    //ï`âÊóÃàÊÉnÉìÉhÉãÇÃèÓïÒÇéÊìæ(ÉEÉBÉìÉhÉEÇÃï\é¶èÛë‘ÇéÊìæ)
    WinRect = wplace.rcNormalPosition;        //ï`âÊóÃàÊÇÃê›íË

    //ÉEÉBÉìÉhÉEê∂ê¨
    *hWnd = CreateWindow(
        (LPCTSTR)atom,
        (LPCTSTR)cWinName,
        WS_CHILD | WS_VISIBLE,//| WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME |WS_VISIBLE,
        WinRect.left, WinRect.top,
        WinRect.right - WinRect.left, WinRect.bottom - WinRect.top,
        hPaWnd, chID, hInst, NULL
    );

    if (*hWnd == NULL) {
        MessageBox(hPaWnd, TEXT("ÉEÉBÉìÉhÉEÇÃê∂ê¨Ç…é∏îsÇµÇ‹ÇµÇΩÅD"), NULL, MB_OK | MB_ICONERROR);
        return false;
    }

    return true;
}

