#include "../interface.h"
#include "../myDB.h"
#include "../common.h"
using namespace std;

string physicalCondition(string userID, string date, string todayTemperature, string HuBeiContact) {
    MyDB db;

    // 先判断该ID是否已经注册过
    string sql = "SELECT 1 FROM User WHERE userID = \"" + userID + "\" LIMIT 1;";
	if(!db.exeSQL(sql, RETRIEVE)) {
        return genResultJson(MYSQL_ERR);
    }
    if (!mysql_num_rows(db.result)) { // 该ID尚未注册
        return genResultJson(HAVENT_REGISTER);
    }

    sql = "INSERT INTO `PhysicalCondition` VALUE (\"" + userID + "\", \"" + date + "\", \"" + todayTemperature + "\", \"" + HuBeiContact + "\");";
    if(!db.exeSQL(sql, CREATE)) { // 注册信息插入mysql失败
        return genResultJson(MYSQL_ERR);
    }

    return genResultJson(SUCCESS);
}

string epidemicSituation(string date) {
    string result;
    MyDB db;

    // 1、获取当日填报了健康状况的人数和未填报的人数
    string sql = "SELECT COUNT(*) FROM PhysicalCondition WHERE date = " + date + ";";
    if(!db.exeSQL(sql, RETRIEVE)) {
        return genResultJson(MYSQL_ERR);
    }
}