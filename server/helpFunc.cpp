#include "interface.h"

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

vector<string> get7date() { // 获取今天及前6天的日期，递增
    vector<string> result;
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    timeinfo->tm_hour -= 24 * 8;
    for (int i = 0; i < 8; i++) {
        timeinfo->tm_hour += 24;
        mktime(timeinfo);
        string month = timeinfo->tm_mon + 1 < 10 ? "0" + to_string(timeinfo->tm_mon + 1) : to_string(timeinfo->tm_mon + 1);
        string day = timeinfo->tm_mday < 10 ? "0" + to_string(timeinfo->tm_mday) : to_string(timeinfo->tm_mday);
        string tempDate = to_string(timeinfo->tm_year + 1900) + "-" + month + "-" + day;
        result.push_back(tempDate);
    }
    return result;
}

// 判断日期是不是XXXX-YY-ZZ型，如果不是则补齐
string dateVerification(string date) {
    vector<string> cutDate = stringCut(date, "-");
    if (cutDate[1].size() == 1) {
        cutDate[1] = "0" + cutDate[1];
    }
    if (cutDate[2].size() == 1) {
        cutDate[2] = "0" + cutDate[2];
    }
    return cutDate[0] + "-" + cutDate[1] + "-" + cutDate[2];
}

//传入的body中是否有该参数，防止服务器挂掉
bool isParamValid(vector<string> &param, Document *doc) {
    for (int i = 0; i < param.size(); ++i) {
        if(!doc->HasMember(param[i].c_str())) {
            return false;
        }
        if(!(*doc)[param[i].c_str()].IsString()){
            return false;
        }
    }
    if (doc->HasMember("date")) {
        string date = dateVerification((*doc)["date"].GetString());
        Document::AllocatorType& allocator = (*doc).GetAllocator();
        (*doc)["date"].SetString(date.c_str(), allocator);
    }
    return true;
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
