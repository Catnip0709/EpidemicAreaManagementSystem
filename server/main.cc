#include <stdlib.h>

#include "myDB.h"
#include "common.h"
#include "interface.h"

using namespace httplib;
using namespace std;
using namespace rapidjson;

int main(int argc,char **argv) {
  if(argc != 2) { // 启动时需要指定监听的端口，否则报错
    cerr << "start paras error" << endl;
    return -1;
  }

  uint16_t port = atoi(argv[1]);
  Server svr;

  svr.Get("/hi", [](const Request & req, Response &res) {
    res.set_content("Hello World!", "text/plain");
  });

  svr.Post("/userRegister", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result = userRegister(doc["userID"].GetString(), doc["userName"].GetString(), doc["password"].GetString(), doc["phone"].GetString(), doc["buildingID"].GetString(), doc["familyID"].GetString());
    // 返回结果给客户端
  });

  svr.Post("/adminRegister", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result = adminRegister(doc["userID"].GetString(), doc["userName"].GetString(), doc["password"].GetString(), doc["phone"].GetString(), doc["buildingID"].GetString());
    // 返回结果给客户端
  });

  svr.Post("/login", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result = login(doc["userID"].GetString(), doc["password"].GetString(), doc["isAdmin"].GetString());
    // 返回结果给客户端
  });

  svr.Post("/newAnnouncement", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result = newAnnouncement(doc["title"].GetString(), doc["content"].GetString(), doc["state"].GetString(), doc["date"].GetString());
    // 返回结果给客户端
  });

  svr.Post("/modifyAnnouncement", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result = modifyAnnouncement(doc["id"].GetString(), doc["title"].GetString(), doc["content"].GetString(), doc["state"].GetString(), doc["date"].GetString());
    // 返回结果给客户端
  });

  svr.Post("/deleteAnnouncement", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result = deleteAnnouncement(doc["id"].GetString());
    // 返回结果给客户端
  });

  svr.Post("/modifyInchargeBuilding", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result = modifyInchargeBuilding(doc["userID"].GetString(), doc["buildingID"].GetString(), doc["isAdd"].GetString());
    // 返回结果给客户端
  });

  svr.Post("/physicalCondition", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result = physicalCondition(doc["userID"].GetString(), doc["date"].GetString(), doc["todayTemperature"].GetString(), doc["HuBeiContact"].GetString());
    // 返回结果给客户端
  });

  svr.Post("/newEquipment", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result = newEquipment(doc["equipmentName"].GetString(), doc["storage"].GetString());
    // 返回结果给客户端
  });

  svr.Post("/modifyEquipment", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result = modifyEquipment(doc["equipmentID"].GetString(), doc["storageChange"].GetString());
    // 返回结果给客户端
  });

  svr.Post("/applyEquipment", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result = applyEquipment(doc["userID"].GetString(), doc["equipmentName"].GetString(), doc["amount"].GetString(), doc["date"].GetString());
    // 返回结果给客户端
  });

  svr.listen("0.0.0.0", port);
}
