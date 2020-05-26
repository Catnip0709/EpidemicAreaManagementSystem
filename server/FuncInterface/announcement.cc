#include "../interface.h"
#include "../myDB.h"
#include "../common.h"
#include "../genJson.h"
using namespace std;

// 新增公告
string newAnnouncement(string title, string content, string state, string date) {
    CGenJson jsonResult;
    Document::AllocatorType& allocator = jsonResult.jsonDoc.GetAllocator();
    MyDB db;

    string sql = "INSERT INTO `Announcement` (title, content, state, date) VALUES (\"" + title + "\", \"" + content + "\", \"" + state + "\", \"" + date + "\");";
    if (!db.exeSQL(sql, CREATE)) { // 插入失败
        return CGenJson::genResultJson(MYSQL_ERR);
    }

    Value v1, v2;
    v1.SetString(to_string(SUCCESS).c_str(), allocator);
    jsonResult.jsonDoc.AddMember("result", v1, allocator);
    v2.SetString(to_string(mysql_insert_id(db.connection)).c_str(), allocator);
    jsonResult.jsonDoc.AddMember("id", v2, allocator);

    return jsonResult.genJson(allocator);
}

// 修改公告
string modifyAnnouncement(string id, string title, string content, string state, string date) {
    MyDB db;

    // 先判断该ID是否已经注册过
    string sql = "SELECT 1 FROM `Announcement` WHERE id = \"" + id + "\" LIMIT 1;";
	if(!db.exeSQL(sql, RETRIEVE)) {
        return CGenJson::genResultJson(MYSQL_ERR);
    }
    
    if (mysql_num_rows(db.result) == 0) { // 不存在该公告
        return CGenJson::genResultJson(HAVENT_REGISTER);
    }

    sql = "UPDATE `Announcement` SET title = \"" + title + "\", content = \"" + content + "\", state = \"" + state + "\", date = \"" + date + "\" WHERE id = " + id + ";";
    if(!db.exeSQL(sql, UPDATE)) {
        return CGenJson::genResultJson(MYSQL_ERR);
    }
    
    return CGenJson::genResultJson(SUCCESS);
}

// 删除公告
string deleteAnnouncement(string id) {
    MyDB db;

    string sql = "DELETE FROM `Announcement` WHERE id = " + id + ";";
    if(!db.exeSQL(sql, CREATE)) { // 删除失败
        return CGenJson::genResultJson(MYSQL_ERR);
    }

    return CGenJson::genResultJson(SUCCESS);
}