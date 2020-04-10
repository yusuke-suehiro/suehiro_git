//-------------------------------------------------------------------------
//    main.cppÉtÉ@ÉCÉãÇÃÉwÉbÉ_Å[ÉtÉ@ÉCÉã
//
//    ä÷êîÅCïœêîêÈåæóp
//-------------------------------------------------------------------------

//ä÷êîêÈåæ
BOOL CALLBACK MainDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );    //ÉÅÉCÉìÉ_ÉCÉAÉçÉOÉvÉçÉVÅ[ÉWÉÉ
BOOL WinInitialize( HINSTANCE hInst, HWND hPaWnd, HMENU chID, const char *cWinName, HWND PaintArea, WNDPROC WndProc ,HWND *hDC);//éqÉEÉBÉìÉhÉEÇê∂ê¨

HRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );        //éqÉEÉBÉìÉhÉEÉvÉçÉVÅ[ÉWÉÉ

//ïœêîêÈåæ
//ïœêîêÈåæ
//ÉâÉWÉIÉ{É^ÉìÇÃÉnÉìÉhÉã
static HWND hRadioR;        //ÉEÉBÉìÉhÉEÉnÉìÉhÉãÅiÉâÉWÉIÉ{É^ÉìÅj
static HWND hRadioG;        //ÉEÉBÉìÉhÉEÉnÉìÉhÉãÅiÉâÉWÉIÉ{É^ÉìÅj
static HWND hRadioBLACK;        //ÉEÉBÉìÉhÉEÉnÉìÉhÉãÅiÉâÉWÉIÉ{É^ÉìÅj

static COLORREF    color, colorPen;    //êF
