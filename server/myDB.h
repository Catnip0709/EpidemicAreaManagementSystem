#include <mysql/mysql.h>
#include "common.h"
using namespace std;
 
class MyDB
{
public:
	MyDB();
	~MyDB();
	bool initDB(string host, string user, string pwd, string db_name);
	bool exeSQL(string sql, int op);

	MYSQL *connection;
	MYSQL_RES *result;
	MYSQL_ROW row;
	vector<vector<string>> sqlResult;

	string myHost  = "localhost";
	string myUser  = "root";
	string myPWD   = "0709";
	string myTable = "EpidemicDatabase";
};
