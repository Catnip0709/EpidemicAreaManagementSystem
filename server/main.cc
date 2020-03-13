//
//  hello.cc
//
//  Copyright (c) 2019 Yuji Hirose. All rights reserved.
//  MIT License
//

#include <stdlib.h>
#include <httplib.h>
#include <iostream>
#include <unordered_map>

#include "myDB.h"
#include "MsgPraser.h"
#include "common.h"
#include "interface.h"

using namespace httplib;
using namespace std;

unordered_map<string, string> getParam(const Request & req) {
  unordered_map<string,string> jsonMsg;
  if(req.get_header_value("Content-Type") == "application/json"){
    MsgPraser JsonPraser;
    JsonPraser.PraseJson(req.body, jsonMsg);
  }
  return jsonMsg;
}

void coutMap(unordered_map<string, string> jsonMsg) { 
  unordered_map<string, string>::iterator mapi;
  for(mapi = jsonMsg.begin(); mapi != jsonMsg.end(); mapi++)
  {
    cout<<mapi->first<< " = " << mapi->second << endl;
  }
}

int main(int argc,char **argv) {
  if(argc != 2){
    cerr << "start paras error" << endl;
    return -1;
  }

  uint16_t port = atoi(argv[1]);
  Server svr;

  svr.Get("/hi", [](const Request & req, Response &res) {
    res.set_content("Hello World!", "text/plain");
  });

  svr.Post("/userRegister", [](const Request & req, Response &res) {
    unordered_map<string, string> jsonMsg = getParam(req);
    int registerResult = userRegister(jsonMsg["userID"], jsonMsg["userName"], jsonMsg["password"], atoi(jsonMsg["isAdministrator"].c_str()), atoi(jsonMsg["buildingID"].c_str()), atoi(jsonMsg["familyID"].c_str()));
    // 返回结果给客户端
  });

  svr.Post("/userLogin", [](const Request & req, Response &res) {
    unordered_map<string, string> jsonMsg = getParam(req);
    int loginResult = userLogin(jsonMsg["userID"], jsonMsg["password"]);
    // 返回结果给客户端
  });

  svr.listen("0.0.0.0", port);
}
