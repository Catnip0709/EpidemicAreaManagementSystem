#ifndef MSG_PRASER_H
#define MSG_PRASER_H
#include"common.h"
#include <unordered_map>
using namespace std;
class MsgPraser{
    public:
        int PraseJson(const string& PostMsg, unordered_map<string,string>& JsonMsg);
    private:
};

#endif