#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <httplib.h>
#include <unordered_map>
#include <assert.h>
#include <time.h>
#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/writer.h"
#include "rapidjson/include/rapidjson/stringbuffer.h"

using namespace std;
using namespace httplib;
using namespace rapidjson;

// 辅助函数接口，实现于helpFunc.cpp
bool CheckSQL(string sql);
bool CheckParameter(string param);
bool isParamValid(vector<string> param, Document *doc);
void GenJsonObjectArray(const string& ArrayName,const unordered_map<int,string>& keyNames,const vector<vector<string>>& queryResult,Document& jsonDoc);
void GenJsonLinearArray(const string& ArrayName,const vector<string>& LinearArray,Document& jsonDoc);
void GenJsonObjectWithObjectValue(const string& ArrayName,const unordered_map<string,string>& keyValues,Document& jsonDoc);
vector<string> stringCut(string str, string cut);
vector<string> get7date();

// 业务接口，实现于FuncInterface文件夹下
string login(string userID, string password, string isAdmin);
string userRegister(string userID, string userName, string password, string phone, string buildingID, string familyID, string state);
string adminRegister(string userID, string userName, string password, string phone, string buildingID);

string newAnnouncement(string title, string content, string state, string date);
string modifyAnnouncement(string id, string title, string content, string state, string date);
string deleteAnnouncement(string id);

string modifyInchargeBuilding(string userID, string buildingID, string isAdd);
string viewUserInfo(string userName, string buildingID, string familyID, string phone);

string physicalCondition(string userID, string date, string todayTemperature, string HuBeiContact);
string epidemicSituation(string date);
string viewPhysicalCondition(string date);

string newEquipment(string equipmentName, string storage);
string modifyEquipment(string equipmentID, string storageChange);
string applyEquipment(string userID, string equipmentName, string amount, string date);
string getApplyEquipment(string adminID);
string getAllEquipment();
string getAllAnnouncement();
string getApplyEquipment(string userID,string type);
string handleApplication(string applyID, string result, string adminID, string reply, string date);
string GetQuarantineInfo(string type,string id);
string SetQuarantineInfo(string userID,string adminID,string endDate);
string SetUserHealthStat(string userID,string adminID,string status);

