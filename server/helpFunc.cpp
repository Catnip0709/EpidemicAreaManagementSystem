#include "interface.h"

Document parseJson(const Request &req) {
  Document doc;
  if (req.get_header_value("Content-Type") == "application/json") {
    string str = "{" + req.body + "}";
    doc.Parse((char*)str.c_str());
  }
  return doc;
}

// 检查SQL语句是否被注入
bool CheckSQL(string sql) {
    string key[9] = { "%","/","union","|","&","^" ,"#","/*","*/"};
    for (int i = 0; i < 9; i++) {
        if (sql.find(key[i]) != string::npos) {
            return false;
        }
    }
    return true;
}

// 检查SQL参数是否被注入
bool CheckParameter(string param) {
    string key[14] = {"and","*","="," ","%0a","%","/","union","|","&","^" ,"#","/*","*/" };
    for (int j = 0; j < 14; ++j) {
        if (param.find(key[j]) != string::npos) {
            return false;
        }
    }
    return true;
}

// string根据下划线切割
vector<string> stringCut(string str) {
  vector<string> result;
	int begin = 0;
	while (1) {
		int end = str.find("_", begin);
		if (end == -1) {
			end = str.size();
		}
		result.push_back(str.substr(begin, end - begin));
		begin = end + 1;
		if (end == str.size()) {
			break;
		}
	}
	return result;
}

// 简单的返回json，只有一个key
string genResultJson(int errCode) {
  StringBuffer s;
  Writer<StringBuffer> writer(s);
  writer.StartObject();
  writer.Key("result");
  writer.Uint(errCode);
  writer.EndObject();
  string result = s.GetString();
  return result;
}