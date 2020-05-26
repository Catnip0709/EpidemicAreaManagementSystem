#include "../interface.h"
#include "../myDB.h"
#include "../common.h"
#include "../genJson.h"
using namespace std;

string physicalCondition(string userID, string date, string todayTemperature, string HuBeiContact) {
    MyDB db;

    // 先判断该ID是否已经注册过
    string sql = "SELECT 1 FROM User WHERE userID = \"" + userID + "\" LIMIT 1;";
	if(!db.exeSQL(sql, RETRIEVE)) {
        return CGenJson::genResultJson(MYSQL_ERR);
    }
    if (!mysql_num_rows(db.result)) { // 该ID尚未注册
        return CGenJson::genResultJson(HAVENT_REGISTER);
    }

    sql = "INSERT INTO `PhysicalCondition` VALUE (\"" + userID + "\", \"" + date + "\", \"" + todayTemperature + "\", \"" + HuBeiContact + "\");";
    if(!db.exeSQL(sql, CREATE)) { // 注册信息插入mysql失败
        return CGenJson::genResultJson(MYSQL_ERR);
    }

    return CGenJson::genResultJson(SUCCESS);
}

string epidemicSituation(string date) {
    CGenJson jsonResult;
    Document::AllocatorType& allocator = jsonResult.jsonDoc.GetAllocator();
    MyDB db;

    // 1、获取当日填报了健康状况的人数和未填报的人数
    string sql = "SELECT COUNT(*) FROM PhysicalCondition WHERE date = " + date + ";";
    if(!db.exeSQL(sql, RETRIEVE)) {
        return CGenJson::genResultJson(MYSQL_ERR);
    }
    string completed = db.sqlResult[0][0];
    sql = "SELECT COUNT(*) FROM User;";
    if(!db.exeSQL(sql, RETRIEVE)) {
        return CGenJson::genResultJson(MYSQL_ERR);
    }
    string uncompleted = to_string(atoi(db.sqlResult[0][0].c_str()) - atoi(completed.c_str()));

    Value todayList;
    vector<string> todayListKey = {"completed", "uncompleted"};
    vector<string> todayListValue = {completed, uncompleted};
    jsonResult.genInsideJson(todayList, todayListKey, todayListValue, allocator);
    jsonResult.jsonDoc.AddMember("todayList", todayList, allocator);

    // 2、获取当前总的（2=隔离、3=疑似、4=确诊、5=死亡）人数
    Value total;
    vector<string> totalKey = {"segregation", "suspected", "diagnosis", "die"};
    vector<string> totalValue;    
    for (int i = 2; i <= 5; ++i) {
        sql = "SELECT COUNT(*) FROM User WHERE state = " + to_string(i) + ";";
        if(!db.exeSQL(sql, RETRIEVE)) {
            return CGenJson::genResultJson(MYSQL_ERR);
        }
        totalValue.push_back(db.sqlResult[0][0]);
    }
    jsonResult.genInsideJson(total, totalKey, totalValue, allocator);
    jsonResult.jsonDoc.AddMember("total", total, allocator);

    // 3、获取今日新增的（2=隔离、3=疑似、4=确诊、5=死亡）人数 = 今日当前-昨天总

}