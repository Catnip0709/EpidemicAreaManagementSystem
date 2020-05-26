#include "interface.h"

class CGenJson {
public:
    Document jsonDoc;
    CGenJson();
    static string genResultJson(int errCode);
    void genInsideJson(Value &objValue, vector<string> key, vector<string> value, Document::AllocatorType& allocator);
    void genInsideArray1(Value &myArray, vector<string> value, Document::AllocatorType& allocator);
    void genInsideArray2(Value &myArray, vector<string> key, vector<vector<string>> value, Document::AllocatorType& allocator);
    string genJson(Document::AllocatorType& allocator);
private:
    vector<Value> vecKey;
    vector<Value> vecValue;
};