#include <iostream>

#include "Daemon.h"

#include <windows.h>


int main()
{
	Daemon daemon;
	daemon.startProcess();

	std::cout << daemon.startExe("D:\\WindowsDaemon\\Debug\\Daemon.exe") << std::endl;

	Sleep(3000);

	std::cout << daemon.stopExe("D:\\WindowsDaemon\\Debug\\Daemon.exe") << std::endl;;





	/*while (true)
	{
		std::cout << "Running" << std::endl; Sleep(1000);
	}*/




}
