#include "Demo.h"

#include <windows.h>
#include <psapi.h>
#include <comdef.h>
#include <tlhelp32.h>

#include <string>
#include <thread>
#include <iostream>


int Demo::isProcessing(std::string file)
{
	std::string path, name;//   路径/文件.exe

	for (size_t i = file.size() - 1; i > 0; i--) {//分割file
		if (file[i] == '/' || file[i] == '\\') {
			path.append(file.substr(0, i));
			name.append(file.substr(i + 1, file.size() - i - 1)); break;
		}
	}

	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (handle == INVALID_HANDLE_VALUE) {
		return -1;
	}

	PROCESSENTRY32 info = { 0 };
	info.dwSize = sizeof(PROCESSENTRY32W);

	if (!Process32First(handle, &info)) {
		return -1;
	}

	int count = 0;

	do {
		if (name.compare(_bstr_t(info.szExeFile)) != 0) {//info.szExeFile
			continue;
		}

		HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, info.th32ProcessID);

		if (process == NULL) {
			continue;
		}

		char exe[MAX_PATH];
		GetModuleFileNameExA(process, NULL, exe, sizeof(exe));//通过pid获取路径

		if (file.compare(exe) == 0) {//进程运行中
			count++;
		}

		CloseHandle(process);

	} while (Process32Next(handle, &info));

	CloseHandle(handle); return count;
}

int main() {
	char exe[MAX_PATH];
	GetModuleFileName(NULL, exe, MAX_PATH);

	if (Demo::isProcessing(exe)>1) {//相同目录只能运行一个
		return 0;
	}

	while (true)
	{
		std::cout << 1 << std::endl; Sleep(300);
	}






}
