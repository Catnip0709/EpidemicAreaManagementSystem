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
    string userRegisterResult = userRegister(doc["userID"].GetString(), doc["userName"].GetString(), doc["password"].GetString(), doc["phone"].GetString(), doc["buildingID"].GetString(), doc["familyID"].GetString());
    // 返回结果给客户端
  });

  svr.Post("/adminRegister", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string adminRegisterResult = adminRegister(doc["userID"].GetString(), doc["userName"].GetString(), doc["password"].GetString(), doc["phone"].GetString(), doc["buildingID"].GetString());
    // 返回结果给客户端
  });

  svr.Post("/login", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string loginResult = login(doc["userID"].GetString(), doc["password"].GetString(), doc["isAdmin"].GetString());
    // 返回结果给客户端
  });

  svr.Post("/newAnnouncement", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string newAnnouncementResult = newAnnouncement(doc["id"].GetString(), doc["title"].GetString(), doc["content"].GetString(), doc["state"].GetString(), doc["date"].GetString());
    // 返回结果给客户端
  });

  svr.Post("/modifyAnnouncement", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string modifyAnnouncementResult = modifyAnnouncement(doc["id"].GetString(), doc["title"].GetString(), doc["content"].GetString(), doc["state"].GetString(), doc["date"].GetString());
    // 返回结果给客户端
  });

  svr.Post("/deleteAnnouncement", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string deleteAnnouncementResult = deleteAnnouncement(doc["id"].GetString());
    // 返回结果给客户端
  });

  svr.Post("/modifyInchargeBuilding", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string modifyInchargeBuildingResult = modifyInchargeBuilding(doc["userID"].GetString(), doc["buildingID"].GetString(), doc["isAdd"].GetString());
    // 返回结果给客户端
  });

  svr.listen("0.0.0.0", port);
}
