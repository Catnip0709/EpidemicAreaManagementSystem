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

// 辅助函数接口
Document parseJson(const Request &req);
wstring StringToWString(const string& s);

// 业务接口
string login(string userID, string password, string isAdmin);
string userRegister(string userID, string userName, string password, string phone, string buildingID, string familyID);
string adminRegister(string userID, string userName, string password, string phone, string buildingID);

string newAnnouncement(string title, string content, string state, string date);
string modifyAnnouncement(string id, string title, string content, string state, string date);
string deleteAnnouncement(string id);

string modifyInchargeBuilding(string userID, string buildingID, string isAdd);

string physicalCondition(string userID, string date, string todayTemperature, string HuBeiContact);

string newEquipment(string equipmentName, string storage);
string modifyEquipment(string equipmentID, string storageChange);
string applyEquipment(string userID, string equipmentName, string amount, string date);