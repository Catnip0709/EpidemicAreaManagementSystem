#include "../interface.h"
#include "../myDB.h"
#include "../common.h"
using namespace std;

// 管理员新增一种物资
string newEquipment(string equipmentName, string storage) {
    string result = "{\"result\":";
    MyDB db;
	db.initDB(db.myHost, db.myUser, db.myPWD, db.myTable);

    string sql = "INSERT INTO EquipmentStorage (equipmentName,storage) VALUES (\"" + equipmentName + "\","+ storage + ");";
    if(!db.exeSQL(sql, CREATE)) { // 插入失败
        result = result + to_string(MYSQL_ERR) + ",\"id\":-1}";
        return result;
    }

    result = result + to_string(SUCCESS) + ",\"id\":" + to_string(mysql_insert_id(db.connection)) + "}";
    return result;
}

// 管理员修改物资存量
string modifyEquipment(string equipmentID, string storageChange) {
    string result = "{\"result\":";
    MyDB db;
	db.initDB(db.myHost, db.myUser, db.myPWD, db.myTable);

    // 先判断该ID是否已经注册过
    string sql = "SELECT 1 FROM EquipmentStorage WHERE equipmentID = \"" + equipmentID + "\" LIMIT 1;";
	if(!db.exeSQL(sql, RETRIEVE)) {
        result = result + to_string(MYSQL_ERR) + "}";
        return result;
    }    
    if (mysql_num_rows(db.result) == 0) { // 不存在该物资ID
        result = result + to_string(HAVENT_REGISTER) + "}";
        return result;
    }

    sql = "UPDATE EquipmentStorage SET storage = storage" + storageChange + " WHERE equipmentID = " + equipmentID + ";";
    if(!db.exeSQL(sql, UPDATE)) {
        result = result + to_string(MYSQL_ERR) + "}";
        return result;
    }
    
    result = result + to_string(SUCCESS) + "}";
    return result;
}

// 人民提出物资申请
string applyEquipment(string userID, string equipmentName, string amount, string date) {
    string result = "{\"result\":";
    MyDB db;
	db.initDB(db.myHost, db.myUser, db.myPWD, db.myTable);

    // 先判断该userID是否已经注册过
    string sql = "SELECT 1 FROM User WHERE userID = \"" + userID + "\" LIMIT 1;";
	if(!db.exeSQL(sql, RETRIEVE)) {
        result = result + to_string(MYSQL_ERR) + ", \"applyID\":-1}";
        return result;
    }
    if (!mysql_num_rows(db.result)) { // 该ID尚未注册
        result = result + to_string(HAVENT_REGISTER) + ", \"applyID\":-1}";
        return result;
    }

    // 提交申请
    sql = "INSERT INTO ApplyEquipment (userID,equipmentName,amount,date) VALUES (\"" + userID + "\",\"" + equipmentName + "\"," + amount + ",\"" + date + "\");";
    if (!db.exeSQL(sql, CREATE)) {
        result = result + to_string(MYSQL_ERR) + ", \"applyID\":-1}";
        return result;
    }

    result = result + to_string(SUCCESS) + ",\"applyID\":" + to_string(mysql_insert_id(db.connection)) + "}";
    return result;
}
