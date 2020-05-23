#include "../interface.h"
#include "../myDB.h"
#include "../common.h"
using namespace std;

// 用户登录
string userLogin (string userID, string password) {
    string result = "{\"result\":";
    MyDB db;
	db.initDB(db.myHost, db.myUser, db.myPWD, db.myTable);

    string sql = "SELECT password FROM User WHERE userID = \"" + userID + "\" LIMIT 1;";
    if (!db.exeSQL(sql, RETRIEVE)) { // MYSQL执行失败
        result = result + to_string(MYSQL_ERR) + "}";
        return result;
    }
    
    if (!mysql_num_rows(db.result)) { // 该ID尚未注册
        result = result + to_string(HAVENT_REGISTER) + "}";
        return result;
    }
    
    if (!(db.sqlResult.size() == 1 && db.sqlResult[0].size() == 1)) { // 数据出错，应该只得到一个结果
        result = result + to_string(DATA_ERR) + "}";
        return result;
    } 
    
    if (!db.sqlResult[0][0].compare(password)) { // 密码错误，db.sqlResult[0][0]是正确密码
        result = result + to_string(PWD_ERR) + "}";
        return result;
    }
    
    result = result + to_string(SUCCESS) + "}";   
    return result;
}

// 用户注册
string userRegister (string userID, string userName, string password, string isAdministrator, string buildingID, string familyID) {
    string result = "{\"result\":";
    MyDB db;
	db.initDB(db.myHost, db.myUser, db.myPWD, db.myTable);

    // 先判断该ID是否已经注册过
    string sql = "SELECT 1 FROM User WHERE userID = \"" + userID + "\" LIMIT 1;";
	if(!db.exeSQL(sql, RETRIEVE)) {
        result = result + to_string(MYSQL_ERR) + "}";
        return result;
    }
    
    if (mysql_num_rows(db.result)) { // 该ID已注册过
        result = result + to_string(BEEN_REGISTER) + "}";
        return result;
    }

    sql = "INSERT INTO User VALUES (\"" + userID + "\", \"" + userName + "\", \"" + password + "\", " + isAdministrator + ", " + buildingID + ", " + familyID + ");";
    
    if(!db.exeSQL(sql, CREATE)) { // 注册信息插入mysql失败
        result = result + to_string(MYSQL_ERR) + "}";
        return result;
    }

    result = result + to_string(SUCCESS) + "}";
    return result;
}