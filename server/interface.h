#include <iostream>
#include <string>
#include <httplib.h>
#include <unordered_map>

#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/writer.h"
#include "rapidjson/include/rapidjson/stringbuffer.h"

using namespace std;
using namespace httplib;
using namespace rapidjson;

// 辅助函数接口
Document parseJson(const Request &req);

// 业务接口
string userLogin(string userID, string password);
string userRegister (string userID, string userName, string password, string isAdministrator, string buildingID, string familyID);
string deleteAnnouncement(string id);