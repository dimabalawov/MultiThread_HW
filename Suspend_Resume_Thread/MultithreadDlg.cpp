#include "MultithreadDlg.h"
#include <string>

using namespace std;

CMultithreadDlg* CMultithreadDlg::ptr = NULL;

CMultithreadDlg::CMultithreadDlg(void)
{
	ptr = this;
}

CMultithreadDlg::~CMultithreadDlg(void)
{

}

void CMultithreadDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}


DWORD WINAPI ThreadPrime(LPVOID lp)
{
	SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);
	HWND hList = (HWND)lp;
	int i = 1;
	while(i<=20000)
	{
		bool Flag = 1;
		for (size_t j = 2; j <= i / 2; j++)
		{
			if (i % j == 0)
			{
				Flag = 0;
			}
		}
		if (Flag)
		{
			SendMessage(hList, LB_ADDSTRING, 0, LPARAM(to_wstring(i).c_str()));
		}
		Sleep(50);
		i++;
	}
	return 0;
}
DWORD WINAPI ThreadEven(LPVOID lp)
{
	SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
	HWND hList = (HWND)lp;
	int i = 2;
	while (i <= 20000)
	{
		bool Flag = 0;
		if (i % 2 == 0)
		{
			Flag = 1;
		}
		if (Flag)
		{
			SendMessage(hList, LB_ADDSTRING, 0, LPARAM(to_wstring(i).c_str()));
		}
		Sleep(50);
		i++;
	}
	return 0;
}
DWORD WINAPI ThreadNums(LPVOID lp)
{
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
	HWND hList = (HWND)lp;
	int i = 0;
	while (i <= 20000)
	{
		SendMessage(hList, LB_ADDSTRING, 0, LPARAM(to_wstring(i).c_str()));
		Sleep(50);
		i++;
	}
	return 0;
}

BOOL CMultithreadDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	hList1 = GetDlgItem(hwnd, IDC_LIST1);
	hList2 = GetDlgItem(hwnd, IDC_LIST2);
	hList3 = GetDlgItem(hwnd, IDC_LIST3);
	hStart1 = GetDlgItem(hwnd, IDC_START1);
	hStart2 = GetDlgItem(hwnd, IDC_START2);
	hStart3 = GetDlgItem(hwnd, IDC_START3);
	srand(time(0));
	Th1 = CreateThread(NULL, 0, ThreadPrime, hList1, CREATE_SUSPENDED, NULL);
	Th2 = CreateThread(NULL, 0, ThreadEven, hList2, CREATE_SUSPENDED, NULL);
	Th3 = CreateThread(NULL, 0, ThreadNums, hList3, CREATE_SUSPENDED, NULL);
	return TRUE;
}

void CMultithreadDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if(id == IDC_START1)
	{
		static BOOL flag = FALSE;
		if(flag)
		{
			SuspendThread(Th1);
			SetWindowText(hStart1, TEXT("Старт"));
		}
		else
		{
			ResumeThread(Th1);
			SetWindowText(hStart1, TEXT("Пауза"));
		}
		flag = !flag;
	}
	else if(id == IDC_START2)
	{
		static BOOL flag = FALSE;
		if(flag)
		{
			SuspendThread(Th2);
			SetWindowText(hStart2, TEXT("Старт"));
		}
		else
		{
			ResumeThread(Th2);
			SetWindowText(hStart2, TEXT("Пауза"));
		}
		flag = !flag;
	}
	else if(id == IDC_START3)
	{
		static BOOL flag = FALSE;
		if(flag)
		{
			SuspendThread(Th3);
			SetWindowText(hStart3, TEXT("Старт")); 
		}
		else
		{
			ResumeThread(Th3);
			SetWindowText(hStart3, TEXT("Пауза"));
		}
		flag = !flag;
	}
	else if(id == IDC_TERMINATE1)
	{
		TerminateThread(Th1, 0);
		CloseHandle(Th1);
		EnableWindow(hStart1, FALSE);
	}
	else if(id == IDC_TERMINATE2)
	{
		TerminateThread(Th2, 0);
		CloseHandle(Th2);
		EnableWindow(hStart2, FALSE);
	}
	else if(id == IDC_TERMINATE3)
	{
		TerminateThread(Th3, 0);
		CloseHandle(Th3);
		EnableWindow(hStart3, FALSE);
	}
}

BOOL CALLBACK CMultithreadDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}