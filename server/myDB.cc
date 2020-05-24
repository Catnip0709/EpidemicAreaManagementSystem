#include <iostream>
#include <string>
#include <vector>
#include "myDB.h"
using namespace std;

MyDB::MyDB() {
	connection = mysql_init(NULL); // 初始化数据库连接变量
	if(connection == NULL) {
		cout << "MYSQL初始化失败！Error:" << mysql_error(connection);
		exit(1);
	}
}
 
MyDB::~MyDB() {
	if(connection != NULL) { // 关闭数据库连接
		mysql_close(connection);
	}
}

bool MyDB::initDB(string host, string user, string pwd, string db_name) {
	// 函数mysql_real_connect建立一个数据库连接
	// 成功返回MYSQL*连接句柄，失败返回NULL
	connection = mysql_real_connect(connection, host.c_str(), user.c_str(), pwd.c_str(), db_name.c_str(), 0, NULL, 0);
	if(connection == NULL) {
		cout << "MYSQL初始化失败！Error:" << mysql_error(connection);
		exit(1);
	}
	return true;
}
 
bool MyDB::exeSQL(string sql, int op) {
	sqlResult.clear();
	
	if(mysql_query(connection, sql.c_str())) { // mysql_query()执行成功返回0，失败返回非0值
		cout << "Query Error:" << mysql_error(connection);
		return false;
	}
	else {
		if (op == RETRIEVE) {
			cout << "——————MYSQL结果——————" << endl;
			result = mysql_use_result(connection); // 获取结果集
			while(row = mysql_fetch_row(result)) { // 不断获取下一行
				vector<string> rowResult;
				for(int j = 0; j < mysql_num_fields(result); ++j) { // mysql_num_fields()返回列数
					rowResult.push_back(row[j]);
					cout << row[j] << " ";
				}
				sqlResult.push_back(rowResult);
				cout << endl;
			}
			cout << "——————————————————" << endl;
		}
	}
	
	return true;
}