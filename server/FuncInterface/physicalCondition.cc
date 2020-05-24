#include "../interface.h"
#include "../myDB.h"
#include "../common.h"
using namespace std;

string physicalCondition(string userID, string date, string todayTemperature, string HuBeiContact) {
    string result = "{\"result\":";
    MyDB db;
	db.initDB(db.myHost, db.myUser, db.myPWD, db.myTable);

    // 先判断该ID是否已经注册过
    string sql = "SELECT 1 FROM User WHERE userID = \"" + userID + "\" LIMIT 1;";
	if(!db.exeSQL(sql, RETRIEVE)) {
        result = result + to_string(MYSQL_ERR) + "}";
        return result;
    }
    if (!mysql_num_rows(db.result)) { // 该ID尚未注册
        result = result + to_string(HAVENT_REGISTER) + "}";
        return result;
    }

    sql = "INSERT INTO `PhysicalCondition` VALUE (\"" + userID + "\", \"" + date + "\", \"" + todayTemperature + "\", \"" + HuBeiContact + "\");";
    if(!db.exeSQL(sql, CREATE)) { // 注册信息插入mysql失败
        result = result + to_string(MYSQL_ERR) + "}";
        return result;
    }

    result = result + to_string(SUCCESS) + "}";
    return result;
}