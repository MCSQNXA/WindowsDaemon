#pragma once

#include <string>


class Daemon
{
public://启动或停止守护进程
	static void startProcess();
	static void stopProcess();

public://守护进程正在运行
	static bool isProcessing(std::string file);


};
