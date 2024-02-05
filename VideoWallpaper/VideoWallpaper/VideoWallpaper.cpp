#include <windows.h>
#include <stdio.h>
#include <iostream>
#include "Function.h"

/*
	这是在Win10中实现动态壁纸的简单demo，旨在用简洁的代码演示其原理
	本例子有一定门槛，不可以直接运行，其中有些代码要根据实际情况修改
	一定要正确安装开源软件ffplay

	★代码不保留版权，使用时请保留如下信息：
	作者：偶尔有点小迷糊 (https://space.bilibili.com/39665558)
	粉丝：23万

	讲解视频：https://www.bilibili.com/video/BV1HZ4y1978a
*/

HWND Code;
wchar_t Null[1024];

BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM Lparam)
{
	HWND hDefView = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", 0);
	if (hDefView != 0) {
		HWND hWorkerw = FindWindowEx(0, hwnd, L"WorkerW", 0);
		ShowWindow(hWorkerw, SW_HIDE);
		if (Code) {
			SetParent(Code, hWorkerw);
		
	    }
	return FALSE;
    }
return TRUE;
}

int main(int argc, char *argv[])
{
	Code = GetForegroundWindow();
	GetCurrentDirectory(GetCurrentDirectory(0, 0), Null);
	wcscat_s(Null, 256, L"\\Resource\\VideoPlayer.exe");
	LPCWSTR Command = L" -cp VideoWallPaper.mp4 -fw -t VideoWallPaper.mp4 -l 0";
	STARTUPINFO si{ 0 };
	PROCESS_INFORMATION pi{ 0 };
	if (CreateProcess((LPCWSTR)Null, (LPWSTR)Command, 0, 0, 0, 0, 0, 0, &si, &pi))
	{
		while (!(FindWindow(L"Main HighGUI class", L"VideoWallPaper.mp4"))) {}
		// 等待视频播放器启动完成。可用循环获取窗口尺寸来代替Sleep()
		Sleep(100);
		HWND hProgman =FindWindow(L"Progman", 0);				// 找到PM窗口
		SendMessageTimeout(hProgman, 0x52C, 0, 0, 0, 100, 0);	// 给它发特殊消息
		HWND hFfplay = FindWindow(L"Main HighGUI class", L"VideoWallPaper.mp4");				// 找到视频窗口
		SetParent(hFfplay, hProgman);							// 将视频窗口设置为PM的子窗口
		EnumWindows(EnumWindowsProc, 0);						// 找到第二个WorkerW窗口并隐藏它
	}
	
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return 0;
}