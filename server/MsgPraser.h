#ifndef MSG_PRASER_H
#define MSG_PRASER_H
#include"common.h"
using namespace std;
class MsgPraser{
    public:
        int PraseJson(const string& PostMsg,vector<pair<string,string>>& JsonMsg);
    private:
};



#endif