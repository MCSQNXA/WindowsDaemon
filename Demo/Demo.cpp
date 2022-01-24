#include <iostream>

#include "Daemon.h"

#include <windows.h>

Daemon daemon;


bool ctrlhandler(DWORD type)
{
	daemon.stopProcess();//停止守护进程

	return false;
}


int main()
{
	daemon.startProcess();//启动守护进程

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlhandler, true);//监听控制台右上角关闭事件


	for (int i = 0; i < 10; i++) {//模拟被杀死
		std::cout << "Running" << "=" << i << std::endl; Sleep(500);
	}



}
