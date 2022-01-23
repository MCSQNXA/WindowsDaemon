#include <iostream>

#include "Daemon.h"


int main()
{
    Daemon daemon;

    std::cout << daemon.isProcessing("D:\\WindowsDaemon\\x64\\Debug\\Demo.exe") << std::endl;
}
