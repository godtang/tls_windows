// testDll.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <afxwin.h>
#include <windows.h>
#include <stdlib.h>
#include <iostream>


typedef int(*FUN_ADD)();

DWORD WINAPI MyThreadProc1(LPVOID lpParameter);
int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE handle1;
	handle1 = CreateThread(NULL, 0, MyThreadProc1, NULL, 0, NULL);
	// 	HANDLE handle2;
	// 	handle2 = CreateThread(NULL,0,MyThreadProc1,NULL,0,NULL);

	const char* dllName = "mydll.dll";
	const char* funName1 = "gettid";

	HMODULE hDLL = LoadLibrary(dllName);

	if (hDLL != NULL)
	{
		FUN_ADD fp1 = FUN_ADD(GetProcAddress(hDLL, funName1));

		if (fp1 != NULL)
		{
			int i = fp1();
			int i2 = fp1();
			std::cout << i << std::endl;
		}
		else
		{
			std::cout << "Cannot Find Function " << funName1 << std::endl;
		}
	}
	else
	{
		std::cout << "Cannot Find " << dllName << std::endl;
	}
	system("pause");
	return 0;
}

DWORD WINAPI MyThreadProc1(LPVOID lpParameter)
{
	std::cout << "The MyThreadProc1 is Running !" << std::endl;

	const char* dllName = "mydll.dll";
	const char* funName1 = "gettid";

	HMODULE hDLL = LoadLibrary(dllName);

	if (hDLL != NULL)
	{
		FUN_ADD fp1 = FUN_ADD(GetProcAddress(hDLL, funName1));

		if (fp1 != NULL)
		{
			int i = fp1();
			int i2 = fp1();
			std::cout << i << std::endl;
		}
	}

	return 0;
}