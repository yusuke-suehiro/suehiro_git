//íËêîêÈåæ
#define DEF_APP_NAME    TEXT("Waveform Test")
#define DEF_MUTEX_NAME    DEF_APP_NAME            //É~ÉÖÅ[ÉeÉbÉNÉXñº
#define DEF_DATAPERS 1000    //1ïbä‘Ç…âΩÉfÅ[É^ì¸èoóÕÇ∑ÇÈÇ©

//ä÷êîêÈåæ
BOOL CALLBACK MainDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );    //ÉÅÉCÉìÉ_ÉCÉAÉçÉOÉvÉçÉVÅ[ÉWÉÉ
//BOOL WinInitialize( HINSTANCE hInst, HWND hPaWnd, HMENU chID, char *cWinName, HWND PaintArea, WNDPROC WndProc ,HWND *hDC);//éqÉEÉBÉìÉhÉEÇê∂ê¨
UINT WINAPI TFunc( LPVOID thParam );                                                //ÉfÅ[É^ì«Ç›çûÇ›ópÉXÉåÉbÉh
