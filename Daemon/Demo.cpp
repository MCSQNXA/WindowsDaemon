#include "Demo.h"

#include <windows.h>
#include <io.h>
#include <psapi.h>
#include <comdef.h>
#include <tlhelp32.h>

#include <string>

#pragma comment(linker,"/subsystem:windows /entry:mainCRTStartup")//不显示控制台(后台运行)


int Demo::getProcessCount(std::string file)
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

std::string Demo::paths(std::string subpath)
{
	static std::string parent;

	if (parent.size() == 0) {
		char exe[MAX_PATH];
		unsigned long len = GetModuleFileName(NULL, exe, MAX_PATH);

		while (len-- > 0) {
			if (exe[len] == '\\') {
				parent.append(exe, len + 1); break;
			}
		}
	}

	return std::string(parent).append(subpath);
}

bool Demo::startExe(std::string file)
{
	std::string path;//file文件父路径

	for (size_t i = file.size() - 1; i > 0; i--) {//分割file
		if (file[i] == '/' || file[i] == '\\') {
			path.append(file.substr(0, i)); break;
		}
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	if (CreateProcess(
		NULL,//指向一个NULL结尾的、用来指定可执行模块的宽字节字符串
		(char*)file.c_str(),//命令行字符串
		NULL,//指向一个SECURITY_ATTRIBUTES结构体，这个结构体决定是否返回的句柄可以被子进程继承。
		NULL,//如果lpProcessAttributes参数为空（NULL），那么句柄不能被继承。<同上>
		false,//指示新进程是否从调用进程处继承了句柄。
		CREATE_NEW_CONSOLE,//指定附加的、用来控制优先类和进程的创建的标
			//CREATE_NEW_CONSOLE新控制台打开子进程
			//CREATE_SUSPENDED子进程创建后挂起，直到调用ResumeThread函数
		NULL,//指向一个新进程的环境块。如果此参数为空，新进程使用调用进程的环境
		path.c_str(),//指定子进程的工作路径
		&si,//决定新进程的主窗体如何显示的STARTUPINFO结构体
		&pi//接收新进程的识别信息的PROCESS_INFORMATION结构体
	)) {
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread); return true;
	}

	return false;
}


int main() {
	char exe[MAX_PATH];
	GetModuleFileName(NULL, exe, MAX_PATH);

	if (Demo::getProcessCount(exe) > 1) {//相同目录只能运行一个
		return -1;
	}

	std::string target = Demo::paths("Demo.exe");//守护目标exe进程路径

	if (_access(target.c_str(), 0) != 0) {
		return -2;
	}

	while (true) {
		if (Demo::getProcessCount(target) < 1) {//目标exe死亡，唤醒
			Demo::startExe(target);
		}

		Sleep(3000);//3秒一个监听周期
	}

}
