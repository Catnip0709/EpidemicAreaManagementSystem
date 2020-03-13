#include <iostream>
#include <string>
#include "interface.h"
#include "myDB.h"
#include "common.h"
using namespace std;

int userLogin (string userID, string password) {
    MyDB db;
	db.initDB(db.myHost, db.myUser, db.myPWD, db.myTable);

    string sql = "SELECT password FROM User WHERE userID = \"" + userID + "\" LIMIT 1;";
    if(!db.exeSQL(sql)) {
        return MYSQL_ERR; // MYSQL执行失败
    }
    if (!db.result->row_count) { 
        return HAVENT_REGISTER; // 该ID尚未注册
    }

    if(!(db.sqlResult.size() == 1 && db.sqlResult[0].size() == 1)){
        return DATA_ERR; // 数据出错，应该只得到一个密码
    } 
    string correctPwd = db.sqlResult[0][0];

    if(!correctPwd.compare(password)) {
        return PWD_ERR; // 密码错误
    }

    return SUCCESS;
}

int userRegister (string userID, string userName, string password, bool isAdministrator, int buildingID, int familyID) {
    MyDB db;
	db.initDB(db.myHost, db.myUser, db.myPWD, db.myTable);

    // 先判断该ID是否已经注册过
    string sql = "SELECT 1 FROM User WHERE userID = \"" + userID + "\" LIMIT 1;";
	if(!db.exeSQL(sql)) {
        return MYSQL_ERR;
    }
    
    if (mysql_num_rows(db.result)) { // 该ID已注册过
        return BEEN_REGISTER;
    }

    sql = "INSERT INTO User VALUES (\"" + userID + "\", \"" + userName + "\", \"" + password + "\", " + to_string(isAdministrator) + ", " + to_string(buildingID) + ", " + to_string(familyID) + ");";
    
    if(!db.exeSQL(sql)) {
        return MYSQL_ERR;
    }
    return SUCCESS;
}