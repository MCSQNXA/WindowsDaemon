#pragma once

#include <string>


class Demo
{
public://通过exe文件获取正在运行的exe进程数量
	static int getProcessCount(std::string file);

public:
	static std::string paths(std::string subpath);


};

