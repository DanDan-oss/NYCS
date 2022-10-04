#ifndef SERVER_SQL_H
#define SERVER_SQL_H

#include <mysql/mysql.h>
#include <iostream>

//class CDataBase
//{
//	virtual ~CDataBase();
//	virtual bool InitDataBase(const char* _dbHost, const char* _dbUser, const char* _dbpass, const char* _dbData, unsigned int _dbPort) = 0;
//
//};



class CDBMysql
{
public:

	CDBMysql(std::string _dbHost="localhost", std::string _dbUser = "user", std::string _dbpass = "pass",
		std::string _dbData = "mysql", unsigned int _dbPort = 3306);
	virtual ~CDBMysql();
	

	void MysqlErrorPrint(int _error_no);
	MYSQL* GetMysqlPoint();

private:
	virtual bool InitDBMysql(const char* _dbHost, const char* _dbUser, const char* _dbpass, const char* _dbData, unsigned int _dbPort);
	virtual bool FiniDBMysql();


private:
	MYSQL* m_mysql;
};


#endif // !SERVER_SQL_H

