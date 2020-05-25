#include "../interface.h"
#include "../myDB.h"
#include "../common.h"
using namespace std;

// 修改管理员userID对buildingID的管理权
string modifyInchargeBuilding(string userID, string buildingID, string isAdd) {
    string result = "{\"result\":";
    MyDB db;

    // 查看该userID是否已经注册过、是不是管理员
    string sql = "SELECT isAdministrator FROM User WHERE userID = \"" + userID + "\"  LIMIT 1;";
    if(!db.exeSQL(sql, RETRIEVE)) {
        result = result + to_string(MYSQL_ERR) + "}";
        return result;
    }
    if (!mysql_num_rows(db.result)) { // 该ID尚未注册过
        result = result + to_string(HAVENT_REGISTER) + "}";
        return result;
    }
    if (!(db.sqlResult.size() == 1 && db.sqlResult[0].size() == 1)) { // 数据出错，应该只得到一个数据
        result = result + to_string(DATA_ERR) + "}";
        return result;
    } 
    if (!db.sqlResult[0][0].compare("0")) { // 该用户不是管理员
        result = result + to_string(PWD_ERR) + "}";
        return result;
    }

    // 查看该userID对目标buildingID是否有管理权
    bool isInCharged = false;
    sql = "SELECT buildingID FROM Building WHERE administratorID = \"" + userID + "\";";
    if (!db.exeSQL(sql, RETRIEVE)) {
        result = result + to_string(MYSQL_ERR) + "}";
        return result;
    }
    for (int i = 0; i < mysql_num_rows(db.result); i++) {
        if (!db.sqlResult[i][0].compare(buildingID)) {
            isInCharged = true;
        }
    }

    if (!isAdd.compare("0")) { // 删除userID对buildingID的管理权
        if (!isInCharged) { // userID对buildingID本来就没有管理权
            result = result + to_string(SUCCESS) + "}";
            return result;
        }
        sql = "DELETE FROM Building WHERE administratorID = \"" + userID + "\" AND buildingID = \"" + buildingID + "\";";
        if (!db.exeSQL(sql, DELETE)) {
            result = result + to_string(MYSQL_ERR) + "}";
            return result;
        }
    }
    else if (!isAdd.compare("1")) { // 添加userID对buildingID的管理权
        if (isInCharged) { // userID对buildingID本来就有管理权
            result = result + to_string(SUCCESS) + "}";
            return result;
        }
        sql = "INSERT INTO Building VALUE (\"" + buildingID + "\", \"" + userID + "\");";
        if (!db.exeSQL(sql, CREATE)) {
            result = result + to_string(MYSQL_ERR) + "}";
            return result;
        }
    }
    else { // 请求非法，isAdd值不合法
        result = result + to_string(ILLEGAL_REQUEST) + "}";
        return result;
    }

    result = result + to_string(SUCCESS) + "}";
    return result;
}