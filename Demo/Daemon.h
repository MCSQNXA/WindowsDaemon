#pragma once

#include <string>


class Daemon
{
private://守护进程正在运行
	static bool running;

private://守护进程exe路径
	static std::string daemon;

public://启动或停止守护进程
	static bool startProcess();
	static void stopProcess();

public://通过exe文件获取正在运行的exe进程数量
	static int getProcessCount(std::string file);

public:
	static std::string paths(std::string subpath);

public://启动exe
	static bool startExe(std::string file);

public://停止exe
	static bool stopExe(std::string file);

};
