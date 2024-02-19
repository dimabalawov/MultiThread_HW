#pragma once
#include "header.h"

class CMultithreadDlg
{
public:
	CMultithreadDlg(void);
public:
	~CMultithreadDlg(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static CMultithreadDlg* ptr;
	void Cls_OnClose(HWND hwnd);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	HWND hList1, hList2, hList3, hStart1, hStart2, hStart3, hTerminate1, hTerminate2, hTerminate3;
	HANDLE Th1, Th2, Th3;
};
