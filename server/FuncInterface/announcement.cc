#include "../interface.h"
#include "../myDB.h"
#include "../common.h"
using namespace std;

// 新增公告
string newAnnouncement(string title, string content, string state, string date) {
    string result = "{\"result\":";
    MyDB db;
	db.initDB();

    string sql = "INSERT INTO `Announcement` (title, content, state, date) VALUES (\"" + title + "\", \"" + content + "\", \"" + state + "\", \"" + date + "\");";
    if (!db.exeSQL(sql, CREATE)) { // 插入失败
        result = result + to_string(MYSQL_ERR) + ",\"id\":-1}";
        return result;
    }

    result = result + to_string(SUCCESS) + ",\"id\":" + to_string(mysql_insert_id(db.connection)) + "}";
    return result;
}

// 修改公告
string modifyAnnouncement(string id, string title, string content, string state, string date) {
    string result = "{\"result\":";
    MyDB db;
	db.initDB();

    // 先判断该ID是否已经注册过
    string sql = "SELECT 1 FROM `Announcement` WHERE id = \"" + id + "\" LIMIT 1;";
	if(!db.exeSQL(sql, RETRIEVE)) {
        result = result + to_string(MYSQL_ERR) + "}";
        return result;
    }
    
    if (mysql_num_rows(db.result) == 0) { // 不存在该公告
        result = result + to_string(HAVENT_REGISTER) + "}";
        return result;
    }

    sql = "UPDATE `Announcement` SET title = \"" + title + "\", content = \"" + content + "\", state = \"" + state + "\", date = \"" + date + "\" WHERE id = " + id + ";";
    if(!db.exeSQL(sql, UPDATE)) {
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
	db.initDB();

    string sql = "DELETE FROM `Announcement` WHERE id = " + id + ";";
    if(!db.exeSQL(sql, CREATE)) { // 删除失败
        result = result + to_string(MYSQL_ERR) + "}";
        return result;
    }

    result = result + to_string(SUCCESS) + "}";
    return result;
}