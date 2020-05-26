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

void GenJsonObjectArray(const string& ArrayName,const unordered_map<int,string>& keyNames,const vector<vector<string>>& queryResult,Document& jsonDoc)
{
    if(queryResult.empty() || ArrayName.empty()){
        return;
    }
    Value InfoArray(kArrayType);
    Document::AllocatorType& allocator = jsonDoc.GetAllocator();
    assert(keyNames.size()==queryResult[0].size());
    for(unsigned int i=0;i<queryResult.size();++i){
        Value objValue,queryValue,keyValue;
        objValue.SetObject();
        for(unsigned int j=0;j<queryResult[i].size();++j){
            queryValue.SetString(queryResult[i][j].c_str(),allocator);
            keyValue.SetString(keyNames.find(j)->second.c_str(),allocator);
            objValue.AddMember(keyValue,queryValue,allocator);
        }
        InfoArray.PushBack(objValue, allocator);
    }
    Value ArrayNameValue;
    ArrayNameValue.SetString(ArrayName.c_str(),allocator);
    jsonDoc.AddMember(ArrayNameValue,InfoArray,allocator);
}
void GenJsonLinearArray(const string& ArrayName,const vector<string>& LinearArray,Document& jsonDoc)
{
    if(LinearArray.empty() || ArrayName.empty()){
        return;
    }
    Value InfoArray(kArrayType);
    Document::AllocatorType& allocator = jsonDoc.GetAllocator();
    for(unsigned int i=0;i<LinearArray.size();++i){
        Value ElementValue;
        ElementValue.SetString(LinearArray[i].c_str(),allocator);
        InfoArray.PushBack(ElementValue, allocator);
    }
    Value ArrayNameValue;
    ArrayNameValue.SetString(ArrayName.c_str(),allocator);
    jsonDoc.AddMember(ArrayNameValue,InfoArray,allocator);
}
void GenJsonObjectWithObjectValue(const string& ArrayName,const unordered_map<string,string>& keyValues,Document& jsonDoc)
{
    if(keyValues.empty() || ArrayName.empty()){
        return;
    }
    Value ObjectValue,k,v;
    ObjectValue.SetObject();
    Document::AllocatorType& allocator = jsonDoc.GetAllocator();
    for(auto it=keyValues.begin();it!=keyValues.end();++it){
        k.SetString(it->first.c_str(),allocator);
        v.SetString(it->second.c_str(),allocator);
        ObjectValue.AddMember(k,v,allocator);
    }
    Value ArrayNameValue;
    ArrayNameValue.SetString(ArrayName.c_str(),allocator);
    jsonDoc.AddMember(ArrayNameValue,ObjectValue,allocator);
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
