#pragma once

#include <string>


class Daemon
{
private://�ػ�������������
	static bool running;

private://�ػ�����exe·��
	static std::string daemon;

public://������ֹͣ�ػ�����
	static bool startProcess();
	static void stopProcess();

public://ͨ��exe�ļ���ȡ�������е�exe��������
	static int getProcessCount(std::string file);

public:
	static std::string paths(std::string subpath);

public://����exe
	static bool startExe(std::string file);

public://ֹͣexe
	static bool stopExe(std::string file);

};
