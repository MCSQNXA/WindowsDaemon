#include "Daemon.h"

#include <windows.h>
#include <psapi.h>
#include <comdef.h>
#include <tlhelp32.h>

#include <string>
#include <thread>
#include <iostream>


bool Daemon::running;


void Daemon::startProcess()
{
	if (Daemon::running) {
		return;
	}

	Daemon::running = true;

	std::thread([&] {
		while (Daemon::running) {

		}
		}).detach();
}

void Daemon::stopProcess()
{
	Daemon::running = false;
}

int Daemon::getProcessCount(std::string file)
{
	std::string path, name;// ·��/�ļ�.exe

	for (size_t i = file.size() - 1; i > 0; i--) {//�ָ�file
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
		return -2;
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
		GetModuleFileNameExA(process, NULL, exe, sizeof(exe));//ͨ��pid��ȡ·��

		if (file.compare(exe) == 0) {//����������
			count++;
		}

		CloseHandle(process);

	} while (Process32Next(handle, &info));

	CloseHandle(handle); return count;
}

std::string Daemon::paths(std::string subpath)
{
	static std::string parent;

	if (parent.size() == 0) {
		char exe[MAX_PATH];
		unsigned long len = GetModuleFileName(NULL, exe, MAX_PATH);

		while (len-- > 0) {
			if (exe[len] == '\\') {
				parent.append(exe, len); break;
			}
		}
	}

	return std::string(parent).append(subpath);
}

bool Daemon::startExe(std::string file)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	if (CreateProcess(
		NULL,//ָ��һ��NULL��β�ġ�����ָ����ִ��ģ��Ŀ��ֽ��ַ���
		(char*)file.c_str(),//�������ַ���
		NULL,//ָ��һ��SECURITY_ATTRIBUTES�ṹ�壬����ṹ������Ƿ񷵻صľ�����Ա��ӽ��̼̳С�
		NULL,//���lpProcessAttributes����Ϊ�գ�NULL������ô������ܱ��̳С�<ͬ��>
		false,//ָʾ�½����Ƿ�ӵ��ý��̴��̳��˾����
		CREATE_NEW_CONSOLE,//ָ�����ӵġ���������������ͽ��̵Ĵ����ı�
			//CREATE_NEW_CONSOLE�¿���̨���ӽ���
			//CREATE_SUSPENDED�ӽ��̴��������ֱ������ResumeThread����
		NULL,//ָ��һ���½��̵Ļ����顣����˲���Ϊ�գ��½���ʹ�õ��ý��̵Ļ���
		NULL,//ָ���ӽ��̵Ĺ���·��
		&si,//�����½��̵������������ʾ��STARTUPINFO�ṹ��
		&pi//�����½��̵�ʶ����Ϣ��PROCESS_INFORMATION�ṹ��
	)) {
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread); return true;
	}

	return false;
}

bool Daemon::stopExe(std::string file)
{
	std::string path, name;// ·��/�ļ�.exe

	for (size_t i = file.size() - 1; i > 0; i--) {//�ָ�file
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
		GetModuleFileNameExA(process, NULL, exe, sizeof(exe));//ͨ��pid��ȡ·��

		if (file.compare(exe) == 0) {//����������
			HANDLE kill = OpenProcess(PROCESS_ALL_ACCESS, FALSE, info.th32ProcessID);

			TerminateProcess(kill, 0);//�رս���

			CloseHandle(kill);
			CloseHandle(process); return true;
		}

		CloseHandle(process);

	} while (Process32Next(handle, &info));

	CloseHandle(handle); return false;
}
