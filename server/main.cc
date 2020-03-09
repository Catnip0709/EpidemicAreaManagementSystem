//
//  hello.cc
//
//  Copyright (c) 2019 Yuji Hirose. All rights reserved.
//  MIT License
//

#include <httplib.h>
#include<iostream>
#include "myDB.h"
#include<stdlib.h>
#include"MsgPraser.h"
#include"common.h"
using namespace httplib;
using namespace std;
void testDB(){
  MyDB db;
	db.initDB("localhost", "root", "0709", "EpidemicDatabase");
	db.exeSQL("INSERT INTO User VALUES(\"452223199707090000\", \"catnip\", \"0709\", false)");
}

int main(int argc,char **argv) {
  if(argc!=2){
    cerr<<"start paras error"<<endl;
    return -1;
  }
  uint16_t port=atoi(argv[1]);
  Server svr;
  svr.Get("/hi", [](const Request & req, Response &res) {
    res.set_content("Hello World!", "text/plain");
  });
  svr.Get("/test", [](const Request & req, Response &res) {
    testDB();
    res.set_content("test tencent!", "text/plain");
  });
  svr.Post("/login",[](const Request & req, Response &res){
    if(req.get_header_value("Content-Type")=="application/json"){
       MsgPraser JsonPraser;
       vector<pair<string,string>> JsonMsg;
       JsonPraser.PraseJson(req.body,JsonMsg);
       for(auto it=JsonMsg.begin();it!=JsonMsg.end();++it){
         cout<<(*it).first<<" "<<(*it).second<<endl;
       }
    }
    res.set_content("Hello World!", "text/plain");
  });

  svr.listen("0.0.0.0",port);
}
