#pragma once

#include <string>


class Daemon
{
public://������ֹͣ�ػ�����
	static void startProcess();
	static void stopProcess();

public://�ػ�������������
	static bool isProcessing(std::string file);


};
