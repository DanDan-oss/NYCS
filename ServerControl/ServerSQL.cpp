#include "ServerSQL.h"


// ====================================== DataBase

//CDataBase::CDataBase()
//{
//	
//}
//
//CDataBase::~CDataBase()
//{
//
//}

// ====================================== MYSQL
CDBMysql::CDBMysql(std::string _dbHost, std::string _dbUser, std::string _dbpass, std::string _dbData, unsigned int _dbPort)
{
	this->InitDBMysql(_dbHost.data(), _dbUser.data(), _dbpass.data(), _dbData.data(), _dbPort);
}

CDBMysql::~CDBMysql()
{
	this->FiniDBMysql();
}


bool CDBMysql::InitDBMysql(const char* _dbHost, const char* _dbUser, const char* _dbpass, const char* _dbData, unsigned int _dbPort)
{
	if(_dbHost==NULL || _dbUser == NULL || _dbpass == NULL || _dbData == NULL)
		return false;

	this->m_mysql = mysql_init(NULL);
	if (!this->m_mysql)
	{
		std::cout << "error: mysql_init fail! :" << mysql_error(this->m_mysql) << std::endl;
		return false;
	}
		

	if (!mysql_real_connect(this->m_mysql, _dbHost, _dbUser, _dbpass, _dbData, _dbPort, NULL, 0))
	{
		std::cout << "error: mysql_real_connect fail! :" << mysql_error(this->m_mysql) << std::endl;
		this->FiniDBMysql();
		return false;
	}
	if (!mysql_set_character_set(this->m_mysql, "utf8"))
		std::cout << "mysql_set_character_set set ok: " << mysql_character_set_name(this->m_mysql) << std::endl;
	return true;
}

bool CDBMysql::FiniDBMysql()
{
	if (this->m_mysql)
	{
		mysql_close(this->m_mysql);
		this->m_mysql = nullptr;
	}

}

MYSQL* CDBMysql::GetMysqlPoint()
{
	return this->m_mysql;
}

void  CDBMysql::MysqlErrorPrint(int _error_no)
{
	if (!this->m_mysql)
	{
		std::cout << "[sql]:mysql环境未初始化 MYSQL*==NULL" << std::endl;
		return;
	}
	switch (_error_no)
	{
	case CR_COMMANDS_OUT_OF_SYNC:
		std::cout << "[sql]:mysql语句执行错误" << std::endl;
		break;
	case CR_SERVER_GONE_ERROR:
		std::cout << "[sql]:MySQL服务器不可用" << std::endl;
		break;
	case CR_SERVER_LOST:
		std::cout << "[sql]:在查询过程中，与服务器的连接丢失" << std::endl;
		break;
	case CR_UNKNOWN_ERROR:
		std::cout << "[sql]:出现未知的mysql错误" << std::endl;
		break;
	case CR_OUT_OF_MEMORY:
		std::cout << "[sql]:mysql结果集内存溢出" << std::endl;
		break;
	default:
		std::cout << "[sql]:出现未知错误" << std::endl;
		break;
	}
	return;
}


