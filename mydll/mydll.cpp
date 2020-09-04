// mydll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <iostream>
#include "mydll.h"
//#include "pcquicklib.h"
__declspec(thread) int s_threadid = 1;
int threadIdPionter =  TlsAlloc();
CREATEDLL_API int gettid()  
{
	
	int threadId = (int)TlsGetValue(threadIdPionter);
	if (0 == threadId)
	{
		threadId = GetCurrentThreadId();
		TlsSetValue(threadIdPionter, (LPVOID)threadId);
	} 
	//else
	{
		return threadId;
	}
}
