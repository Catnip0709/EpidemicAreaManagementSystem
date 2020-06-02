#include "../interface.h"
#include "../myDB.h"
#include "../common.h"
#include "../genJson.h"
using namespace std;

// 修改管理员userID对buildingID的管理权
string modifyInchargeBuilding(string userID, string buildingID, string isAdd) {
    MyDB db;

    // 查看该userID是否已经注册过、是不是管理员
    string sql = "SELECT isAdministrator FROM User WHERE userID = \"" + userID + "\"  LIMIT 1;";
    if(!db.exeSQL(sql, RETRIEVE)) {
        return CGenJson::genResultJson(MYSQL_ERR);
    }
    if (!mysql_num_rows(db.result)) { // 该ID尚未注册过
        return CGenJson::genResultJson(HAVENT_REGISTER);
    }
    if (!(db.sqlResult.size() == 1 && db.sqlResult[0].size() == 1)) { // 数据出错，应该只得到一个数据
        return CGenJson::genResultJson(DATA_ERR);
    } 
    if (!db.sqlResult[0][0].compare("0")) { // 该用户不是管理员
        return CGenJson::genResultJson(PWD_ERR);
    }

    // 查看该userID对目标buildingID是否有管理权
    bool isInCharged = false;
    sql = "SELECT buildingID FROM Building WHERE administratorID = \"" + userID + "\";";
    if (!db.exeSQL(sql, RETRIEVE)) {
        return CGenJson::genResultJson(MYSQL_ERR);
    }
    for (int i = 0; i < mysql_num_rows(db.result); i++) {
        if (!db.sqlResult[i][0].compare(buildingID)) {
            isInCharged = true;
        }
    }

    if (!isAdd.compare("0")) { // 删除userID对buildingID的管理权
        if (!isInCharged) { // userID对buildingID本来就没有管理权
            return CGenJson::genResultJson(SUCCESS);
        }
        sql = "DELETE FROM Building WHERE administratorID = \"" + userID + "\" AND buildingID = \"" + buildingID + "\";";
        if (!db.exeSQL(sql, DELETE)) {
            return CGenJson::genResultJson(MYSQL_ERR);
        }
    }
    else if (!isAdd.compare("1")) { // 添加userID对buildingID的管理权
        if (isInCharged) { // userID对buildingID本来就有管理权
            return CGenJson::genResultJson(SUCCESS);
        }
        sql = "INSERT INTO Building VALUE (\"" + buildingID + "\", \"" + userID + "\");";
        if (!db.exeSQL(sql, CREATE)) {
            return CGenJson::genResultJson(MYSQL_ERR);
        }
    }
    else { // 请求非法，isAdd值不合法
        return CGenJson::genResultJson(ILLEGAL_REQUEST);
    }

    return CGenJson::genResultJson(SUCCESS);
}

// 管理员查看所有用户信息
string viewUserInfo(string userID, string userName, string buildingID, string familyID, string phone) {
    CGenJson jsonResult;
    Document::AllocatorType& allocator = jsonResult.jsonDoc.GetAllocator();
    MyDB db;
    // 1、添加result信息，如果失败就直接调用genResultJson返回，这里可以直接拼接0
    jsonResult.jsonDoc.AddMember("result", "0", allocator);
    
    string sql = "SELECT userID, userName, phone, buildingID, familyID, state FROM User";
    
    if (!(userID.empty() && userName.empty() && buildingID.empty() && familyID.empty() && phone.empty())) {// 至少一项不为空        
        bool count = false; // 当前变量之前的变量是否有值（为了拼接sql）
        sql += " WHERE ";
        if (!userID.empty()) {
            count = true;
            sql = sql + "userID = \"" + userID + "\"";
        }
        if (!userName.empty()) {
            if(count) {
                sql = sql + " and userName = " + userName;
            }
            else {
                sql = sql + "userName = " + userName;
            }
            count = true;
        }
        if (!buildingID.empty()){
            if(count) {
                sql = sql + " and buildingID = " + buildingID;
            }
            else {
                sql = sql + "buildingID = " + buildingID;
            }
            count = true;
        }
        if (!familyID.empty()){
            if(count) {
                sql = sql + " and familyID = " + familyID;
            }
            else {
                sql = sql + "familyID = " + familyID;
            }
            count = true;
        }
        if (!phone.empty()){
            if(count) {
                sql = sql + " and phone = \"" + phone + "\"";
            }
            else {
                sql = sql + "phone = \"" + phone + "\"";
            }
            count = true;
        }
    }
    sql += ";";
    if(!db.exeSQL(sql, RETRIEVE)) {
        return CGenJson::genResultJson(MYSQL_ERR);
    }

    // 2、添加pageTotal信息
    int intPageTotal = db.sqlResult.size();
    string pageTotalValue = to_string(intPageTotal);
    Value pageTotal;
    pageTotal.SetString(pageTotalValue.c_str(), allocator);
    jsonResult.jsonDoc.AddMember("pageTotal", pageTotal, allocator);

    // 3、添加info信息    
    Value info(kArrayType);
    if (!intPageTotal) {
        jsonResult.jsonDoc.AddMember("info", kArrayType, allocator);
        return jsonResult.genJson(allocator);
    }

    // 3.1 生成除了familyName的其他信息（insideJson)
    vector<string> infoKeyStr = {"userID", "userName", "phone", "buildingID", "familyID", "state"};
    vector<vector<string>> infoValueStr = db.sqlResult;
    vector<Value> infoValue(intPageTotal);
    for (int i = 0; i < intPageTotal; ++i) {
        jsonResult.genInsideJson(infoValue[i], infoKeyStr, infoValueStr[i], allocator);
    }

    // 3.2 生成familyName
    for (int i = 0; i < intPageTotal; ++i) {
        sql = "SELECT userName FROM User WHERE buildingID = " + infoValueStr[i][3] + " and familyID = " + infoValueStr[i][4] + " and userID != \"" + infoValueStr[i][0] + "\";";
        if(!db.exeSQL(sql, RETRIEVE)) {
            return CGenJson::genResultJson(MYSQL_ERR);
        }
        Value familyName(kArrayType);
        int familyNameNum = mysql_num_rows(db.result);
        if (familyNameNum) {
            vector<string> familyNameStr;
            for (int j = 0; j < familyNameNum; ++j) {
                familyNameStr.push_back(db.sqlResult[j][0]);
            }
            jsonResult.genInsideArray1(familyName, familyNameStr, allocator);
        }
        infoValue[i].AddMember("familyName", familyName, allocator);
        info.PushBack(infoValue[i], allocator);
    }
    jsonResult.jsonDoc.AddMember("info", info, allocator);
    
    return jsonResult.genJson(allocator);
}