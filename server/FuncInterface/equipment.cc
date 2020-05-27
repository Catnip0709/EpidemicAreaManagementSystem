#include "../interface.h"
#include "../myDB.h"
#include "../common.h"
#include "../genJson.h"
#include<unordered_map>
using namespace std;

// 管理员新增一种物资
string newEquipment(string equipmentName, string storage) {
    CGenJson jsonResult;
    Document::AllocatorType& allocator = jsonResult.jsonDoc.GetAllocator();
    MyDB db;

    string sql = "INSERT INTO EquipmentStorage (equipmentName,storage) VALUES (\"" + equipmentName + "\","+ storage + ");";
    if(!db.exeSQL(sql, CREATE)) { // 插入失败
        return CGenJson::genResultJson(MYSQL_ERR);
    }

    Value v1, v2;
    v1.SetString(to_string(SUCCESS).c_str(), allocator);
    jsonResult.jsonDoc.AddMember("result", v1, allocator);
    v2.SetString(to_string(mysql_insert_id(db.connection)).c_str(), allocator);
    jsonResult.jsonDoc.AddMember("id", v2, allocator);

    return jsonResult.genJson(allocator);
}

// 管理员修改物资存量
string modifyEquipment(string equipmentID, string storageChange) {
    MyDB db;

    // 先判断该ID是否已经注册过，顺便获取目标物资存量
    string sql = "SELECT storage FROM EquipmentStorage WHERE equipmentID = \"" + equipmentID + "\" LIMIT 1;";
	if(!db.exeSQL(sql, RETRIEVE)) {
        return CGenJson::genResultJson(MYSQL_ERR);
    }    
    if (mysql_num_rows(db.result) == 0) { // 不存在该物资ID
        return CGenJson::genResultJson(HAVENT_REGISTER);
    }

    // 请求减少物资时需要判断物资是否够
    if (storageChange[0] == '-') {
        int storage = atoi(db.sqlResult[0][0].c_str());
        int left = storage + atoi(storageChange.c_str());
        if(left < 0) { // 物资不足
            return CGenJson::genResultJson(NOT_ENOUGH);
        }
    }

    sql = "UPDATE EquipmentStorage SET storage = storage" + storageChange + " WHERE equipmentID = " + equipmentID + ";";
    if(!db.exeSQL(sql, UPDATE)) {
        return CGenJson::genResultJson(MYSQL_ERR);
    }
    
    return CGenJson::genResultJson(SUCCESS);
}

// 人民提出物资申请
string applyEquipment(string userID, string equipmentName, string amount, string date) {
    CGenJson jsonResult;
    Document::AllocatorType& allocator = jsonResult.jsonDoc.GetAllocator();
    MyDB db;

    // 先判断该userID是否已经注册过，顺便获取栋号
    string sql = "SELECT buildingID FROM User WHERE userID = \"" + userID + "\" LIMIT 1;";
	if(!db.exeSQL(sql, RETRIEVE)) {
        return CGenJson::genResultJson(MYSQL_ERR);
    }
    if (!mysql_num_rows(db.result)) { // 该ID尚未注册
        return CGenJson::genResultJson(HAVENT_REGISTER);
    }
    string buildingID = db.sqlResult[0][0];

    // 提交申请
    sql = "INSERT INTO ApplyEquipment (userID,buildingID,equipmentName,amount,date,state) VALUES (\"" + userID + "\"," + buildingID + ",\"" + equipmentName + "\"," + amount + ",\"" + date + "\",2);";
    if (!db.exeSQL(sql, CREATE)) {
        return CGenJson::genResultJson(MYSQL_ERR);
    }

    Value v1, v2;
    v1.SetString(to_string(SUCCESS).c_str(), allocator);
    jsonResult.jsonDoc.AddMember("result", v1, allocator);
    v2.SetString(to_string(mysql_insert_id(db.connection)).c_str(), allocator);
    jsonResult.jsonDoc.AddMember("applyID", v2, allocator);

    return jsonResult.genJson(allocator);
}

// 管理员读取管理的栋的所有物资申请列表
string getApplyEquipment(string adminID) {
    CGenJson jsonResult;
    Document::AllocatorType& allocator = jsonResult.jsonDoc.GetAllocator();
    MyDB db;

    // 查看该adminID是否注册了管理员，以及他管理的栋号
    string sql = "SELECT buildingID FROM Admin WHERE userID = \"" + adminID + "\" LIMIT 1;";
	if(!db.exeSQL(sql, RETRIEVE)) {
        return CGenJson::genResultJson(MYSQL_ERR);
    }    
    if (mysql_num_rows(db.result) == 0) { 
        return CGenJson::genResultJson(HAVENT_REGISTER);
    }

    vector<string> buildingID = stringCut(db.sqlResult[0][0],"_");
    if (!buildingID.size()) { // 该管理员没有在管的栋
        return "{\"result\":0,\"applyNum\":0, \"applyInfo\": []}";
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
        return CGenJson::genResultJson(MYSQL_ERR);
    }  
    if (!db.sqlResult.size()) { // 管理员管的栋没有申请信息
        return "{\"result\":0,\"applyNum\":0, \"applyInfo\": []}";
    }

    // json结果
    Value v1, v2, v3(kArrayType);
    v1.SetString(to_string(SUCCESS).c_str(), allocator);
    jsonResult.jsonDoc.AddMember("result", v1, allocator);
    v2.SetString(to_string(db.sqlResult.size()).c_str(), allocator);
    jsonResult.jsonDoc.AddMember("applyNum", v2, allocator);

    vector<string> applyInfoKey = {"applyID", "userID", "equipmentName", "amount", "date", "state"};
    jsonResult.genInsideArray2(v3, applyInfoKey, db.sqlResult, allocator);
    jsonResult.jsonDoc.AddMember("applyInfo", v3, allocator);
    
    return jsonResult.genJson(allocator);
}

// 管理员处理某一物资申请（同意/驳回）
string handleApplication(string applyID, string result, string adminID, string reply, string date) {
    MyDB db;

    // 先判断该applyID是否已经注册过
    string sql = "SELECT 1 FROM ApplyEquipment WHERE applyID = \"" + applyID + "\" LIMIT 1;";
	if (!db.exeSQL(sql, RETRIEVE)) {
        return CGenJson::genResultJson(MYSQL_ERR);
    }
    if (!mysql_num_rows(db.result)) { // 该applyID尚未注册
        return CGenJson::genResultJson(HAVENT_REGISTER);
    }

    // 插入HandleApplication表格
    sql = "INSERT INTO HandleApplication VALUE(\"" + applyID + "\"," + result + ",\"" + adminID + "\",\"" + reply + "\",\"" + date + "\");";
    if (!db.exeSQL(sql, CREATE)) {
        return CGenJson::genResultJson(MYSQL_ERR);
    }

    // 将ApplyEquipment表格对应订单的当前状态更正
    sql = "UPDATE ApplyEquipment SET state = " + result + " WHERE applyID = " + applyID + ";";
    if (!db.exeSQL(sql, UPDATE)) {
        return CGenJson::genResultJson(MYSQL_ERR);
    }

    return CGenJson::genResultJson(SUCCESS);
}
//select * from EquipmentStorage
string getAllEquipment()
{
    MyDB db;
    Document jsonDoc;
    jsonDoc.SetObject();
    Document::AllocatorType& allocator = jsonDoc.GetAllocator();
    StringBuffer ResBuffer;
    Writer<StringBuffer> Writer(ResBuffer);
    do{
        Value Key,value;
        Key.SetString("result");
        string sql="select * from EquipmentStorage;";
        if(!db.exeSQL(sql,RETRIEVE)) { 
            value.SetInt(MYSQL_ERR);
            jsonDoc.AddMember(Key,value,allocator); 
            break;
        }
        if(db.sqlResult.empty()){
            Value EmptyArray(kArrayType);
            value.SetInt(SUCCESS);
            jsonDoc.AddMember(Key,value,allocator);
            Key.SetString("list");
            jsonDoc.AddMember(Key,EmptyArray,allocator);
            Key.SetString("pageTotal");
            value.SetInt(0); 
            jsonDoc.AddMember(Key,value,allocator);
            break;
        }
        unordered_map<int,string> KeyNames;
        KeyNames[0]="id";
        KeyNames[1]="name";
        KeyNames[2]="remain";
        GenJsonObjectArray("list",KeyNames,db.sqlResult,jsonDoc);
        value.SetInt(SUCCESS);
        jsonDoc.AddMember(Key,value,allocator); 
        Key.SetString("pageTotal");
        value.SetInt(db.sqlResult.size());
        jsonDoc.AddMember(Key,value,allocator); 
    }while(false);
    jsonDoc.Accept(Writer);
    cout<<ResBuffer.GetString()<<endl;
    return ResBuffer.GetString();
}
