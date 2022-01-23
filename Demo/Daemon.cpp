#include "Daemon.h"

#include <windows.h>
#include <psapi.h>
#include <comdef.h>
#include <tlhelp32.h>

#include <string>
#include <thread>
#include <iostream>


void Daemon::startProcess()
{
	std::thread([&] {

		}).detach();
}

void Daemon::stopProcess()
{

}

bool Daemon::isProcessing(std::string file)
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
		return false;
	}

	PROCESSENTRY32 info = { 0 };
	info.dwSize = sizeof(PROCESSENTRY32W);

	if (!Process32First(handle, &info)) {
		return false;
	}

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
			return true;
		}

		CloseHandle(process);

	} while (Process32Next(handle, &info));

	CloseHandle(handle); return false;
}
