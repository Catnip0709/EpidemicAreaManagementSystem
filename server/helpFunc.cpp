#include "interface.h"

Document parseJson(const Request &req) {
  Document doc;
  if (req.get_header_value("Content-Type") == "application/json") {
    string str = "{" + req.body + "}";
    doc.Parse((char*)str.c_str());
  }
  return doc;
}