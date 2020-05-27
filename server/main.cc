#include <stdlib.h>
#include "myDB.h"
#include "common.h"
#include "interface.h"

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
    string result;
    if (CheckParameter(req.body)) {
      result = userRegister(doc["userID"].GetString(), doc["userName"].GetString(), doc["password"].GetString(), doc["phone"].GetString(), doc["buildingID"].GetString(), doc["familyID"].GetString(), doc["familyID"].GetString());
    }
    else {
      result = "{\"result\":-2}";
    }
    res.set_content(result,"application/json");
  });

  svr.Post("/adminRegister", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result;
    if (CheckParameter(req.body)) {
      result = adminRegister(doc["userID"].GetString(), doc["userName"].GetString(), doc["password"].GetString(), doc["phone"].GetString(), doc["buildingID"].GetString());
    }
    else {
      result = "{\"result\":-2}";
    }
    res.set_content(result,"application/json");
  });

  svr.Post("/login", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result;
    if (CheckParameter(req.body)) {
      result = login(doc["userID"].GetString(), doc["password"].GetString(), doc["isAdmin"].GetString());
    }
    else {
      result = "{\"result\":-2}";
    }
    res.set_content(result,"application/json");
  });

  svr.Post("/newAnnouncement", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result;
    if (CheckParameter(req.body)) {
      result = newAnnouncement(doc["title"].GetString(), doc["content"].GetString(), doc["state"].GetString(), doc["date"].GetString());
    }
    else {
      result = "{\"result\":-2,\"id\":-1}";
    }
    res.set_content(result,"application/json");
  });

  svr.Post("/modifyAnnouncement", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result;
    if (CheckParameter(req.body)) {
      result = modifyAnnouncement(doc["id"].GetString(), doc["title"].GetString(), doc["content"].GetString(), doc["state"].GetString(), doc["date"].GetString());
    }
    else {
      result = "{\"result\":-2}";
    }
    res.set_content(result,"application/json");
  });

  svr.Post("/deleteAnnouncement", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result;
    if (CheckParameter(req.body)) {
      result = deleteAnnouncement(doc["id"].GetString());
    }
    else {
      result = "{\"result\":-2}";
    }
    res.set_content(result,"application/json");
  });

  svr.Post("/modifyInchargeBuilding", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result;
    if (CheckParameter(req.body)) {
      result = modifyInchargeBuilding(doc["userID"].GetString(), doc["buildingID"].GetString(), doc["isAdd"].GetString());
    }
    else {
      result = "{\"result\":-2}";
    }
    res.set_content(result,"application/json");
  });

  svr.Post("/physicalCondition", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result;
    if (CheckParameter(req.body)) {
      result = physicalCondition(doc["userID"].GetString(), doc["date"].GetString(), doc["todayTemperature"].GetString(), doc["HuBeiContact"].GetString());
    }
    else {
      result = "{\"result\":-2}";
    }
  });

  svr.Post("/newEquipment", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result;
    if (CheckParameter(req.body)) {
      result = newEquipment(doc["equipmentName"].GetString(), doc["storage"].GetString());
    }
    else {
      result = "{\"result\":-2,\"equipmentID\":-1} ";
    }
    res.set_content(result,"application/json");
  });

  svr.Post("/modifyEquipment", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result;
    if (CheckParameter(req.body)) {
      result = modifyEquipment(doc["equipmentID"].GetString(), doc["storageChange"].GetString());
    }
    else {
      result = "{\"result\":-2}";
    }
    res.set_content(result,"application/json");
  });

  svr.Post("/applyEquipment", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result;
    if (CheckParameter(req.body)) {
      result = applyEquipment(doc["userID"].GetString(), doc["equipmentName"].GetString(), doc["amount"].GetString(), doc["date"].GetString());
    }
    else {
      result = "{\"result\":-2}";
    }
    res.set_content(result,"application/json");
  });

  svr.Post("/getApplyEquipment", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result;
    if (CheckParameter(req.body)) {
      result = getApplyEquipment(doc["adminID"].GetString());
    }
    else {
      result = "{\"result\":-2}";
    }
    res.set_content(result,"application/json");
  });

  svr.Post("/handleApplication", [](const Request & req, Response &res) {
    Document doc = parseJson(req);
    string result;
    if (CheckParameter(req.body)) {
      result = handleApplication(doc["applyID"].GetString(), doc["result"].GetString(), doc["adminID"].GetString(), doc["reply"].GetString(), doc["date"].GetString());
    }
    else {
      result = "{\"result\":-2}";
    }
    res.set_content(result,"application/json");
  });
  svr.Post("/QuarantineInformation", [](const Request & req, Response &res) {
    Document doc;
    ParseResult pRes=doc.Parse(req.body.c_str());
    if(pRes){
      string result = GetQuarantineInfo(doc["type"].GetString(), doc["userID"].GetString());
      res.set_content(result,"application/json");
    }
    else{
      res.status=400;
      res.set_content("ERROR JSON","text/plain");
    }
  });
  svr.Post("/QuarantineStatChange", [](const Request & req, Response &res) {
    Document doc;
    ParseResult pRes=doc.Parse(req.body.c_str());
    if(pRes){
      string result = SetQuarantineInfo(doc["userID"].GetString(), doc["adminID"].GetString(),doc["endDate"].GetString());
      res.set_content(result,"application/json");
    }
    else{
      res.status=400;
      res.set_content("ERROR JSON","text/plain");
    }
  });
  svr.Post("/HealthStateChange", [](const Request & req, Response &res) {
    Document doc;
    ParseResult pRes=doc.Parse(req.body.c_str());
    if(pRes){
      string result = SetUserHealthStat(doc["userID"].GetString(), doc["adminID"].GetString(),doc["state"].GetString());
      res.set_content(result,"application/json");
    }
    else{
      res.status=400;
      res.set_content("ERROR JSON","text/plain");
    }
  });
  svr.Get("/GetAllEquipment",[](const Request & req, Response &res){
    string result=getAllEquipment();
    res.set_content(result,"application/json");
  });
  svr.Get("/GetAllAnnouncement",[](const Request & req, Response &res){
    string result=getAllAnnouncement();
    res.set_content(result,"application/json");
  });
  svr.listen("0.0.0.0", port);
}
