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
  auto ret=svr.set_mount_point("/","./html");
  if (!ret) {
    cerr<<"server mount error"<<endl;
    return -1;
  }
  ret = svr.set_mount_point("/admin", "./admin");
  if (!ret) {
    cerr<<"server mount error"<<endl;
    return -1;
  }
  ret = svr.set_mount_point("/user", "./user");
  if (!ret) {
    cerr<<"server mount error"<<endl;
    return -1;
  }
  svr.Post("/userRegister", [](const Request & req, Response &res) {
    string result;
    Document doc;
    ParseResult pRes = doc.Parse(req.body.c_str());
    vector<string> param = {"userID", "userName", "password", "phone", "buildingID", "familyID", "state"};
    if (pRes && CheckParameter(req.body) && isParamValid(param, &doc)) {
      result = userRegister(doc["userID"].GetString(), doc["userName"].GetString(), doc["password"].GetString(), doc["phone"].GetString(), doc["buildingID"].GetString(), doc["familyID"].GetString(), doc["state"].GetString());
      cout << "userRegister" << endl << result << endl << endl;
      res.set_content(result,"application/json");
    }
    else {
      res.status = 400;
      res.set_content("ERROR JSON","text/plain");
    }
  });

  svr.Post("/adminRegister", [](const Request & req, Response &res) {
    string result;
    Document doc;
    ParseResult pRes = doc.Parse(req.body.c_str());
    vector<string> param = {"userID", "userName", "password", "phone", "buildingID"};
    if (pRes && CheckParameter(req.body) && isParamValid(param, &doc)) {
      result = adminRegister(doc["userID"].GetString(), doc["userName"].GetString(), doc["password"].GetString(), doc["phone"].GetString(), doc["buildingID"].GetString());
      cout << "adminRegister" << endl << result << endl << endl;
      res.set_content(result,"application/json");
    }
    else {
      res.status = 400;
      res.set_content("ERROR JSON","text/plain");
    }
  });

  svr.Post("/login", [](const Request & req, Response &res) {
    string result;
    Document doc;
    ParseResult pRes = doc.Parse(req.body.c_str());
    vector<string> param = {"userID", "password", "isAdmin"};
    if (pRes && CheckParameter(req.body) && isParamValid(param, &doc)) {
      result = login(doc["userID"].GetString(), doc["password"].GetString(), doc["isAdmin"].GetString());
      cout << "login" << endl << result << endl << endl;
      res.set_content(result,"application/json");
    }
    else {
      res.status = 400;
      res.set_content("ERROR JSON","text/plain");
    }
  });

  svr.Post("/newAnnouncement", [](const Request & req, Response &res) {
    string result;
    Document doc;
    ParseResult pRes = doc.Parse(req.body.c_str());
    vector<string> param = {"title", "content", "state", "date"};
    if (pRes && CheckParameter(req.body) && isParamValid(param, &doc)) {
      result = newAnnouncement(doc["title"].GetString(), doc["content"].GetString(), doc["state"].GetString(), doc["date"].GetString());
      cout << "newAnnouncement" << endl << result << endl << endl;
      res.set_content(result,"application/json");
    }
    else {
      res.status = 400;
      res.set_content("ERROR JSON","text/plain");
    }
  });

  svr.Post("/modifyAnnouncement", [](const Request & req, Response &res) {
    string result;
    Document doc;
    ParseResult pRes = doc.Parse(req.body.c_str());
    vector<string> param = {"id", "title", "content", "state", "date"};
    if (pRes && CheckParameter(req.body) && isParamValid(param, &doc)) {
      result = modifyAnnouncement(doc["id"].GetString(), doc["title"].GetString(), doc["content"].GetString(), doc["state"].GetString(), doc["date"].GetString());
      cout << "modifyAnnouncement" << endl << result << endl << endl;
      res.set_content(result,"application/json");
    }
    else {
      res.status = 400;
      res.set_content("ERROR JSON","text/plain");
    }
  });

  svr.Post("/deleteAnnouncement", [](const Request & req, Response &res) {
    string result;
    Document doc;
    ParseResult pRes = doc.Parse(req.body.c_str());
    vector<string> param = {"id"};
    if (pRes && CheckParameter(req.body) && isParamValid(param, &doc)) {
      result = deleteAnnouncement(doc["id"].GetString());
      cout << "deleteAnnouncement" << endl << result << endl << endl;
      res.set_content(result,"application/json");
    }
    else {
      res.status = 400;
      res.set_content("ERROR JSON","text/plain");
    }
  });

  svr.Post("/modifyInchargeBuilding", [](const Request & req, Response &res) {
    string result;
    Document doc;
    ParseResult pRes = doc.Parse(req.body.c_str());
    vector<string> param = {"userID", "buildingID", "isAdd"};
    if (pRes && CheckParameter(req.body) && isParamValid(param, &doc)) {
      result = modifyInchargeBuilding(doc["userID"].GetString(), doc["buildingID"].GetString(), doc["isAdd"].GetString());
      cout << "modifyInchargeBuilding" << endl << result << endl << endl;
      res.set_content(result,"application/json");
    }
    else {
      res.status = 400;
      res.set_content("ERROR JSON","text/plain");
    }
  });

  svr.Post("/viewUserInfo", [](const Request & req, Response &res) {
    string result;
    Document doc;
    ParseResult pRes = doc.Parse(req.body.c_str());
    vector<string> param = {"userID", "userName", "buildingID", "familyID", "phone"};
    if (pRes && CheckParameter(req.body) && isParamValid(param, &doc)) {
      result = viewUserInfo(doc["userID"].GetString(), doc["userName"].GetString(), doc["buildingID"].GetString(), doc["familyID"].GetString(), doc["phone"].GetString());
      cout << "viewUserInfo" << endl << result << endl << endl;
      res.set_content(result,"application/json");
    }
    else {
      res.status = 400;
      res.set_content("ERROR JSON","text/plain");
    }
  });

  svr.Post("/physicalCondition", [](const Request & req, Response &res) {
    string result;
    Document doc;
    ParseResult pRes = doc.Parse(req.body.c_str());
    vector<string> param = {"userID", "date", "todayTemperature", "HuBeiContact"};
    if (pRes && CheckParameter(req.body) && isParamValid(param, &doc)) {
      result = physicalCondition(doc["userID"].GetString(), doc["date"].GetString(), doc["todayTemperature"].GetString(), doc["HuBeiContact"].GetString());
      cout << "physicalCondition" << endl << result << endl << endl;
      res.set_content(result,"application/json");
    }
    else {
      res.status = 400;
      res.set_content("ERROR JSON","text/plain");
    }
  });
  
  svr.Get("/epidemicSituation",[](const Request & req, Response &res){
    string result = epidemicSituation();
    res.set_content(result,"application/json");
  });

  svr.Post("/viewPhysicalCondition", [](const Request & req, Response &res) {
    string result;
    Document doc;
    ParseResult pRes = doc.Parse(req.body.c_str());
    vector<string> param = {"date"};
    if (pRes && CheckParameter(req.body) && isParamValid(param, &doc)) {
      result = viewPhysicalCondition(doc["date"].GetString());
      cout << "viewPhysicalCondition" << endl << result << endl << endl;
      res.set_content(result,"application/json");
    }
    else {
      res.status = 400;
      res.set_content("ERROR JSON","text/plain");
    }
  });

  svr.Post("/newEquipment", [](const Request & req, Response &res) {
    string result;
    Document doc;
    ParseResult pRes = doc.Parse(req.body.c_str());
    vector<string> param = {"equipmentName", "storage"};
    if (pRes && CheckParameter(req.body) && isParamValid(param, &doc)) {
      result = newEquipment(doc["equipmentName"].GetString(), doc["storage"].GetString());
      cout << "newEquipment" << endl << result << endl << endl;
      res.set_content(result,"application/json");
    }
    else {
      result = "{\"result\":\"-2\"} ";
    }
  });

  svr.Post("/modifyEquipment", [](const Request & req, Response &res) {
    string result;
    Document doc;
    ParseResult pRes = doc.Parse(req.body.c_str());
    vector<string> param = {"equipmentID", "storageChange"};
    if (pRes && CheckParameter(req.body) && isParamValid(param, &doc)) {
      result = modifyEquipment(doc["equipmentID"].GetString(), doc["storageChange"].GetString());
      cout << "modifyEquipment" << endl << result << endl << endl;
      res.set_content(result,"application/json");
    }
    else {
      res.status = 400;
      res.set_content("ERROR JSON","text/plain");
    }
  });

  svr.Post("/applyEquipment", [](const Request & req, Response &res) {
    string result;
    Document doc;
    ParseResult pRes = doc.Parse(req.body.c_str());
    vector<string> param = {"userID", "equipmentName", "amount", "date"};
    if (pRes && CheckParameter(req.body) && isParamValid(param, &doc)) {
      result = applyEquipment(doc["userID"].GetString(), doc["equipmentName"].GetString(), doc["amount"].GetString(), doc["date"].GetString());
      cout << "applyEquipment" << endl << result << endl << endl;
      res.set_content(result,"application/json");
    }
    else {
      res.status = 400;
      res.set_content("ERROR JSON","text/plain");
    }
  });

  svr.Post("/getApplyEquipment", [](const Request & req, Response &res) {
    string result;
    Document doc;
    ParseResult pRes = doc.Parse(req.body.c_str());
    vector<string> param = {"adminID"};
    if (pRes && CheckParameter(req.body) && isParamValid(param, &doc)) {
      result = getApplyEquipment(doc["adminID"].GetString());
      cout << "getApplyEquipment" << endl << result << endl << endl;
      res.set_content(result,"application/json");
    }
    else {
      res.status = 400;
      res.set_content("ERROR JSON","text/plain");
    }
  });

  svr.Post("/handleApplication", [](const Request & req, Response &res) {
    string result;
    Document doc;
    ParseResult pRes = doc.Parse(req.body.c_str());
    vector<string> param = {"applyID", "result", "adminID", "reply", "date"};
    if (pRes && CheckParameter(req.body) && isParamValid(param, &doc)) {
      result = handleApplication(doc["applyID"].GetString(), doc["result"].GetString(), doc["adminID"].GetString(), doc["reply"].GetString(), doc["date"].GetString());
      cout << "handleApplication" << endl << result << endl << endl;
      res.set_content(result,"application/json");
    }
    else {
      res.status = 400;
      res.set_content("ERROR JSON","text/plain");
    }
  });
  svr.Post("/QuarantineInformation", [](const Request & req, Response &res) {
    Document doc;
    ParseResult pRes = doc.Parse(req.body.c_str());
    vector<string> param = {"type", "userID"};
    if(pRes && isParamValid(param, &doc)){
      string result = GetQuarantineInfo(doc["type"].GetString(), doc["userID"].GetString());
      res.set_content(result,"application/json");
    }
    else{
      res.status = 400;
      res.set_content("ERROR JSON","text/plain");
    }
  });
  svr.Post("/QuarantineStatChange", [](const Request & req, Response &res) {
    Document doc;
    ParseResult pRes=doc.Parse(req.body.c_str());
    vector<string> param = {"userID", "adminID", "endDate"};
    if(pRes && isParamValid(param, &doc)){
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
    vector<string> param = {"userID", "adminID", "state"};
    if(pRes && isParamValid(param, &doc)){
      string result = SetUserHealthStat(doc["userID"].GetString(), doc["adminID"].GetString(),doc["state"].GetString());
      res.set_content(result,"application/json");
    }
    else{
      res.status = 400;
      res.set_content("ERROR JSON","text/plain");
    }
  });
  svr.Get("/GetAllEquipment",[](const Request & req, Response &res){
    string result = getAllEquipment();
    res.set_content(result,"application/json");
  });
  svr.Get("/GetAllAnnouncement",[](const Request & req, Response &res){
    string result=getAllAnnouncement();
    res.set_content(result,"application/json");
  });
  svr.Post("/getApplyStatus", [](const Request & req, Response &res) {
    Document doc;
    ParseResult pRes = doc.Parse(req.body.c_str());
    vector<string> param = {"userID", "type"};
    if(pRes && isParamValid(param, &doc)){
      string result = getApplyEquipment(doc["userID"].GetString(), doc["type"].GetString());
      res.set_content(result,"application/json");
    }
    else{
      res.status = 400;
      res.set_content("ERROR JSON","text/plain");
    }
  });
  
  svr.listen("0.0.0.0", port);
}
