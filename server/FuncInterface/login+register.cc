#include "../interface.h"
#include "../myDB.h"
#include "../common.h"
using namespace std;

// 登录
string login(string userID, string password, string isAdmin) {
    string result = "{\"result\":";
    MyDB db;

    string table = "User";
    if (!isAdmin.compare("1")) {
        table = "Admin";
    }

    string sql = "SELECT password FROM " + table +" WHERE userID = \"" + userID + "\" LIMIT 1;";
    if (!db.exeSQL(sql, RETRIEVE)) { // MYSQL执行失败
        return genResultJson(MYSQL_ERR);
        
    }
    
    if (!mysql_num_rows(db.result)) { // 该ID尚未注册
        return genResultJson(HAVENT_REGISTER);
    }
    
    if (!(db.sqlResult.size() == 1 && db.sqlResult[0].size() == 1)) { // 数据出错，应该只得到一个结果
        return genResultJson(DATA_ERR);
    } 
    
    if (db.sqlResult[0][0].compare(password)) { // 密码错误，db.sqlResult[0][0]是正确密码
        return genResultJson(PWD_ERR);
    }
    
    return genResultJson(SUCCESS);
}

// 普通用户注册
string userRegister(string userID, string userName, string password, string phone, string buildingID, string familyID, string state) {
    MyDB db;
    // 先判断该ID是否已经注册过
    string sql = "SELECT 1 FROM User WHERE userID = \"" + userID + "\" LIMIT 1;";
	if(!db.exeSQL(sql, RETRIEVE)) {
        return genResultJson(MYSQL_ERR);
    }    
    if (mysql_num_rows(db.result)) { // 该ID已注册过
        return genResultJson(BEEN_REGISTER);
    }

    sql = "INSERT INTO User VALUES (\"" + userID + "\", \"" + userName + "\", \"" + password + "\", " + phone + ", " + buildingID + ", " + familyID + "," + state +");";    
    if(!db.exeSQL(sql, CREATE)) { 
        return genResultJson(MYSQL_ERR);
    }

    return genResultJson(SUCCESS);
}

// 管理员注册
string adminRegister(string userID, string userName, string password, string phone, string buildingID) {
    MyDB db;

    // 先判断该管理员ID是否已经注册过
    string sql = "SELECT 1 FROM Admin WHERE userID = \"" + userID + "\" LIMIT 1;";
	if(!db.exeSQL(sql, RETRIEVE)) {
        return genResultJson(MYSQL_ERR);
    }
    
    if (mysql_num_rows(db.result)) { // 该ID已注册过
        return genResultJson(BEEN_REGISTER);
    }

    sql = "INSERT INTO Admin VALUES (\"" + userID + "\", \"" + userName + "\", \"" + password + "\", " + phone + ", \"" + buildingID + "\");";
    
    if(!db.exeSQL(sql, CREATE)) { // 注册信息插入mysql失败
        return genResultJson(MYSQL_ERR);
    }

    return genResultJson(SUCCESS);
}