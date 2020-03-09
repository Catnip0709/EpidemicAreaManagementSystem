#include"MsgPraser.h"
int MsgPraser::PraseJson(const string& PostMsg,vector<pair<string,string>>& JsonMsg){
    int iRet=SUCCESS;
    do{
        string Msg(PostMsg);
        if(Msg.empty()){
            iRet=ERROR;
            break;
        }
        size_t pos=0,begin=0;
        string SubJson;
        SubJson.clear();
        while((pos=Msg.find_first_of(','))!=Msg.npos){
            SubJson=Msg.substr(begin,pos-begin);
            size_t SubJsonSpiltPos=SubJson.find(':');
            JsonMsg.push_back(pair<string,string>(SubJson.substr(begin+1,SubJsonSpiltPos-begin-2),
                                                  SubJson.substr(SubJsonSpiltPos+2,SubJson.length()-SubJsonSpiltPos-3)));
            Msg=Msg.substr(pos+1,Msg.length()-pos-1);  
            SubJson.clear();                            
        }
        size_t SubJsonSpiltPos=Msg.find(':');
        JsonMsg.push_back(pair<string,string>(Msg.substr(1,SubJsonSpiltPos-2),
                                              Msg.substr(SubJsonSpiltPos+2,Msg.length()-SubJsonSpiltPos-3)));
    }while(false);
    return iRet;
}