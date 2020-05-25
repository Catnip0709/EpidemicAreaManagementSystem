#include "../interface.h"
#include "../myDB.h"
#include "../common.h"
using namespace std;

// 管理员新增一种物资
string newEquipment(string equipmentName, string storage) {
    string result = "{\"result\":";
    MyDB db;

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

    // 先判断该userID是否已经注册过，顺便获取栋号
    string sql = "SELECT buildingID FROM User WHERE userID = \"" + userID + "\" LIMIT 1;";
	if(!db.exeSQL(sql, RETRIEVE)) {
        result = result + to_string(MYSQL_ERR) + ", \"applyID\":-1}";
        return result;
    }
    if (!mysql_num_rows(db.result)) { // 该ID尚未注册
        result = result + to_string(HAVENT_REGISTER) + ", \"applyID\":-1}";
        return result;
    }
    string buildingID = db.sqlResult[0][0];

    // 提交申请
    sql = "INSERT INTO ApplyEquipment (userID,buildingID,equipmentName,amount,date,state) VALUES (\"" + userID + "\"," + buildingID + ",\"" + equipmentName + "\"," + amount + ",\"" + date + "\",2);";
    if (!db.exeSQL(sql, CREATE)) {
        result = result + to_string(MYSQL_ERR) + ", \"applyID\":-1}";
        return result;
    }

    result = result + to_string(SUCCESS) + ",\"applyID\":" + to_string(mysql_insert_id(db.connection)) + "}";
    return result;
}

// 管理员读取管理的栋的所有物资申请列表
string getApplyEquipment(string adminID) {
    string result = "{\"result\":";
    MyDB db;

    // 查看该adminID是否注册了管理员，以及他管理的栋号
    string sql = "SELECT buildingID FROM Admin WHERE userID = \"" + adminID + "\" LIMIT 1;";
	if(!db.exeSQL(sql, RETRIEVE)) {
        result = result + to_string(MYSQL_ERR) + "}";
        return result;
    }    
    if (mysql_num_rows(db.result) == 0) { 
        result = result + to_string(HAVENT_REGISTER) + "}";
        return result;
    }

    vector<string> buildingID = stringCut(db.sqlResult[0][0]);
    if (!buildingID.size()) { // 该管理员没有在管的栋
        result = "{\"result\":0,\"applyNum\":0, \"applyInfo\": []}";
        return result;
    }

    // 读取物资申请列表
    sql = "SELECT applyID, userID, equipmentName, amount, date, state FROM ApplyEquipment WHERE buildingID = ";
    for (int i = 0; i < buildingID.size(); ++i) {
        sql += buildingID[i];
        if (i != buildingID.size() - 1) {
            sql += " OR buildingID = ";
        }
    }
    sql += ";";
    if(!db.exeSQL(sql, RETRIEVE)) {
        result = result + to_string(MYSQL_ERR) + "}";
        return result;
    }  
    if (!db.sqlResult.size()) { // 管理员管的栋没有申请信息
        result = "{\"result\":0,\"applyNum\":0, \"applyInfo\": []}";
        return result;
    }

    Document jsonDoc;
    jsonDoc.SetObject();
    Value myArray(kArrayType);
    Document::AllocatorType& allocator = jsonDoc.GetAllocator();
    vector<string> applyInfoKey = {"applyID", "userID", "equipmentName", "amount", "date", "state"};
    for (int i = 0; i < db.sqlResult.size(); ++i) { // 每一组信息
        Value objValue;
        objValue.SetObject();
        for (int j = 0; j < db.sqlResult[i].size(); ++j) {
            Value k, v;
            k.SetString(applyInfoKey[j].c_str(), allocator);
            v.SetString(db.sqlResult[i][j].c_str(), allocator);
            objValue.AddMember(k, v, allocator);
        }
        myArray.PushBack(objValue, allocator);
    }
    jsonDoc.AddMember("applyInfo", myArray, allocator);
    StringBuffer strbuf;
    Writer<StringBuffer> writer(strbuf);
    jsonDoc.Accept(writer);
    string applyInfo = strbuf.GetString(); 
    applyInfo.erase(applyInfo.begin());
    applyInfo.erase(applyInfo.end() - 1);

    result = result + to_string(SUCCESS) + ", \"applyNum\":" + to_string(db.sqlResult.size()) + "," + applyInfo + "}";

    return result;
}

// 管理员处理某一物资申请（同意/驳回）
string handleApplication(string applyID, string result, string adminID, string reply, string date) {
    string Result = "{\"result\":";
    MyDB db;

    // 先判断该applyID是否已经注册过
    string sql = "SELECT 1 FROM ApplyEquipment WHERE applyID = \"" + applyID + "\" LIMIT 1;";
	if (!db.exeSQL(sql, RETRIEVE)) {
        Result = Result + to_string(MYSQL_ERR) + "}";
        return Result;
    }
    if (!mysql_num_rows(db.result)) { // 该applyID尚未注册
        Result = Result + to_string(HAVENT_REGISTER) + "}";
        return Result;
    }

    // 插入HandleApplication表格
    sql = "INSERT INTO HandleApplication VALUE(\"" + applyID + "\"," + result + ",\"" + adminID + "\",\"" + reply + "\",\"" + date + "\");";
    if (!db.exeSQL(sql, CREATE)) {
        Result = Result + to_string(MYSQL_ERR) + "}";
        return Result;
    }

    // 将ApplyEquipment表格对应订单的当前状态更正
    sql = "UPDATE ApplyEquipment SET state = " + result + " WHERE applyID = " + applyID + ";";
    if (!db.exeSQL(sql, UPDATE)) {
        Result = Result + to_string(MYSQL_ERR) + "}";
        return Result;
    }

    Result = Result + to_string(SUCCESS) + "}";
    return Result;
}
