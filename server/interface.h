#include <iostream>
#include <string>
#include <vector>
#include <httplib.h>

#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/writer.h"
#include "rapidjson/include/rapidjson/stringbuffer.h"

using namespace std;
using namespace httplib;
using namespace rapidjson;

// 辅助函数接口，实现于helpFunc.cpp
Document parseJson(const Request &req);
bool CheckSQL(string sql);
bool CheckParameter(string param);
vector<string> stringCut(string str);
string genResultJson(int errCode);

// 业务接口，实现于FuncInterface文件夹下
string login(string userID, string password, string isAdmin);
string userRegister(string userID, string userName, string password, string phone, string buildingID, string familyID, string state);
string adminRegister(string userID, string userName, string password, string phone, string buildingID);

string newAnnouncement(string title, string content, string state, string date);
string modifyAnnouncement(string id, string title, string content, string state, string date);
string deleteAnnouncement(string id);

string modifyInchargeBuilding(string userID, string buildingID, string isAdd);

string physicalCondition(string userID, string date, string todayTemperature, string HuBeiContact);

string newEquipment(string equipmentName, string storage);
string modifyEquipment(string equipmentID, string storageChange);
string applyEquipment(string userID, string equipmentName, string amount, string date);
string getApplyEquipment(string adminID);
string handleApplication(string applyID, string result, string adminID, string reply, string date);