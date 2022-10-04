#include <iostream>
#include "ServerNetwork.h"
#include "ServerSQL.h"

void testDB()
{
	int errorNo = 0;
	unsigned int dwLine = 0;
	MYSQL_RES* pRes = nullptr;
	MYSQL_FIELD* fileTitle = nullptr;
	MYSQL_ROW rowRet = nullptr;

	CDBMysql dbMysql("sh-cynosdbmysql-grp-jjzi4ace.sql.tencentcdb.com", "mohui",  "mh!12345", "mysql", 21971);

	if (errorNo = mysql_query(dbMysql.GetDBPoint(), "select * from  global_grants"))
	{
		std::cout << "MYSQL 数据库查询失败" << errorNo << std::endl;
		return;
	}
	std::cout << mysql_get_server_info(dbMysql.GetDBPoint()) << std::endl;
	pRes = mysql_store_result(dbMysql.GetDBPoint());
	dwLine = mysql_num_fields(pRes);			//  返回结果集有多少行
	std::cout << "MYSQL查询结果集中有 " << dwLine << "列数据" << std::endl; 
	fileTitle = mysql_fetch_fields(pRes);

	for (int i = 0; i < dwLine; ++i)
		std::cout << fileTitle[i].name << "    ";
	std::cout << std::endl;


	for (int iNdex = 0; rowRet = mysql_fetch_row(pRes); ++iNdex)
	{
		std::cout << iNdex << "   ";
		for (int j = 0; j < dwLine; ++j)
			std::cout << rowRet[j] << "    ";
		std::cout << std::endl;
	}

	//while (rowRet = mysql_fetch_row(pRes))
	//{
	//	std::cout << "";
	//	for (int i = 0; i < dwLine; ++i)
	//		std::cout << rowRet[i] << "    ";
	//	std::cout << std::endl;
	//}

	mysql_free_result(pRes);
	return;
}

void testSccket()
{
	ZinxKernel::ZinxKernelInit();
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(7788, new CServerTcpFact())));
	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
}


int main()
{
    printf("%s 向你问好!\n", "ServerControl");

	testSccket();
	//testDB();

    return 0;
}