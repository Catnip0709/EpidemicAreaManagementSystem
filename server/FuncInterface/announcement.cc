#include "../interface.h"
#include "../myDB.h"
#include "../common.h"
using namespace std;

// 新增公告
string newAnnouncement(string id, string title, string content, string state, string date) {
    string result = "{\"result\":";
    MyDB db;
	db.initDB(db.myHost, db.myUser, db.myPWD, db.myTable);

    // 先判断该ID是否已经注册过
    string sql = "SELECT 1 FROM `announcement` WHERE id = \"" + id + "\" LIMIT 1;";
	if(!db.exeSQL(sql)) {
        result = result + to_string(MYSQL_ERR) + "}";
        return result;
    }
    
    if (mysql_num_rows(db.result)) { // 该ID已注册过
        result = result + to_string(BEEN_REGISTER) + "}";
        return result;
    }

    sql = "INSERT INTO `announcement` VALUES (" + id + 
          "\"" + title + "\"," +
          "\"" + content + "\"," +
          "\"" + state + "\"," +
          "\"" + date + "\");";

    if(!db.exeSQL(sql)) { // 插入失败
        result = result + to_string(MYSQL_ERR) + "}";
        return result;
    }

    result = result + to_string(SUCCESS) + "}";
    return result;
}

// 修改公告
string modifyAnnouncement(string id, string title, string content, string state, string date) {
    string result = "{\"result\":";
    MyDB db;
	db.initDB(db.myHost, db.myUser, db.myPWD, db.myTable);

    // 先判断该ID是否已经注册过
    string sql = "SELECT 1 FROM `announcement` WHERE id = \"" + id + "\" LIMIT 1;";
	if(!db.exeSQL(sql)) {
        result = result + to_string(MYSQL_ERR) + "}";
        return result;
    }
    
    if (mysql_num_rows(db.result) == 0) { // 不存在该公告
        result = result + to_string(HAVENT_REGISTER) + "}";
        return result;
    }

    // 删掉旧的
    string sql = "DELETE FROM `announcement` WHERE id = " + id + ";";
    if(!db.exeSQL(sql)) { // 删除失败
        result = result + to_string(MYSQL_ERR) + "}";
        return result;
    }

    // 插入新的
    sql = "INSERT INTO `announcement` VALUES (" + id + 
          "\"" + title + "\"," +
          "\"" + content + "\"," +
          "\"" + state + "\"," +
          "\"" + date + "\");";

    if(!db.exeSQL(sql)) { // 插入失败
        result = result + to_string(MYSQL_ERR) + "}";
        return result;
    }

    result = result + to_string(SUCCESS) + "}";
    return result;
}

// 删除公告
string deleteAnnouncement(string id) {
    string result = "{\"result\":";
    MyDB db;
	db.initDB(db.myHost, db.myUser, db.myPWD, db.myTable);

    string sql = "DELETE FROM `announcement` WHERE id = " + id + ";";
    if(!db.exeSQL(sql)) { // 删除失败
        result = result + to_string(MYSQL_ERR) + "}";
        return result;
    }

    result = result + to_string(SUCCESS) + "}";
    return result;
}