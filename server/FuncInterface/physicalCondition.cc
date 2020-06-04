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

string epidemicSituation() {
    CGenJson jsonResult;
    Document::AllocatorType& allocator = jsonResult.jsonDoc.GetAllocator();
    MyDB db;

    vector<string> date7 = get7date();
    // 0、添加result信息（如果出错会直接返回，故可直接添加成功信息）
    jsonResult.jsonDoc.AddMember("result", "0", allocator);

    // 1、获取当日填报了健康状况的人数和未填报的人数  todayList
    string sql = "SELECT COUNT(*) FROM PhysicalCondition WHERE date = \"" + date7[6] + "\";";
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

    // 2、获取当前总的（2=隔离、3=疑似、4=确诊、5=死亡）人数 total
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

    // 3、获取今日新增的（2=隔离、3=疑似、4=确诊、5=死亡）人数 = 今日当前-昨天总数  todayIncrease
    Value todayIncrease;
    sql = "SELECT segregation, suspected, diagnosis, die FROM SpecialStateDailyRecord WHERE date = \"" + date7[5] + "\";";
    if(!db.exeSQL(sql, RETRIEVE)) {
        return CGenJson::genResultJson(MYSQL_ERR);
    }
    if (mysql_num_rows(db.result) != 1 || mysql_num_fields(db.result) != 4) {
        return CGenJson::genResultJson(DATA_ERR);
    }
    vector<string> yesterdayTotal = db.sqlResult[0];
    vector<string> todayIncreaseValue;
    for(int i = 0; i < 4; ++i) {
        todayIncreaseValue.push_back(to_string((atoi(totalValue[i].c_str()) - atoi(yesterdayTotal[i].c_str()))));
    }
    jsonResult.genInsideJson(todayIncrease, totalKey, todayIncreaseValue, allocator);
    jsonResult.jsonDoc.AddMember("todayIncrease", todayIncrease, allocator);

    // 4、日期数组
    Value KEYdate(kArrayType);
    jsonResult.genInsideArray1(KEYdate, date7, allocator);
    jsonResult.jsonDoc.AddMember("date", KEYdate, allocator);

    // 5、趋势变化
    // 5.1 获取数据（4个数组）
    vector<vector<string>> trendDataValue;
    for (int i = 0; i < 4; ++i) {
        vector<string> trendDataValuePart;
        for (int j = 0; j < 6; ++j) { // 前6天数据
            sql = "SELECT " + totalKey[i] + " FROM SpecialStateDailyRecord WHERE date = \"" + date7[j] + "\";";
            if(!db.exeSQL(sql, RETRIEVE)) {
                return CGenJson::genResultJson(MYSQL_ERR);
            }
            trendDataValuePart.push_back(db.sqlResult[0][0]);
        }
        trendDataValuePart.push_back(totalValue[i]); // 当天数据
        trendDataValue.push_back(trendDataValuePart);
    }
    // 5.2 生成label
    Value trendLableValue[4];
    vector<string> trendLabelValueStr = {"隔离", "疑似", "确诊", "死亡"};
    for (int i = 0; i < trendLabelValueStr.size(); ++i) {
        trendLableValue[i].SetString(trendLabelValueStr[i].c_str(), allocator);
    }
    // 5.3 生成data
    Value *trendDataArray[4];
    for (int i = 0; i < 4; ++i) {
        trendDataArray[i] = new Value(kArrayType);
        jsonResult.genInsideArray1(*trendDataArray[i], trendDataValue[i], allocator);
    }

    // 5.4 label和data合起来成为一个小json
    Value lableAndData[4];
    for (int i = 0; i < 4; ++i) {
        lableAndData[i].SetObject();
        lableAndData[i].AddMember("label", trendLableValue[i], allocator);
        lableAndData[i].AddMember("data", *trendDataArray[i], allocator);
    }

    // 5.5 生成数组
    Value trend(kArrayType);
    for (int i = 0; i < 4; ++i) {
        trend.PushBack(lableAndData[i], allocator);
    }

    // 5.6 加入jsonResult.jsonDoc
    jsonResult.jsonDoc.AddMember("trend", trend, allocator);

    return jsonResult.genJson(allocator);
}

string viewPhysicalCondition(string date) {    
    CGenJson jsonResult;
    Document::AllocatorType& allocator = jsonResult.jsonDoc.GetAllocator();
    MyDB db;

    // 1、添加result信息
    jsonResult.jsonDoc.AddMember("result", "0", allocator);

    string sql = "SELECT * FROM PhysicalCondition WHERE date = \"" + date + "\"";
    if(!db.exeSQL(sql, RETRIEVE)) {
        return CGenJson::genResultJson(MYSQL_ERR);
    }

    // 2、pageTotal
    int intPageTotal = db.sqlResult.size();
    //string pageTotalValue = to_string(intPageTotal);
    Value pageTotal;
    pageTotal.SetInt(intPageTotal);
    //pageTotal.SetString(pageTotalValue.c_str(), allocator);
    jsonResult.jsonDoc.AddMember("pageTotal", pageTotal, allocator);

    // 3、info数组
    Value info(kArrayType);
    if (!intPageTotal) {
        jsonResult.jsonDoc.AddMember("info", kArrayType, allocator);
        return jsonResult.genJson(allocator);
    }
    vector<vector<string>> infoData = db.sqlResult;
    vector<string> infoKey = {"userID", "date", "todayTemperature", "HuBeiContact"};
    vector<Value> infoPart(intPageTotal);
    for (int i = 0; i < intPageTotal; ++i) {
        jsonResult.genInsideJson(infoPart[i], infoKey, infoData[i], allocator);
    }
    for (int i = 0; i < intPageTotal; ++i) { // 每一组信息
        info.PushBack(infoPart[i], allocator);
    }
    jsonResult.jsonDoc.AddMember("info", info, allocator);
    return jsonResult.genJson(allocator);
}
