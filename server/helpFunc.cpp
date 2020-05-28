#include "interface.h"

Document* parseJson(const Request &req) {
  Document *doc=new Document();
  ParseResult pRes=doc->Parse(req.body.c_str());
  if(!pRes){
      return NULL;
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

// string根据cut符号切割
vector<string> stringCut(string str, string cut) {
  vector<string> result;
	int begin = 0;
	while (1) {
		int end = str.find(cut, begin);
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

vector<string> get7date() { // date为今天，获取今天及前6天的日期
    vector<string> result;
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    for (int i = 0; i < 7; i++) {
        timeinfo->tm_hour -= 24;
        mktime(timeinfo);
        string month = timeinfo->tm_mon + 1 < 10 ? "0" + to_string(timeinfo->tm_mon + 1) : to_string(timeinfo->tm_mon + 1);
        string tempDate = to_string(timeinfo->tm_year + 1900) + "-" + month + "-" + to_string(timeinfo->tm_mday);
        result.push_back(tempDate);
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
