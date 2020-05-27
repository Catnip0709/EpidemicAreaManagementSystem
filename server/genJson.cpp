#include "genJson.h"

CGenJson::CGenJson() {
  jsonDoc.SetObject();
}

// 简单的返回json，只有一个key="result"
string CGenJson::genResultJson(int errCode) {
  StringBuffer s;
  Writer<StringBuffer> writer(s);
  writer.StartObject();
  writer.Key("result");
  writer.Uint(errCode);
  writer.EndObject();
  string result = s.GetString();
  return result;
}

// 生成内部json
void CGenJson::genInsideJson(Value &objValue, vector<string> key, vector<string> value, Document::AllocatorType& allocator) {
  objValue.SetObject();
  Value k[key.size()], v[value.size()];
  for (int i = 0; i < key.size(); ++i) {
    k[i].SetString(key[i].c_str(), allocator);
    v[i].SetString(value[i].c_str(), allocator);
    objValue.AddMember(k[i], v[i], allocator);
  }
}

// 生成内部一维数组
void CGenJson::genInsideArray1(Value &myArray, vector<string> key, vector<string> value, Document::AllocatorType& allocator) {
  for (int i = 0; i < value.size(); ++i) { // 每一组信息
    Value objValue;
    objValue.SetObject();
    Value k, v;
    k.SetString(key[i].c_str(), allocator);
    v.SetString(value[i].c_str(), allocator);
    objValue.AddMember(k, v, allocator);
    myArray.PushBack(objValue, allocator);
  }
}

// 生成内部二维数组
void CGenJson::genInsideArray2(Value &myArray, vector<string> key, vector<vector<string>> value, Document::AllocatorType& allocator) {
  //unordered_map<string, vector<string>>::iterator;
  for (int i = 0; i < value.size(); ++i) { // 每一组信息
    Value objValue;
    objValue.SetObject();
    for (int j = 0; j < value[i].size(); ++j) {
      Value k, v;
      k.SetString(key[j].c_str(), allocator);
      v.SetString(value[i][j].c_str(), allocator);
      objValue.AddMember(k, v, allocator);
    }
    myArray.PushBack(objValue, allocator);
  }
}

// 生成总json
string CGenJson::genJson(Document::AllocatorType& allocator) {
  StringBuffer strbuf;
  Writer<StringBuffer> writer(strbuf);
  jsonDoc.Accept(writer);
  return strbuf.GetString(); 
}