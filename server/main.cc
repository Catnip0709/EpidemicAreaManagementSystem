//
//  hello.cc
//
//  Copyright (c) 2019 Yuji Hirose. All rights reserved.
//  MIT License
//

#include <httplib.h>
#include "myDB.h"
using namespace httplib;

void testDB(){
  MyDB db;
	db.initDB("localhost", "root", "0709", "EpidemicDatabase");
	db.exeSQL("INSERT INTO User VALUES(\"452223199707090000\", \"catnip\", \"0709\", false)");
}

int main(void) {
  Server svr;
  svr.Get("/hi", [](const Request & /*req*/, Response &res) {
    res.set_content("Hello World!", "text/plain");
  });
  svr.Get("/test", [](const Request & /*req*/, Response &res) {
    testDB();
    res.set_content("test tencent!", "text/plain");
  });
  svr.listen("0.0.0.0", 1234);
}
