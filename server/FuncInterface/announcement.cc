#include "../interface.h"
#include "../myDB.h"
#include "../common.h"
using namespace std;

// 修改公告
string modifyAnnouncement(string id, string title, string content, string state, string date) {
    string result = "{\"result\":";
    MyDB db;
	db.initDB(db.myHost, db.myUser, db.myPWD, db.myTable);


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