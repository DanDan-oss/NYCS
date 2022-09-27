#include <iostream>
#include "ServerNetwork.h"

int main()
{
    printf("%s 向你问好!\n", "ServerControl");

    ZinxKernel::ZinxKernelInit();
    ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(7788, new CServerTcpFact())));
	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();

    return 0;
}