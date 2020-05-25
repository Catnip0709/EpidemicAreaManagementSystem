#include <mysql/mysql.h>
#include "interface.h"
#include "common.h"
using namespace std;
 
class MyDB
{
public:
	MyDB();
	~MyDB();
	bool initDB();
	bool exeSQL(string sql, int op);

	MYSQL *connection;
	MYSQL_RES *result;
	MYSQL_ROW row;
	vector<vector<string>> sqlResult;

private:
	string myHost  = "localhost";
	string myUser  = "root";
	string myPWD   = "0709";
	string myTable = "EpidemicDatabase";
};
